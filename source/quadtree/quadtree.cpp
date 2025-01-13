#include "quadtree.h"

#include "quadtreeNode.h"
#include <set>
#include <algorithm>
#include <stdexcept>
#include <omp.h>

//Cut-Off-Wert für construct_task_with_cutoff
const int cutoff_threshold = 100;


Quadtree::Quadtree(Universe& universe, BoundingBox bounding_box, std::int8_t construct_mode) {
    root = new QuadtreeNode(bounding_box);  // Initialisiere den Wurzelknoten

    // Liste der Indizes der Himmelskörper innerhalb der BoundingBox erstellen
    std::vector<std::int32_t> body_indices;

    // Durchlaufe alle Himmelskörper im Universum
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        const Vector2d<double>& position = universe.positions[i];

        // Überprüfe, ob der Körper innerhalb der BoundingBox liegt
        if (position[0] >= bounding_box.x_min && position[0] <= bounding_box.x_max &&
            position[1] >= bounding_box.y_min && position[1] <= bounding_box.y_max) {
            body_indices.push_back(i);  // Füge den Körperindex zur Liste hinzu
        }
    }

    // Wähle den Konstruktionsmodus und rufe die entsprechende Funktion auf
    switch (construct_mode) {
    case 0:
        root->children = construct(universe, bounding_box, body_indices);
        break;
    case 1:
        root->children = construct_task(universe, bounding_box, body_indices);
        break;
    case 2:
        root->children = construct_task_with_cutoff(universe, bounding_box, body_indices);
        break;
    default:
        std::cerr << "Unbekannter Konstruktionmodus!" << std::endl;
        break;
    }
}

Quadtree::~Quadtree() {
    delete root;  // Lösche den Wurzelknoten und seine Kinder rekursiv
}

void Quadtree::calculate_cumulative_masses() {
    root->calculate_node_cumulative_mass();
}

void Quadtree::calculate_center_of_mass() {
    root->calculate_node_center_of_mass();
}


std::vector<QuadtreeNode*> Quadtree::construct(Universe& universe, BoundingBox BB, std::vector<std::int32_t> body_indices) {
    std::vector<QuadtreeNode*> nodes;

    // Wenn nur ein Körper im Quadranten ist, erstellen wir einen Blattknoten
    if (body_indices.size() == 1) {
        QuadtreeNode* node = new QuadtreeNode(BB);
        node->body_identifier = body_indices[0];  // Setze den Körperindex im Blattknoten
        nodes.push_back(node);
    }
    else {
        // Wenn mehr als ein Körper im Quadranten ist, teilen wir den Raum in 4 Subquadranten

        // Berechne die Grenzen für die Subquadranten
        double x_mid = (BB.x_min + BB.x_max) / 2.0;
        double y_mid = (BB.y_min + BB.y_max) / 2.0;

        // Subquadranten erstellen
        BoundingBox childBBs[4] = {
            BoundingBox(BB.x_min, x_mid, BB.y_min, y_mid),  // links unten
            BoundingBox(x_mid, BB.x_max, BB.y_min, y_mid),  // rechts unten
            BoundingBox(BB.x_min, x_mid, y_mid, BB.y_max),  // links oben
            BoundingBox(x_mid, BB.x_max, y_mid, BB.y_max)   // rechts oben
        };

        // Indizes der Himmelskörper in den Subquadranten aufteilen
        std::vector<std::int32_t> child_indices[4];

        for (auto idx : body_indices) {
            const Vector2d<double>& pos = universe.positions[idx];
            if (pos[0] < x_mid) {
                if (pos[1] < y_mid)
                    child_indices[0].push_back(idx);  // links unten
                else
                    child_indices[2].push_back(idx);  // links oben
            }
            else {
                if (pos[1] < y_mid)
                    child_indices[1].push_back(idx);  // rechts unten
                else
                    child_indices[3].push_back(idx);  // rechts oben
            }
        }

        // Rekursiver Aufruf für die Subquadranten
        for (int i = 0; i < 4; ++i) {
            if (!child_indices[i].empty()) {
                std::vector<QuadtreeNode*> children = construct(universe, childBBs[i], child_indices[i]);
                QuadtreeNode* internal_node = new QuadtreeNode(BB);  // Erstelle einen internen Knoten
                internal_node->children = children;  // Setze die Subquadranten als Kinder
                nodes.push_back(internal_node);
            }
        }
    }

    return nodes;
}

