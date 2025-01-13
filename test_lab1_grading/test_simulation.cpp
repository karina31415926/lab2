#include "test.h"

#include "simulation/naive_sequential_simulation.h"
#include "simulation/constants.h"

#include "utilities/import.hpp"
#include "utilities/export.hpp"

#include <exception>
#include <iostream>

#include "utilities.h"

class SimulationTest : public LabTest {};

TEST_F(SimulationTest, test_five_a_one_ppws24_D75C){
    // forces grundsätzlich verändert
    Universe uni;

    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe.txt"};
    load_universe(tmp, uni);
    // all forces initially 0,0

    std::int32_t size_pre = uni.forces.size();

    NaiveSequentialSimulation::calculate_forces(uni);

    ASSERT_EQ(uni.forces.size(), size_pre);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_TRUE((uni.forces[i][0] != 0.0) || uni.forces[i][1] != 0.0);
    }
}

TEST_F(SimulationTest, test_five_ppws24_D75C){
    // Consistency of other values preserved
    Universe original_uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe.txt"};
    load_universe(tmp, original_uni);

    Universe uni;
    load_universe(tmp, uni);
    NaiveSequentialSimulation::calculate_forces(uni);

    ASSERT_EQ(uni.num_bodies, original_uni.num_bodies);
    ASSERT_EQ(uni.current_simulation_epoch, original_uni.current_simulation_epoch);
    ASSERT_EQ(uni.forces.size(), original_uni.forces.size());
    ASSERT_EQ(uni.weights.size(), original_uni.weights.size());
    ASSERT_EQ(uni.velocities.size(), original_uni.velocities.size());
    ASSERT_EQ(uni.positions.size(), original_uni.positions.size());
    
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(original_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(original_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(original_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(original_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(original_uni.positions[i][1], 0.000001));
    }
}

TEST_F(SimulationTest, test_five_a_three_ppws24_D75C){
    // forces korrekt berechnet
    Universe original_uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe.txt"};
    load_universe(tmp, original_uni);

    Universe uni;
    load_universe(tmp, uni);
    NaiveSequentialSimulation::calculate_forces(uni);

    // load reference
    Universe reference_uni;
    auto tmp2 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_forces.txt"};
    load_universe(tmp2, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    ASSERT_EQ(uni.weights.size(), reference_uni.weights.size());
    ASSERT_EQ(uni.forces.size(), reference_uni.forces.size());
    ASSERT_EQ(uni.velocities.size(), reference_uni.velocities.size());
    ASSERT_EQ(uni.positions.size(), reference_uni.positions.size());

    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(reference_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(reference_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(reference_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
}

TEST_F(SimulationTest, test_five_b_one_ppws24_D75C){
    // check defined epoch_in_seconds
    ASSERT_EQ(epoch_in_seconds, 2.628e+6);
    
    // check if velocities are updated
    Universe original_uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_forces.txt"};
    load_universe(tmp, original_uni);

    Universe uni;
    load_universe(tmp, uni);
    NaiveSequentialSimulation::calculate_velocities(uni);

    ASSERT_EQ(original_uni.num_bodies, uni.num_bodies);
    for(int i = 0; i < original_uni.num_bodies; i++){
        ASSERT_NE(round_to(uni.velocities[i][0], 0.000001), round_to(original_uni.velocities[i][0], 0.000001));
        ASSERT_NE(round_to(uni.velocities[i][1], 0.000001), round_to(original_uni.velocities[i][1], 0.000001));
    }

    // check for correct velocities if no forces are applied
    Universe uni2;
    load_universe(tmp, uni2);
    for(int i = 0; i < uni2.num_bodies; i++){
        uni2.forces[i].set(0, 0);
    }
    NaiveSequentialSimulation::calculate_velocities(uni2);
    for(int i = 0; i < uni2.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni2.velocities[i][0], 0.000001), round_to(original_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni2.velocities[i][1], 0.000001), round_to(original_uni.velocities[i][1], 0.000001));
    }

    // check for consistency of other data
    ASSERT_EQ(uni.num_bodies, original_uni.num_bodies);
    ASSERT_EQ(uni.weights.size(), original_uni.weights.size());
    ASSERT_EQ(uni.forces.size(), original_uni.forces.size());
    ASSERT_EQ(uni.velocities.size(), original_uni.velocities.size());
    ASSERT_EQ(uni.positions.size(), original_uni.positions.size());

    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(original_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(original_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(original_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(original_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(original_uni.positions[i][1], 0.000001));
    }
}

TEST_F(SimulationTest, test_five_b_two_ppws24_D75C){
    // check for correct velocities if forces are applied
    Universe uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_forces.txt"};
    load_universe(tmp, uni);

    NaiveSequentialSimulation::calculate_velocities(uni);

    Universe reference_uni;
    auto tmp2 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_velocities.txt"};
    load_universe(tmp2, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
    }
}

TEST_F(SimulationTest, test_five_c_one_ppws24_D75C){
    // check if positions are updated
    Universe original_uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_velocities.txt"};
    load_universe(tmp, original_uni);

    Universe uni;
    load_universe(tmp, uni);
    NaiveSequentialSimulation::calculate_positions(uni);

    ASSERT_EQ(original_uni.num_bodies, uni.num_bodies);
    for(int i = 0; i < original_uni.num_bodies; i++){
        ASSERT_NE(round_to(uni.positions[i][0], 0.000001), round_to(original_uni.positions[i][0], 0.000001));
        ASSERT_NE(round_to(uni.positions[i][1], 0.000001), round_to(original_uni.positions[i][1], 0.000001));
    }

    // check if no update occurs if velocities are 0, 0
    Universe uni2;
    load_universe(tmp, uni2);
    for(int i = 0; i < uni2.num_bodies; i++){
        uni2.velocities[i].set(0, 0);
    }
    NaiveSequentialSimulation::calculate_positions(uni2);
    for(int i = 0; i < uni2.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni2.positions[i][0], 0.000001), round_to(original_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni2.positions[i][1], 0.000001), round_to(original_uni.positions[i][1], 0.000001));
    }

    // check for consistency of data
    ASSERT_EQ(uni.num_bodies, original_uni.num_bodies);
    ASSERT_EQ(uni.weights.size(), original_uni.weights.size());
    ASSERT_EQ(uni.forces.size(), original_uni.forces.size());
    ASSERT_EQ(uni.velocities.size(), original_uni.velocities.size());
    ASSERT_EQ(uni.positions.size(), original_uni.positions.size());

    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(original_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(original_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(original_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(original_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(original_uni.velocities[i][1], 0.000001));
    }
}

TEST_F(SimulationTest, test_five_c_two_ppws24_D75C){
    // check for correct positions if velocities are not 0
    Universe uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_velocities.txt"};
    load_universe(tmp, uni);

    NaiveSequentialSimulation::calculate_positions(uni);

    Universe reference_uni;
    auto tmp2 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_calculate_positions.txt"};
    load_universe(tmp2, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
}

std::int32_t count_files_in_directory(std::filesystem::path directory){
    std::int32_t count = 0;
    for(auto entry : std::filesystem::directory_iterator(directory)){
        if(entry.is_regular_file()){
            count++;
        }
    }
    return count;
}

TEST_F(SimulationTest, test_five_d_ppws24_D75C){
    Universe uni; 
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe.txt"};
    load_universe(tmp, uni);

    BoundingBox bb(-939948456012620.000000,942861260429425.875000, -914208968239933.750000,936301039007632.625000);

    auto tmp_path = std::filesystem::path{"test_five_d_ppws24_D75C_plot"};
    //  cleanup potentially remaining results in directory
    std::filesystem::remove_all(tmp_path);
    std::filesystem::create_directories(tmp_path);
    Plotter plotter(bb, tmp_path, 400, 400);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni, true, 1);

    // check if epoch counter is incremented 
    ASSERT_EQ(uni.current_simulation_epoch, 1);
    // check if epoch simulation results are correct
    Universe reference_uni;
    auto tmp2 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_one_epoch.txt"};
    load_universe(tmp2, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(reference_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(reference_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(reference_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
    // One plot should have been created
    ASSERT_EQ(count_files_in_directory(tmp_path), 1);
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);

    // check if plotting can be disabled fully
    std::filesystem::create_directories(tmp_path);
    Plotter plotter1(bb, tmp_path, 400, 400);
    Universe uni1;
    load_universe(tmp, uni1);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, false, 1);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, false, 1);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, false, 1);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, false, 1);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, false, 1);
    ASSERT_EQ(count_files_in_directory(tmp_path), 0);
    std::filesystem::remove_all(tmp_path);


    // check if plotting every n epochs works
    std::filesystem::create_directories(tmp_path);
    Plotter plotter2(bb, tmp_path, 400, 400);
    Universe uni2;
    load_universe(tmp, uni2);
    // important: account for potential differences in implementation: plot before / after increasing epoch counter
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3); // plot
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3); // plot
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3); // plot
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3); 
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3); // plot
    NaiveSequentialSimulation::simulate_epoch(plotter, uni1, true, 3);
    ASSERT_EQ(count_files_in_directory(tmp_path), 4);
    std::filesystem::remove_all(tmp_path);
}

