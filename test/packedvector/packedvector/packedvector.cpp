#include <gtest/gtest.h>
#include <packedvector.hpp>
using namespace packedvector;

TEST(PackedVectorTest, Size) {
  PackedVector<uint32_t, 10> pv;
  ASSERT_EQ(pv.size(), 0);

  pv.push_back(1023);
  pv.push_back(512);
  pv.push_back(256);

  ASSERT_EQ(pv.size(), 3);

  pv.clear();
  ASSERT_EQ(pv.size(), 0);

  pv.push_back(1);
  EXPECT_EQ(pv.size(), 1);
}

TEST(PackedVectorTest, Capacity) {
  PackedVector<uint32_t, 5> pv;

  EXPECT_EQ(pv.capacity(), 0);

  pv.push_back(1);
  EXPECT_EQ(pv.capacity(), 1);

  pv.push_back(2);
  EXPECT_EQ(pv.capacity(), 2);
  pv.push_back(3);
  EXPECT_EQ(pv.capacity(), 4);
  pv.push_back(4);
  pv.push_back(5);
  EXPECT_EQ(pv.capacity(), 8);
}

TEST(PackedVectorTest, BitPacking) {
  PackedVector<uint32_t, 3> pv; // 3-bit wide elements

  pv.push_back(0);
  pv.push_back(1);
  pv.push_back(2);
  pv.push_back(3);
  pv.push_back(4);
  pv.push_back(5);
  pv.push_back(6);
  pv.push_back(7);

  ASSERT_EQ(pv.size(), 8);
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(pv[i], i);
  }
}

TEST(PackedVectorTest, CopyAssignment) {
  PackedVector<uint32_t, 10> pv1 = {1023, 512, 256};
  PackedVector<uint32_t, 10> pv2;
  pv2 = pv1;

  ASSERT_EQ(pv2.size(), 3);
  EXPECT_EQ(pv2[0], 1023);
  EXPECT_EQ(pv2[1], 512);
  EXPECT_EQ(pv2[2], 256);
}

TEST(PackedVectorTest, MoveAssignment) {
  PackedVector<uint32_t, 10> pv1 = {1023, 512, 256};
  PackedVector<uint32_t, 10> pv2;
  pv2 = std::move(pv1);

  ASSERT_EQ(pv2.size(), 3);
  EXPECT_EQ(pv2[0], 1023);
  EXPECT_EQ(pv2[1], 512);
  EXPECT_EQ(pv2[2], 256);

  ASSERT_EQ(pv1.size(), 0);
  ASSERT_EQ(pv1.capacity(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