std::vector<QuadtreeNode*> Quadtree::construct_task(Universe& universe, BoundingBox BB, std::vector<std::int32_t> body_indices) {
    std::vector<QuadtreeNode*> nodes;

    // If there's only one body in this quadrant, create a leaf node
    if (body_indices.size() == 1) {
        QuadtreeNode* node = new QuadtreeNode(BB);
        node->body_identifier = body_indices[0]; // Set the body index in the leaf node
        nodes.push_back(node);
    }
    else {
        // More than one body: split the space into 4 sub-quadrants
        double x_mid = (BB.x_min + BB.x_max) / 2.0;
        double y_mid = (BB.y_min + BB.y_max) / 2.0;

        BoundingBox childBBs[4] = {
            BoundingBox(BB.x_min, x_mid, BB.y_min, y_mid),  // Bottom-left
            BoundingBox(x_mid, BB.x_max, BB.y_min, y_mid),  // Bottom-right
            BoundingBox(BB.x_min, x_mid, y_mid, BB.y_max),  // Top-left
            BoundingBox(x_mid, BB.x_max, y_mid, BB.y_max)   // Top-right
        };

        // Allocate child body indices
        std::vector<std::int32_t> child_indices[4];
        for (auto idx : body_indices) {
            const Vector2d<double>& pos = universe.positions[idx];
            if (pos[0] < x_mid) {
                if (pos[1] < y_mid)
                    child_indices[0].push_back(idx); // Bottom-left
                else
                    child_indices[2].push_back(idx); // Top-left
            }
            else {
                if (pos[1] < y_mid)
                    child_indices[1].push_back(idx); // Bottom-right
                else
                    child_indices[3].push_back(idx); // Top-right
            }
        }

        // Preallocate space for nodes to avoid thread-safety issues
        nodes.resize(4, nullptr);

        // Parallelize the processing of sub-quadrants
#pragma omp parallel for
        for (int i = 0; i < 4; ++i) {
            if (!child_indices[i].empty()) {
                // Recursively construct the quadtree for this sub-quadrant
                std::vector<QuadtreeNode*> children = construct_task(universe, childBBs[i], child_indices[i]);

                // Create an internal node for this quadrant
                QuadtreeNode* internal_node = new QuadtreeNode(BB);
                internal_node->children = children;

                // Safely assign the node to the preallocated vector
                nodes[i] = internal_node;
            }
        }

        // Remove nullptr entries (for empty quadrants)
        nodes.erase(std::remove(nodes.begin(), nodes.end(), nullptr), nodes.end());
    }

    return nodes;
}


std::vector<QuadtreeNode*> Quadtree::construct_task_with_cutoff(Universe& universe, BoundingBox& BB, std::vector<std::int32_t>& body_indices) {
    std::vector<QuadtreeNode*> nodes;

    // Base case: if the cutoff threshold is reached, create a leaf node
    if (body_indices.size() <= cutoff_threshold) {
        QuadtreeNode* node = new QuadtreeNode(BB);
        node->body_identifier = body_indices[0];  // Set the body index in the leaf node
        nodes.push_back(node);
    }
    else {
        // Calculate the boundaries for the 4 subquadrants
        double x_mid = (BB.x_min + BB.x_max) / 2.0;
        double y_mid = (BB.y_min + BB.y_max) / 2.0;

        // Define the bounding boxes for the 4 subquadrants
        BoundingBox childBBs[4] = {
            BoundingBox(BB.x_min, x_mid, BB.y_min, y_mid),  // Bottom-left
            BoundingBox(x_mid, BB.x_max, BB.y_min, y_mid),  // Bottom-right
            BoundingBox(BB.x_min, x_mid, y_mid, BB.y_max),  // Top-left
            BoundingBox(x_mid, BB.x_max, y_mid, BB.y_max)   // Top-right
        };

        // Distribute the body indices into the 4 subquadrants
        std::vector<std::int32_t> child_indices[4];
        for (auto idx : body_indices) {
            const Vector2d<double>& pos = universe.positions[idx];
            if (pos[0] < x_mid) {
                if (pos[1] < y_mid)
                    child_indices[0].push_back(idx);  // Bottom-left
                else
                    child_indices[2].push_back(idx);  // Top-left
            }
            else {
                if (pos[1] < y_mid)
                    child_indices[1].push_back(idx);  // Bottom-right
                else
                    child_indices[3].push_back(idx);  // Top-right
            }
        }

        // Parallelize the construction of subquadrants
        std::vector<QuadtreeNode*> children[4];

#pragma omp parallel for shared(children, childBBs, child_indices)
        for (int i = 0; i < 4; ++i) {
            if (!child_indices[i].empty()) {
                // Recursively construct the subtree for each non-empty subquadrant
                children[i] = construct_task_with_cutoff(universe, childBBs[i], child_indices[i]);
            }
        }

        // Combine the child nodes into a single internal node
        QuadtreeNode* internal_node = new QuadtreeNode(BB);
        for (int i = 0; i < 4; ++i) {
            if (!children[i].empty()) {
                for (QuadtreeNode* child : children[i]) {
                    internal_node->children.push_back(child);
                }
            }
        }
        nodes.push_back(internal_node);
    }

    return nodes;
}

std::vector<BoundingBox> Quadtree::get_bounding_boxes(QuadtreeNode* qtn) {
    // traverse quadtree and collect bounding boxes
    std::vector<BoundingBox> result;
    // collect bounding boxes from children
    for (auto child : qtn->children) {
        for (auto bb : get_bounding_boxes(child)) {
            result.push_back(bb);
        }
    }
    result.push_back(qtn->bounding_box);
    return result;
}








