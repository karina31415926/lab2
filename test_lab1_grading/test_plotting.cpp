#include "test.h"

#include "structures/universe.h"
#include "input_generator/input_generator.h"
#include "plotting/plotter.h"

#include <exception>
#include <iostream>
#include <filesystem>

class PlottingTest : public LabTest {};

TEST_F(PlottingTest, test_three_b_ppws24_D75C){
    // test mark pixel
    BoundingBox bb(-10, 10, -10, 10);
    auto tmp_path = std::filesystem::path{"test_three_b_ppws24_D75C_plot"};
   
    Plotter plotter1(bb, tmp_path, 400, 400);
    plotter1.mark_pixel(0, 0, 255, 255, 255);
    plotter1.mark_pixel(0, 399, 255, 255, 255);
    plotter1.mark_pixel(399, 0, 255, 255, 255);
    plotter1.mark_pixel(399, 399, 255, 255, 255);
    plotter1.mark_pixel(199, 199, 255, 255, 255);

    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter1.get_pixel(x, y);
            if((x == 0 && y == 0) || (x == 0 && y == 399) || (x == 399 && y == 0) || (x == 399 && y == 399) || (x == 199 && y == 199) ){
                ASSERT_EQ(px.get_blue_channel(), 255);
                ASSERT_EQ(px.get_green_channel(), 255);
                ASSERT_EQ(px.get_red_channel(), 255);
            }
            else{
                ASSERT_EQ(px.get_blue_channel(), 0);
                ASSERT_EQ(px.get_green_channel(), 0);
                ASSERT_EQ(px.get_red_channel(), 0);
            }
        }
    }


    ASSERT_THROW(plotter1.mark_pixel(0, 400, 255, 255, 255), std::exception);
    ASSERT_THROW(plotter1.mark_pixel(0, 500, 255, 255, 255), std::exception);
    ASSERT_THROW(plotter1.mark_pixel(-5, 233, 255, 255, 255), std::exception);
    ASSERT_THROW(plotter1.mark_pixel(500, 500, 255, 255, 255), std::exception);


    // test mark position
    Plotter plotter(bb, tmp_path, 400, 400);
    plotter.mark_position(Vector2d<double>(-10, -10), 255, 255, 255);
    plotter.mark_position(Vector2d<double>(-10, 10), 255, 255, 255);
    plotter.mark_position(Vector2d<double>(10, -10), 255, 255, 255);
    plotter.mark_position(Vector2d<double>(10, 10), 255, 255, 255);
    plotter.mark_position(Vector2d<double>(0, 0), 255, 255, 255);

    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter.get_pixel(x, y);
            if((x == 0 && y == 0) || (x == 0 && y == 399) || (x == 399 && y == 0) || (x == 399 && y == 399) || (x == 199 && y == 199) ){
                ASSERT_EQ(px.get_blue_channel(), 255);
                ASSERT_EQ(px.get_green_channel(), 255);
                ASSERT_EQ(px.get_red_channel(), 255);
            }
            else{
                ASSERT_EQ(px.get_blue_channel(), 0);
                ASSERT_EQ(px.get_green_channel(), 0);
                ASSERT_EQ(px.get_red_channel(), 0);
            }
        }
    }

    // check no modification when plotting out of bounds
    Plotter plotter3(bb, tmp_path, 400, 400);
    plotter3.mark_position(Vector2d<double>(-20, 0), 255, 255, 255);
    plotter3.mark_position(Vector2d<double>(0, -20), 255, 255, 255);
    plotter3.mark_position(Vector2d<double>(-20, -20), 255, 255, 255);
    plotter3.mark_position(Vector2d<double>(-20, 20), 255, 255, 255);
    plotter3.mark_position(Vector2d<double>(20, -20), 255, 255, 255);
    plotter3.mark_position(Vector2d<double>(20, 20), 255, 255, 255);

    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter3.get_pixel(x, y);
            ASSERT_EQ(px.get_blue_channel(), 0);
            ASSERT_EQ(px.get_green_channel(), 0);
            ASSERT_EQ(px.get_red_channel(), 0);
        }
    }
}

