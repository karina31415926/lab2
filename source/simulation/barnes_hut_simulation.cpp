#include "simulation/barnes_hut_simulation.h"
#include "simulation/naive_parallel_simulation.h"
#include "physics/gravitation.h"
#include "physics/mechanics.h"

#include <cmath>

void BarnesHutSimulation::simulate_epochs(Plotter& plotter, Universe& universe, std::uint32_t num_epochs, bool create_intermediate_plots, std::uint32_t plot_intermediate_epochs) {
    for (int i = 0; i < num_epochs; i++) {
        simulate_epoch(plotter, universe, create_intermediate_plots, plot_intermediate_epochs);
    }
}

void BarnesHutSimulation::simulate_epoch(Plotter& plotter, Universe& universe, bool create_intermediate_plots, std::uint32_t plot_intermediate_epochs) {
    // 1. Erzeuge den Quadtree für den aktuellen Zustand des Universums
    Quadtree quadtree(universe, universe.get_bounding_box(), 2);  // Modus 2: Parallele Erstellung mit Cut-Off
    // Der Quadtree wird basierend auf der Bounding Box und den Körper-Indizes aufgebaut

    // 2. Berechne die kumulierten Massen und Massenschwerpunkte aller Quadranten
    quadtree.calculate_cumulative_masses();
    quadtree.calculate_center_of_mass();

    // 3. Berechne die auftretenden Kräfte
    calculate_forces(universe, quadtree);

    // 4. Berechne die resultierenden Bewegungsgeschwindigkeiten und Positionsänderungen
    NaiveParallelSimulation::calculate_velocities(universe);  // Berechnung der neuen Geschwindigkeiten
    NaiveParallelSimulation::calculate_positions(universe);  // Berechnung der neuen Positionen

    // 5. Inkrementiere den Epochenzähler
    universe.current_simulation_epoch++;

    // 6. Plotten der Zwischenstände, wenn gewünscht
    if (create_intermediate_plots && (universe.current_simulation_epoch % plot_intermediate_epochs == 0)) {
        // Zeichne die Positionen aller Himmelskörper im Universum
        for (std::uint32_t i = 0; i < universe.num_bodies; i++) {
            const Vector2d<double>& position = universe.positions[i];
            // Markiere die Position der Himmelskörper auf dem Plot
            plotter.mark_position(position, 255, 0, 0);  // Rot für die Position
        }

        // Speichere den aktuellen Plot und leere das Bild
        plotter.write_and_clear();
    }
}

void BarnesHutSimulation::get_relevant_nodes(Universe& universe, Quadtree& quadtree, std::vector<QuadtreeNode*>& relevant_nodes, Vector2d<double>& body_position, std::int32_t body_index, double threshold_theta) {
    // Berechne die relevanten Knoten, die zur Berechnung der Kräfte verwendet werden
    // Dies ist eine rekursive Funktion, also rufen wir sie für die Wurzel des Quadtrees auf
    get_relevant_nodes_recursive(quadtree.root, body_position, body_index, threshold_theta, relevant_nodes);
}

//Hilfsfunktion von get_relevant_nodes
void BarnesHutSimulation::get_relevant_nodes_recursive(QuadtreeNode* node, Vector2d<double>& body_position, std::int32_t body_index, double threshold_theta, std::vector<QuadtreeNode*>& relevant_nodes) {
    // Berechne den Durchmesser des Quadranten des aktuellen Knotens (BoundingBox-Diagonale)
    double d = node->bounding_box.get_diagonal();

    // Berechne die Distanz zwischen dem Massenschwerpunkt des Quadranten und dem Körper K
    Vector2d<double> delta = body_position - node->center_of_mass;
    double r = delta.length();

    // Berechne ? = d / r
    double theta = d / r;

    // Wenn der Quadrant den Körper K enthält, muss er weiter unterteilt werden
    if (node->body_identifier == body_index) {
        // Falls es sich um einen Blattknoten handelt, der nur Körper K enthält, wird dieser Knoten als relevant betrachtet
        if (node->children.empty()) {
            relevant_nodes.push_back(node);
        }
    }
    // Wenn der Quadrant klein genug ist oder der Schwellwert erreicht wird, betrachten wir ihn als relevant
    else if (theta <= threshold_theta) {
        relevant_nodes.push_back(node);
    }
    // Andernfalls teilen wir den Quadranten weiter auf (rekursive Aufrufe)
    else {
        // Wenn der Quadrant Kinder hat, durchsuchen wir diese
        for (auto* child : node->children) {
            get_relevant_nodes_recursive(child, body_position, body_index, threshold_theta, relevant_nodes);
        }
    }
}

void BarnesHutSimulation::calculate_forces(Universe& universe, Quadtree& quadtree) {
    // Definiere den Schwellwert für ?
    const double threshold_theta = 0.2;

    // Vektor für die relevanten Knoten
    std::vector<QuadtreeNode*> relevant_nodes;

    // Berechne die relevanten Knoten, die zur Berechnung der Kräfte verwendet werden
    get_relevant_nodes(universe, quadtree, relevant_nodes, universe.positions[0], 0, threshold_theta);

    // Parallele Schleife, um die Kräfte für jeden Körper zu berechnen
#pragma omp parallel for
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        Vector2d<double> total_force(0.0, 0.0);  // Initialisiere die Gesamtkräfte für den Körper i

        // Berechne die Kräfte aus den relevanten Knoten
        for (auto* node : relevant_nodes) {
            if (node->body_identifier == -1) {
                // Wenn der Knoten kein Blattknoten ist, berechne die Gravitationskraft zwischen Körper i und dem Massenschwerpunkt des Knotens
                Vector2d<double> delta = universe.positions[i] - node->center_of_mass;
                double distance = delta.length();  // Berechnung der Distanz

                // Vermeide Division durch Null
                if (distance > 0) {
                    double force_magnitude = gravitational_force(universe.weights[i], node->cumulative_mass, distance);

                    // Normalisiere den Vektor und berechne die Kraft
                    Vector2d<double> force_direction = delta / distance;
                    total_force = total_force + force_direction * force_magnitude;
                }
            }
            else {
                // Wenn der Knoten ein Blattknoten ist, berechne die Kraft zwischen Körper i und dem Körper im Knoten
                if (i != node->body_identifier) {  // Vermeide, dass ein Körper seine eigene Kraft berechnet
                    Vector2d<double> delta = universe.positions[i] - universe.positions[node->body_identifier];
                    double distance = delta.length();

                    // Vermeide Division durch Null
                    if (distance > 0) {
                        double force_magnitude = gravitational_force(universe.weights[i], universe.weights[node->body_identifier], distance);

                        // Normalisiere den Vektor und berechne die Kraft
                        Vector2d<double> force_direction = delta / distance;
                        total_force = total_force + force_direction * force_magnitude;
                    }
                }
            }
        }

        // Speichere die berechnete Kraft für Körper i
        universe.forces[i] = total_force;
    }
}