#include <util/simulated_clock.hpp>
#include <iostream>
#include <gtest/gtest.h>

TEST(SimulatedClockTest, StartTime) {
  EXPECT_EQ(util::simulated_clock::now(), util::simulated_clock::time_point {});
}
