#include "test.h"

#include "physics/gravitation.h"
#include "physics/mechanics.h"
#include "structures/vector2d.h"

#include <exception>
#include <iostream>

class PhysicsTest : public LabTest {};

[[nodiscard]] static double reference_gravitational_force(double mass_1,  double mass_2, double distance){
    return 6.67430*1e-11 * ((mass_1 * mass_2)/(pow(distance, 2)));
}   

template <typename T>
[[nodiscard]] static Vector2d<T> reference_calculate_acceleration(Vector2d<T> applied_force, double mass){
    return applied_force / mass;
}

template <typename T> 
[[nodiscard]] static Vector2d<T> reference_calculate_velocity(Vector2d<T> base_velocity, Vector2d<T> acceleration, double time_in_seconds){
    return base_velocity + (acceleration * time_in_seconds);
}


TEST_F(PhysicsTest, test_four_a_ppws24_D75C){
    ASSERT_EQ(gravitational_constant, 6.67430*1e-11);
    
    double m1, m2, d;

    m1 = 0;
    m2 = 13371241.121;
    d = 15125151221.516;

    ASSERT_EQ(gravitational_force(m1, m2, d), reference_gravitational_force(m1, m2, d));
    m1 = 51675124.92141;
    ASSERT_EQ(gravitational_force(m1, m2, d), reference_gravitational_force(m1, m2, d));
    m2 = 0;
    ASSERT_EQ(gravitational_force(m1, m2, d), reference_gravitational_force(m1, m2, d));
    m2 = 6156212332.2151;
    d = 41512561251512455.2211;
    ASSERT_EQ(gravitational_force(m1, m2, d), reference_gravitational_force(m1, m2, d));    
}

