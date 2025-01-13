#include "simulation/naive_parallel_simulation.h"
#include "simulation/constants.h"
#include "physics/gravitation.h"
#include "physics/mechanics.h"

#include <cmath>

void NaiveParallelSimulation::simulate_epochs(Plotter& plotter, Universe& universe, std::uint32_t num_epochs, bool create_intermediate_plots, std::uint32_t plot_intermediate_epochs){
    for(int i = 0; i < num_epochs; i++){
        simulate_epoch(plotter, universe, create_intermediate_plots, plot_intermediate_epochs);
    }
}

void NaiveParallelSimulation::simulate_epoch(Plotter& plotter, Universe& universe, bool create_intermediate_plots, std::uint32_t plot_intermediate_epochs){
    calculate_forces(universe);
    calculate_velocities(universe);
    calculate_positions(universe);
    universe.current_simulation_epoch++;
    if(create_intermediate_plots){
        if(universe.current_simulation_epoch % plot_intermediate_epochs == 0){
            plotter.add_bodies_to_image(universe);
            plotter.write_and_clear();
        }
    }
}


void NaiveParallelSimulation::calculate_forces(Universe& universe) {
    // Initialize forces to zero for all bodies
#pragma omp parallel for
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        universe.forces[i] = Vector2d<double>{ 0.0, 0.0 };
    }

    // Compute the forces between all pairs of bodies
#pragma omp parallel for
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        for (std::uint32_t j = i + 1; j < universe.num_bodies; ++j) {
            // Calculate the displacement vector between body i and body j
            Vector2d<double> displacement = universe.positions[j] - universe.positions[i];

            // Compute the distance between body i and body j
            double distance = std::sqrt(displacement[0] * displacement[0] + displacement[1] * displacement[1]);

            // Prevent division by zero if bodies are at the same position
            if (distance == 0) {
                throw std::invalid_argument("Bodies are at the same position.");
            }

            // Calculate the gravitational force magnitude between the bodies
            double force_magnitude = gravitational_force(universe.weights[i], universe.weights[j], distance);

            // Normalize the displacement vector to get the direction of the force
            Vector2d<double> force_direction = displacement / distance;

            // Calculate the force vector between body i and body j
            Vector2d<double> force = force_direction * force_magnitude;

            // Apply the forces to both bodies (action-reaction principle)

            #pragma omp critical
            {
                universe.forces[i] = universe.forces[i] + force;
                universe.forces[j] = universe.forces[j] - force;
            }
        }
    }
}

void NaiveParallelSimulation::calculate_velocities(Universe& universe) {
    // Parallel loop with OpenMP to update velocities for all bodies
#pragma omp parallel for
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        // Calculate the acceleration using Newton's second law: a = F / m
        Vector2d<double> acceleration = universe.forces[i] / universe.weights[i];

        // Calculate the new velocity: v' = v_0 + a * t
        Vector2d<double> new_velocity = universe.velocities[i] + acceleration * epoch_in_seconds;

        // Update the velocity for the body in the universe
        universe.velocities[i] = new_velocity;
    }
}

void NaiveParallelSimulation::calculate_positions(Universe& universe) {
    // Parallel loop with OpenMP to update positions for all bodies
#pragma omp parallel for
    for (std::uint32_t i = 0; i < universe.num_bodies; ++i) {
        // Calculate the displacement: s = v * t
        Vector2d<double> displacement = universe.velocities[i] * epoch_in_seconds;

        // Update the position: p' = p0 + s
        universe.positions[i] = universe.positions[i] + displacement;
    }
}