TEST_F(SimulationTest, test_five_e_ppws24_D75C){
    Universe uni;
    auto tmp = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe.txt"};
    load_universe(tmp, uni);

    BoundingBox bb(-939948456012620.000000,942861260429425.875000, -914208968239933.750000,936301039007632.625000);

    auto tmp_path = std::filesystem::path{"test_five_e_ppws24_D75C_plot"};
    //  cleanup potentially remaining results in directory
    std::filesystem::remove_all(tmp_path);
    std::filesystem::create_directories(tmp_path);
    Plotter plotter(bb, tmp_path, 400, 400);

    // complete simulation of multiple epochs
    NaiveSequentialSimulation::simulate_epochs(plotter, uni, 1, true, 1);
    // check if epoch counter is incremented 
    ASSERT_EQ(uni.current_simulation_epoch, 1);
    // check if epoch simulation results are correct
    Universe reference_uni;
    auto tmp2 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_one_epoch.txt"};
    load_universe(tmp2, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(reference_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(reference_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(reference_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
    // One plot should have been created per epoch
    ASSERT_EQ(count_files_in_directory(tmp_path), 1);
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);


    // check if amount of simulated epochs is configurable
    uni = Universe();
    load_universe(tmp, uni);  // reset uni
    std::filesystem::create_directories(tmp_path);
    Plotter plotter1(bb, tmp_path, 400, 400);

    // complete simulation of multiple epochs
    NaiveSequentialSimulation::simulate_epochs(plotter1, uni, 50, true, 1);
    // check if epoch counter is incremented 
    ASSERT_EQ(uni.current_simulation_epoch, 50);
    // check if epoch simulation results are correct
    auto tmp3 = std::filesystem::path{"../test_input_grading/test_five_ppws24_D75C_universe_after_50_epochs.txt"};
    load_universe(tmp3, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(reference_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(reference_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(reference_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
    // One plot should have been created per epoch
    ASSERT_EQ(count_files_in_directory(tmp_path), 50);
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);



    // check if 0 simulated epochs have no influence
    uni = Universe();
    load_universe(tmp, uni);  // reset uni
    std::filesystem::create_directories(tmp_path);
    Plotter plotter2(bb, tmp_path, 400, 400);

    // complete simulation of multiple epochs
    NaiveSequentialSimulation::simulate_epochs(plotter2, uni, 0, true, 1);
    // check if epoch counter is incremented 
    ASSERT_EQ(uni.current_simulation_epoch, 0);
    // check if epoch simulation results are correct
    load_universe(tmp, reference_uni);

    ASSERT_EQ(uni.num_bodies, reference_uni.num_bodies);
    for(int i = 0; i < uni.num_bodies; i++){
        ASSERT_FLOAT_EQ(round_to(uni.weights[i], 0.000001), round_to(reference_uni.weights[i], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][0], 0.000001), round_to(reference_uni.forces[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.forces[i][1], 0.000001), round_to(reference_uni.forces[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][0], 0.000001), round_to(reference_uni.velocities[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.velocities[i][1], 0.000001), round_to(reference_uni.velocities[i][1], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][0], 0.000001), round_to(reference_uni.positions[i][0], 0.000001));
        ASSERT_FLOAT_EQ(round_to(uni.positions[i][1], 0.000001), round_to(reference_uni.positions[i][1], 0.000001));
    }
    // One plot should have been created per epoch
    ASSERT_TRUE((count_files_in_directory(tmp_path) == 0) || (count_files_in_directory(tmp_path) == 1));
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);


    // check if plotting can be disabled
    uni = Universe();
    load_universe(tmp, uni);  // reset uni
    std::filesystem::create_directories(tmp_path);
    Plotter plotter3(bb, tmp_path, 400, 400);

    // complete simulation of multiple epochs
    NaiveSequentialSimulation::simulate_epochs(plotter3, uni, 50, false, 1);
    // No plots should have been created
    ASSERT_EQ(count_files_in_directory(tmp_path), 0);
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);

    // check if plotting every n epochs works
    uni = Universe();
    load_universe(tmp, uni);  // reset uni
    std::filesystem::create_directories(tmp_path);
    Plotter plotter4(bb, tmp_path, 400, 400);

    // complete simulation of multiple epochs
    NaiveSequentialSimulation::simulate_epochs(plotter4, uni, 50, true, 6);
    ASSERT_EQ(uni.current_simulation_epoch, 50);
    // No plots should have been created
    // important: account for potential differences in implementation: plot before / after increasing epoch counter
    ASSERT_TRUE((count_files_in_directory(tmp_path) == 9) || (count_files_in_directory(tmp_path) == 8));
    //  cleanup created directory
    std::filesystem::remove_all(tmp_path);
}