#include "test.h"

#include "structures/vector2d.h"

#include <exception>
#include <iostream>

class Vector2dTest : public LabTest {};

TEST_F(Vector2dTest, test_two_a_ppws24_D75C){
    // constructors
    auto vector_1 = Vector2d<double>();
    auto vector_2 = Vector2d<double>((double) 42.03, (double) 1337.03);
    // operator []
    ASSERT_EQ(vector_1[0], 0.0);
    ASSERT_EQ(vector_1[1], 0.0);
    ASSERT_EQ(vector_2[0], 42.03);
    ASSERT_EQ(vector_2[1], 1337.03);
    ASSERT_THROW(vector_2[3], std::exception);
    ASSERT_THROW(vector_2[-1], std::exception);
    
    // method set
    auto vector_3 = Vector2d<double>();
    vector_3.set(651.5, 125.21);
    ASSERT_EQ(vector_3[0], 651.5);
    ASSERT_EQ(vector_3[1], 125.21);

    // template type
    auto vector_4 = Vector2d<double>(42.02, 151.63);
    ASSERT_EQ(vector_4[0], 42.02);
    ASSERT_EQ(vector_4[1], 151.63);
    auto vector_5 = Vector2d<std::int32_t>(42.02, 151.63);
    ASSERT_EQ(vector_5[0], 42);
    ASSERT_EQ(vector_5[1], 151);
    ASSERT_NE(vector_5[0], 42.02);
    ASSERT_NE(vector_5[1], 151.63);    
}


TEST_F(Vector2dTest, test_two_b_ppws24_D75C){
    // ==
    auto vector1 = Vector2d<double>();
    auto vector2 = Vector2d<double>();
    auto vector3 = Vector2d<double>();
    vector1.set(512.37, 71.562);
    vector2.set(512.37, 71.562);
    ASSERT_TRUE(vector1 == vector2);
    ASSERT_FALSE(vector1 == vector3);
    vector3.set(61.783, 71);
    ASSERT_FALSE(vector1 == vector3);
    vector3.set(512.37, 71.562);
    ASSERT_TRUE(vector1 == vector3);

    // +
    vector1.set(6.0, 2.0);
    vector2.set(7.2, 91.215);
    
    ASSERT_DOUBLE_EQ(Vector2d<double>(13.2, 93.215)[0], (vector1 + vector2)[0]);
    ASSERT_DOUBLE_EQ(Vector2d<double>(13.2, 93.215)[1], (vector1 + vector2)[1]);
    // -
    ASSERT_DOUBLE_EQ(Vector2d<double>(-1.2, -89.215)[0], (vector1 - vector2)[0]);
    ASSERT_DOUBLE_EQ(Vector2d<double>(-1.2, -89.215)[1], (vector1 - vector2)[1]);
    // *
    ASSERT_DOUBLE_EQ(Vector2d<double>(50.4, 638.505)[0], (vector2 * 7.0)[0]);
    ASSERT_DOUBLE_EQ(Vector2d<double>(50.4, 638.505)[1], (vector2 * 7.0)[1]);
    // /
    ASSERT_DOUBLE_EQ(Vector2d<double>(2.4, 30.405)[0], (vector2 / 3.0)[0]);
    ASSERT_DOUBLE_EQ(Vector2d<double>(2.4, 30.405)[1], (vector2 / 3.0)[1]);
    // const
    ASSERT_DOUBLE_EQ(vector1[0], 6.0);
    ASSERT_DOUBLE_EQ(vector1[1], 2.0);
    ASSERT_DOUBLE_EQ(vector2[0], 7.2);
    ASSERT_DOUBLE_EQ(vector2[1], 91.215);
    
}