TEST_F(PlottingTest, test_three_c_ppws24_D75C){
    BoundingBox bb(-10, 10, -10, 10);
    auto tmp_path = std::filesystem::path{"test_three_c_ppws24_D75C_plot"};

    // test highlight position
    Plotter plotter1(bb, tmp_path, 400, 400);
    plotter1.highlight_position(Vector2d<double>(-5, 5), 255, 53, 126);
    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter1.get_pixel(x, y);
            if(x == 99 || y == 299){                        
                ASSERT_EQ(px.get_red_channel(), 255);   
                ASSERT_EQ(px.get_green_channel(), 53);
                ASSERT_EQ(px.get_blue_channel(), 126);
            }
            else{
                ASSERT_EQ(px.get_blue_channel(), 0);
                ASSERT_EQ(px.get_green_channel(), 0);
                ASSERT_EQ(px.get_red_channel(), 0);
            }
        }
    }

    Plotter plotter2(bb, tmp_path, 400, 400);
    plotter2.highlight_position(Vector2d<double>(10, 10), 51, 215, 93);
    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter2.get_pixel(x, y);
            if(x == 399 || y == 399){
                ASSERT_EQ(px.get_red_channel(), 51);   
                ASSERT_EQ(px.get_green_channel(), 215);
                ASSERT_EQ(px.get_blue_channel(), 93);
            }
            else{
                ASSERT_EQ(px.get_blue_channel(), 0);
                ASSERT_EQ(px.get_green_channel(), 0);
                ASSERT_EQ(px.get_red_channel(), 0);
            }
        }
    }

    Plotter plotter3(bb, tmp_path, 400, 400);
    plotter3.highlight_position(Vector2d<double>(-15, -15), 255, 0, 0 );
    plotter3.highlight_position(Vector2d<double>(-15, 15), 255, 0, 0 );
    plotter3.highlight_position(Vector2d<double>(15, -15), 255, 0, 0 );
    plotter3.highlight_position(Vector2d<double>(15, 15), 255, 0, 0 );
    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter3.get_pixel(x, y);
            ASSERT_EQ(px.get_blue_channel(), 0);
            ASSERT_EQ(px.get_green_channel(), 0);
            ASSERT_EQ(px.get_red_channel(), 0);
        }
    }


    // test add_bodies_to_image
    Plotter plotter4(bb, tmp_path, 400, 400);
    Universe universe4;
    universe4.num_bodies = 8;
    
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.forces.push_back(Vector2d<double>(0, 0));
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.weights.push_back(0);
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));
    universe4.velocities.push_back(Vector2d<double>(0, 0));

    universe4.positions.push_back(Vector2d<double>(5, 5));
    universe4.positions.push_back(Vector2d<double>(5, 5));
    universe4.positions.push_back(Vector2d<double>(-10, 10));
    universe4.positions.push_back(Vector2d<double>(0, 0));
    universe4.positions.push_back(Vector2d<double>(-5, 10));
    universe4.positions.push_back(Vector2d<double>(-5, -5));
    universe4.positions.push_back(Vector2d<double>(10, 5));
    universe4.positions.push_back(Vector2d<double>(10, -5));

    universe4.positions.push_back(Vector2d<double>(7, -25));
    universe4.positions.push_back(Vector2d<double>(45, -2));

    plotter4.add_bodies_to_image(universe4);

    // 299,299
    //0,399
    // 199,199
    //99,399
    //99,99
    //399,299
    //399,99
    for(int x = 0; x < 400; x++){
        for(int y = 0; y < 400; y++){
            auto px = plotter4.get_pixel(x, y);
            if((x == 299 && y == 299) || (x == 0 && y == 399) || (x == 199 && y == 199) || (x == 99 && y == 399) || (x == 99 && y == 99) || (x == 399 && y == 299) || (x == 399 && y == 99)){
                ASSERT_EQ(px.get_blue_channel(), 255);
                ASSERT_EQ(px.get_green_channel(), 255);
                ASSERT_EQ(px.get_red_channel(), 255);   
            }
            else{
                ASSERT_EQ(px.get_blue_channel(), 0);
                ASSERT_EQ(px.get_green_channel(), 0);
                ASSERT_EQ(px.get_red_channel(), 0);
            }
        }
    }

}