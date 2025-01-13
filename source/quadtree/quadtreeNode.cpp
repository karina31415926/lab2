#include "quadtreeNode.h"

#include <iostream>

#include "structures/universe.h"


double QuadtreeNode::calculate_node_cumulative_mass() {
    // Wenn die kumulierte Masse bereits berechnet wurde, überspringen wir die Berechnung
    if (cumulative_mass_ready) {
        return cumulative_mass;
    }
    Universe universe = Universe();
    // Wenn es sich um einen Blattknoten handelt, setze die kumulierte Masse auf die Masse des Körpers
    if (body_identifier != -1) {
        // Annahme: Der Körper mit dem `body_identifier` existiert im Universum und seine Masse kann abgerufen werden
        cumulative_mass = universe.weights[body_identifier];
    }
    else {
        // Für interne Knoten: Summiere die kumulierte Masse aller Kinder
        cumulative_mass = 0.0;
        for (auto* child : children) {
            cumulative_mass += child->calculate_node_cumulative_mass();  // Rekursiver Aufruf für Kinder
        }
    }

    // Nachdem die kumulierte Masse berechnet wurde, markieren wir das Flag als "fertig"
    cumulative_mass_ready = true;

    // Gebe die kumulierte Masse zurück
    return cumulative_mass;
}

QuadtreeNode::QuadtreeNode(BoundingBox arg_bounding_box)
    : bounding_box(arg_bounding_box), body_identifier(-1), center_of_mass_ready(false),
    cumulative_mass_ready(false), cumulative_mass(0.0) {
    children.clear();  // Keine Kinder für Blattknoten
}

QuadtreeNode::~QuadtreeNode() {
    for (auto* child : children) {
        delete child;
    }
}

Vector2d<double> QuadtreeNode::calculate_node_center_of_mass() {
    // Wenn der Massenschwerpunkt bereits berechnet wurde, überspringen wir die Berechnung
    if (center_of_mass_ready) {
        return center_of_mass;
    }
    Universe universe = Universe();
    // Wenn es sich um einen Blattknoten handelt, ist der Massenschwerpunkt die Position des Körpers
    if (body_identifier != -1) {
        center_of_mass = universe.positions[body_identifier];
    }
    else {
        // Für interne Knoten: Berechne den Massenschwerpunkt als gewichteten Durchschnitt der Kinder
        Vector2d<double> weighted_center(0.0, 0.0);
        double total_mass = 0.0;

        // Berechne den Massenschwerpunkt jedes Kindes
        for (auto* child : children) {
            // Berechne den Massenschwerpunkt des Kindes
            child->calculate_node_center_of_mass();

            // Summiere die gewichteten Schwerpunkte
            weighted_center = weighted_center + child->center_of_mass * child->cumulative_mass;  // Vektoraddition

            total_mass += child->cumulative_mass;  // Addiere die Masse des Kindes
        }

        // Berechne den Gesamt-Massenschwerpunkt für diesen Knoten
        if (total_mass > 0.0) {
            center_of_mass = weighted_center / total_mass;
        }
    }

    // Markiere den Knoten als fertig berechnet
    center_of_mass_ready = true;

    // Gib den Massenschwerpunkt zurück
    return center_of_mass;
}