#include "test.h"

#include "structures/universe.h"
#include "input_generator/input_generator.h"

#include <exception>
#include <iostream>

class UniverseTest : public LabTest {};

TEST_F(UniverseTest, test_two_e_ppws24_D75C){
    // fields
    auto universe = Universe();
    ASSERT_EQ(universe.current_simulation_epoch, 0);
    ASSERT_EQ(universe.num_bodies, 0);
    ASSERT_EQ(universe.weights.size(), 0);
    ASSERT_EQ(universe.forces.size(), 0);
    ASSERT_EQ(universe.velocities.size(), 0);
    ASSERT_EQ(universe.positions.size(), 0);

    // check data types
    ASSERT_NO_THROW(universe.current_simulation_epoch++);
    ASSERT_NO_THROW(universe.num_bodies++);
    ASSERT_NO_THROW(universe.weights.push_back((double) 42.0));
    ASSERT_NO_THROW(universe.forces.push_back(Vector2d<double>(123.3, 151.63)));
    ASSERT_NO_THROW(universe.velocities.push_back(Vector2d<double>(623.151, -151.21)));
    ASSERT_NO_THROW(universe.positions.push_back(Vector2d<double>(56151.2, 1512.215)));
}

TEST_F(UniverseTest, test_two_f_ppws24_D75C){
    auto universe = Universe();
    InputGenerator::create_random_universe(1000, universe);
    
    // calculate reference bounding box
    double ref_x_min = INT64_MAX;
    double ref_x_max = INT64_MIN;
    double ref_y_min = INT64_MAX;
    double ref_y_max = INT64_MIN;
    for(int i = 0; i < 1000; i++){
        double pos_x, pos_y;
        pos_x = universe.positions[i][0];
        pos_y = universe.positions[i][1];

        if(pos_x > ref_x_max){
            ref_x_max = pos_x;
        }
        if(pos_x < ref_x_min){
            ref_x_min = pos_x;
        }
        if(pos_y > ref_y_max){
            ref_y_max = pos_y;
        }
        if(pos_y < ref_y_min){
            ref_y_min = pos_y;
        }
    }

    // get BoundingBox
    auto bb = universe.get_bounding_box();

    ASSERT_EQ(bb.x_min, ref_x_min);
    ASSERT_EQ(bb.x_max, ref_x_max);
    ASSERT_EQ(bb.y_min, ref_y_min);
    ASSERT_EQ(bb.y_max, ref_y_max);
}