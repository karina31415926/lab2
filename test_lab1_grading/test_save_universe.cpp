#include "test.h"

#include "structures/universe.h"
#include "input_generator/input_generator.h"

#include "utilities/import.hpp"
#include "utilities/export.hpp"

#include <exception>
#include <iostream>
#include <filesystem>
#include <cstdio>
#include "utilities.h"

class SaveUniverseTest : public LabTest {};

TEST_F(SaveUniverseTest, test_three_a_ppws24_D75C){
    // fields
    auto universe = Universe();
    InputGenerator::create_random_universe(1000, universe);
    
    auto tmp_path = std::filesystem::path{"test_three_a_ppws24_D75C_universe.txt"};

    save_universe(tmp_path, universe);

    Universe loaded_universe;
    load_universe(tmp_path, loaded_universe);

    ASSERT_EQ(universe.num_bodies, loaded_universe.num_bodies);
    ASSERT_EQ(universe.weights.size(), loaded_universe.weights.size());
    ASSERT_EQ(universe.forces.size(), loaded_universe.forces.size());
    ASSERT_EQ(universe.velocities.size(), loaded_universe.velocities.size());
    ASSERT_EQ(universe.positions.size(), loaded_universe.positions.size());

    for(int i = 0; i < universe.num_bodies; i++){
        ASSERT_EQ(round_to(universe.weights[i], 0.000001), round_to(loaded_universe.weights[i], 0.000001));
        ASSERT_EQ(round_to(universe.forces[i][0], 0.000001), round_to(loaded_universe.forces[i][0], 0.000001));
        ASSERT_EQ(round_to(universe.forces[i][1], 0.000001), round_to(loaded_universe.forces[i][1], 0.000001));
        ASSERT_EQ(round_to(universe.velocities[i][0], 0.000001), round_to(loaded_universe.velocities[i][0], 0.000001));
        ASSERT_EQ(round_to(universe.velocities[i][1], 0.000001), round_to(loaded_universe.velocities[i][1], 0.000001));
        ASSERT_EQ(round_to(universe.positions[i][0], 0.000001), round_to(loaded_universe.positions[i][0], 0.000001));
        ASSERT_EQ(round_to(universe.positions[i][1], 0.000001), round_to(loaded_universe.positions[i][1], 0.000001));
    }
    std::remove("test_three_a_ppws24_D75C_universe.txt");
}