TEST_F(PhysicsTest, test_four_b_ppws24_D75C){

    double m1, m2, d;
    m1 = 151522;
    m2 = 13371241.121;
    d = 15125151221.516;

    auto f1 = Vector2d<double>(0, 0);
    auto f2 = Vector2d<double>(-15152.12, -73532.321);
    auto f3 = Vector2d<double>(-15152.12, 73532.321);
    auto f4 = Vector2d<double>(15152.12, -73532.321);
    auto f5 = Vector2d<double>(15152.12, 73532.321);
    auto f6 = Vector2d<std::int32_t>(15152.12, 73532.321);

    // calculate_acceleration
    ASSERT_EQ(calculate_acceleration(f1, m1), reference_calculate_acceleration(f1, m1));
    ASSERT_EQ(calculate_acceleration(f1, m2), reference_calculate_acceleration(f1, m2));
    ASSERT_EQ(calculate_acceleration(f2, m1), reference_calculate_acceleration(f2, m1));
    ASSERT_EQ(calculate_acceleration(f2, m2), reference_calculate_acceleration(f2, m2));
    ASSERT_EQ(calculate_acceleration(f3, m1), reference_calculate_acceleration(f3, m1));
    ASSERT_EQ(calculate_acceleration(f3, m2), reference_calculate_acceleration(f3, m2));
    ASSERT_EQ(calculate_acceleration(f4, m1), reference_calculate_acceleration(f4, m1));
    ASSERT_EQ(calculate_acceleration(f4, m2), reference_calculate_acceleration(f4, m2));
    ASSERT_EQ(calculate_acceleration(f5, m1), reference_calculate_acceleration(f5, m1));
    ASSERT_EQ(calculate_acceleration(f5, m2), reference_calculate_acceleration(f5, m2));
    ASSERT_EQ(calculate_acceleration(f6, m1), reference_calculate_acceleration(f6, m1));
    ASSERT_EQ(calculate_acceleration(f6, m2), reference_calculate_acceleration(f6, m2));

    // calculate velocity

    auto v1 = Vector2d<double>(0, 0);
    auto v2 = Vector2d<double>(-5121.61256, -612782.2213);
    auto v3 = Vector2d<double>(-5121.61256, 612782.2213);
    auto v4 = Vector2d<double>(5121.61256, -612782.2213);
    auto v5 = Vector2d<double>(5121.61256, 612782.2213);
    auto v6 = Vector2d<std::int32_t>(5121.61256, 612782.2213);

    auto a1 = Vector2d<double>(0, 0);
    auto a2 = Vector2d<double>(-521551225.512, -15125151.374);
    auto a3 = Vector2d<double>(-521551225.512, 15125151.374);
    auto a4 = Vector2d<double>(521551225.512, -15125151.374);
    auto a5 = Vector2d<double>(521551225.512, 15125151.374);
    auto a6 = Vector2d<std::int32_t>(521225.512, 1512151.374);

    double t1 = 0;
    double t2 = 1415112;

    ASSERT_EQ(calculate_velocity(v1, a1, t1), reference_calculate_velocity(v1, a1, t1));
    ASSERT_EQ(calculate_velocity(v1, a1, t2), reference_calculate_velocity(v1, a1, t2));
    ASSERT_EQ(calculate_velocity(v1, a2, t1), reference_calculate_velocity(v1, a2, t1));
    ASSERT_EQ(calculate_velocity(v1, a2, t2), reference_calculate_velocity(v1, a2, t2));
    ASSERT_EQ(calculate_velocity(v1, a3, t1), reference_calculate_velocity(v1, a3, t1));
    ASSERT_EQ(calculate_velocity(v1, a3, t2), reference_calculate_velocity(v1, a3, t2));
    ASSERT_EQ(calculate_velocity(v1, a4, t1), reference_calculate_velocity(v1, a4, t1));
    ASSERT_EQ(calculate_velocity(v1, a4, t2), reference_calculate_velocity(v1, a4, t2));
    ASSERT_EQ(calculate_velocity(v1, a5, t1), reference_calculate_velocity(v1, a5, t1));
    ASSERT_EQ(calculate_velocity(v1, a5, t2), reference_calculate_velocity(v1, a5, t2));

    ASSERT_EQ(calculate_velocity(v2, a1, t1), reference_calculate_velocity(v2, a1, t1));
    ASSERT_EQ(calculate_velocity(v2, a1, t2), reference_calculate_velocity(v2, a1, t2));
    ASSERT_EQ(calculate_velocity(v2, a2, t1), reference_calculate_velocity(v2, a2, t1));
    ASSERT_EQ(calculate_velocity(v2, a2, t2), reference_calculate_velocity(v2, a2, t2));
    ASSERT_EQ(calculate_velocity(v2, a3, t1), reference_calculate_velocity(v2, a3, t1));
    ASSERT_EQ(calculate_velocity(v2, a3, t2), reference_calculate_velocity(v2, a3, t2));
    ASSERT_EQ(calculate_velocity(v2, a4, t1), reference_calculate_velocity(v2, a4, t1));
    ASSERT_EQ(calculate_velocity(v2, a4, t2), reference_calculate_velocity(v2, a4, t2));
    ASSERT_EQ(calculate_velocity(v2, a5, t1), reference_calculate_velocity(v2, a5, t1));
    ASSERT_EQ(calculate_velocity(v2, a5, t2), reference_calculate_velocity(v2, a5, t2));

    ASSERT_EQ(calculate_velocity(v3, a1, t1), reference_calculate_velocity(v3, a1, t1));
    ASSERT_EQ(calculate_velocity(v3, a1, t2), reference_calculate_velocity(v3, a1, t2));
    ASSERT_EQ(calculate_velocity(v3, a2, t1), reference_calculate_velocity(v3, a2, t1));
    ASSERT_EQ(calculate_velocity(v3, a2, t2), reference_calculate_velocity(v3, a2, t2));
    ASSERT_EQ(calculate_velocity(v3, a3, t1), reference_calculate_velocity(v3, a3, t1));
    ASSERT_EQ(calculate_velocity(v3, a3, t2), reference_calculate_velocity(v3, a3, t2));
    ASSERT_EQ(calculate_velocity(v3, a4, t1), reference_calculate_velocity(v3, a4, t1));
    ASSERT_EQ(calculate_velocity(v3, a4, t2), reference_calculate_velocity(v3, a4, t2));
    ASSERT_EQ(calculate_velocity(v3, a5, t1), reference_calculate_velocity(v3, a5, t1));
    ASSERT_EQ(calculate_velocity(v3, a5, t2), reference_calculate_velocity(v3, a5, t2));

    ASSERT_EQ(calculate_velocity(v4, a1, t1), reference_calculate_velocity(v4, a1, t1));
    ASSERT_EQ(calculate_velocity(v4, a1, t2), reference_calculate_velocity(v4, a1, t2));
    ASSERT_EQ(calculate_velocity(v4, a2, t1), reference_calculate_velocity(v4, a2, t1));
    ASSERT_EQ(calculate_velocity(v4, a2, t2), reference_calculate_velocity(v4, a2, t2));
    ASSERT_EQ(calculate_velocity(v4, a3, t1), reference_calculate_velocity(v4, a3, t1));
    ASSERT_EQ(calculate_velocity(v4, a3, t2), reference_calculate_velocity(v4, a3, t2));
    ASSERT_EQ(calculate_velocity(v4, a4, t1), reference_calculate_velocity(v4, a4, t1));
    ASSERT_EQ(calculate_velocity(v4, a4, t2), reference_calculate_velocity(v4, a4, t2));
    ASSERT_EQ(calculate_velocity(v4, a5, t1), reference_calculate_velocity(v4, a5, t1));
    ASSERT_EQ(calculate_velocity(v4, a5, t2), reference_calculate_velocity(v4, a5, t2));

    ASSERT_EQ(calculate_velocity(v5, a1, t1), reference_calculate_velocity(v5, a1, t1));
    ASSERT_EQ(calculate_velocity(v5, a1, t2), reference_calculate_velocity(v5, a1, t2));
    ASSERT_EQ(calculate_velocity(v5, a2, t1), reference_calculate_velocity(v5, a2, t1));
    ASSERT_EQ(calculate_velocity(v5, a2, t2), reference_calculate_velocity(v5, a2, t2));
    ASSERT_EQ(calculate_velocity(v5, a3, t1), reference_calculate_velocity(v5, a3, t1));
    ASSERT_EQ(calculate_velocity(v5, a3, t2), reference_calculate_velocity(v5, a3, t2));
    ASSERT_EQ(calculate_velocity(v5, a4, t1), reference_calculate_velocity(v5, a4, t1));
    ASSERT_EQ(calculate_velocity(v5, a4, t2), reference_calculate_velocity(v5, a4, t2));
    ASSERT_EQ(calculate_velocity(v5, a5, t1), reference_calculate_velocity(v5, a5, t1));
    ASSERT_EQ(calculate_velocity(v5, a5, t2), reference_calculate_velocity(v5, a5, t2));

    ASSERT_EQ(calculate_velocity(v6, a6, t1), reference_calculate_velocity(v6, a6, t1));
    ASSERT_EQ(calculate_velocity(v6, a6, t2), reference_calculate_velocity(v6, a6, t2));

}