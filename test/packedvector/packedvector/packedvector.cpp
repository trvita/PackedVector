#include <gtest/gtest.h>
#include <packedvector.hpp>
using namespace packedvector;

TEST(packedvectorMethods, packedvector) { EXPECT_EQ(1, 1); }
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
