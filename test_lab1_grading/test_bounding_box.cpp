#include "test.h"

#include "structures/bounding_box.h"

#include <exception>
#include <iostream>

class BoundingBoxTest : public LabTest {};

TEST_F(BoundingBoxTest, test_two_c_ppws24_D75C){
    // constructors and fields x_min, x_max, y_min, y_max exist 
    auto bb1 = BoundingBox();
    bb1.x_min = 5.1;
    bb1.x_max = 17.3;
    bb1.y_min = -7.13;
    bb1.y_max = 141.1;
    ASSERT_EQ(bb1.x_min, 5.1);
    ASSERT_EQ(bb1.x_max, 17.3);
    ASSERT_EQ(bb1.y_min, -7.13);
    ASSERT_EQ(bb1.y_max, 141.1);

    auto bb2 = BoundingBox(bb1.x_min, bb1.x_max, bb1.y_min, bb1.y_max);
    ASSERT_EQ(bb1.x_min, bb2.x_min);
    ASSERT_EQ(bb1.x_max, bb2.x_max);
    ASSERT_EQ(bb1.y_min, bb2.y_min);
    ASSERT_EQ(bb1.y_max, bb2.y_max);

    // given methods are defined
    std::string s = bb1.get_string();
    ASSERT_TRUE(s.length() > 0);
    auto scaled_bb1 = bb1.get_scaled(4);
    ASSERT_TRUE(bb1.get_diagonal() < scaled_bb1.get_diagonal());
    
    auto bb3 = BoundingBox(5, 42, 0, 0);
    bb3.plotting_sanity_check();
    ASSERT_NE(bb3.y_min, 0);
    ASSERT_NE(bb3.y_max, 0);

    auto bb4 = BoundingBox(0, 0, 12, 124);
    bb4.plotting_sanity_check();
    ASSERT_NE(bb4.x_min, 0);
    ASSERT_NE(bb4.x_max, 0);
}

TEST_F(BoundingBoxTest, test_two_d_ppws24_D75C){
    auto bb = BoundingBox(-5, 5, -10, 10);
    // contains
    ASSERT_TRUE(bb.contains(Vector2d<double>(0, 0)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(-2.5, -5)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(-2.5, 5)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(2.5, -5)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(2.5, 5)));

    // contains borders
    ASSERT_TRUE(bb.contains(Vector2d<double>(-5, 0)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(5, 0)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(0, -10)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(0, 10)));

    // contains corners
    ASSERT_TRUE(bb.contains(Vector2d<double>(-5, -10)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(-5, 10)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(5, -10)));
    ASSERT_TRUE(bb.contains(Vector2d<double>(5, 10)));
    
    // not contained
    ASSERT_FALSE(bb.contains(Vector2d<double>(-10, -20)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(-10, 20)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(10, -20)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(10, 20)));

    // not contained in one dimension
    ASSERT_FALSE(bb.contains(Vector2d<double>(-5, -20)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(-5, 20)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(-10, -10)));
    ASSERT_FALSE(bb.contains(Vector2d<double>(10, -10)));

    // get_quadrant
    auto qd0 = bb.get_quadrant(0);
    auto qd1 = bb.get_quadrant(1);
    auto qd2 = bb.get_quadrant(2);
    auto qd3 = bb.get_quadrant(3);
    ASSERT_THROW(auto qd0 = bb.get_quadrant(4), std::exception);

    ASSERT_TRUE(qd0.contains(Vector2d<double>(-5, 10)));
    ASSERT_TRUE(qd0.contains(Vector2d<double>(-5, 0)));
    ASSERT_TRUE(qd0.contains(Vector2d<double>(0, 10)));
    ASSERT_TRUE(qd0.contains(Vector2d<double>(0, 0)));
    ASSERT_FALSE(qd0.contains(Vector2d<double>(3, 3)));
    ASSERT_FALSE(qd0.contains(Vector2d<double>(-3, -3)));
    ASSERT_FALSE(qd0.contains(Vector2d<double>(3, -3)));

    ASSERT_TRUE(qd1.contains(Vector2d<double>(0, 10)));
    ASSERT_TRUE(qd1.contains(Vector2d<double>(0, 0)));
    ASSERT_TRUE(qd1.contains(Vector2d<double>(5, 10)));
    ASSERT_TRUE(qd1.contains(Vector2d<double>(5, 0)));
    ASSERT_FALSE(qd1.contains(Vector2d<double>(-3, 3)));
    ASSERT_FALSE(qd1.contains(Vector2d<double>(-3, -3)));
    ASSERT_TRUE(qd2.contains(Vector2d<double>(-5, 0)));
    ASSERT_TRUE(qd2.contains(Vector2d<double>(0, -10)));
    ASSERT_TRUE(qd2.contains(Vector2d<double>(0, 0)));
    ASSERT_FALSE(qd2.contains(Vector2d<double>(-3, 3)));
    ASSERT_FALSE(qd2.contains(Vector2d<double>(3, 3)));
    ASSERT_FALSE(qd2.contains(Vector2d<double>(3, -3)));

    ASSERT_TRUE(qd3.contains(Vector2d<double>(5, -10)));
    ASSERT_TRUE(qd3.contains(Vector2d<double>(5, 0)));
    ASSERT_TRUE(qd3.contains(Vector2d<double>(0, -10)));
    ASSERT_TRUE(qd3.contains(Vector2d<double>(0, 0)));
    ASSERT_FALSE(qd3.contains(Vector2d<double>(-3, 3)));
    ASSERT_FALSE(qd3.contains(Vector2d<double>(3, 3)));
    ASSERT_FALSE(qd3.contains(Vector2d<double>(-3, -3)));
}