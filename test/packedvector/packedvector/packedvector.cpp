#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>
#include <malloc.h>
#include <packedvector.hpp>
#include <vector>
using namespace packedvector;

template <typename T> size_t memVector(const std::vector<T> &vec) {
  return sizeof(vec) + vec.capacity() * sizeof(T);
}

template <typename T, size_t N>
size_t memPackedVector(const PackedVector<T, N> &packedVec) {
  return sizeof(packedVec) + malloc_usable_size(packedVec.getData().get());
}

TEST(PackedVectorTest, StdVectorComparison) {
  const size_t numElements = 1000;
  PackedVector<uint32_t, 10>
      packedVec; // потому что в 10 бит помещаются значения до 1024
  std::vector<uint32_t> Vec;
  for (size_t i = 0; i < numElements; ++i) {
    Vec.push_back(i);
    packedVec.push_back(i);
  }
  for (size_t i = 0; i < numElements; ++i) {
    EXPECT_EQ(packedVec[i], Vec[i]);
  }
  EXPECT_LT(memPackedVector(packedVec), memVector(Vec));
}

TEST(PackedVectorTest, Correctness) {
  const size_t numElements = 1000;
  PackedVector<uint32_t, 10> packedVec;

  for (size_t i = 0; i < numElements; ++i) {
    packedVec.push_back(i);
  }
  for (size_t i = 0; i < numElements; ++i) {
    EXPECT_EQ(packedVec[i], i);
  }
}

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
  PackedVector<uint32_t, 10> pv1{1023, 512, 256};
  PackedVector<uint32_t, 10> pv2;
  pv2 = pv1;

  ASSERT_EQ(pv2.size(), 3);
  EXPECT_EQ(pv2[0], 1023);
  EXPECT_EQ(pv2[1], 512);
  EXPECT_EQ(pv2[2], 256);
}

TEST(PackedVectorTest, MoveAssignment) {
  PackedVector<uint32_t, 10> pv1{1023, 512, 256};
  PackedVector<uint32_t, 10> pv2;
  pv2 = std::move(pv1);

  ASSERT_EQ(pv2.size(), 3);
  EXPECT_EQ(pv2[0], 1023);
  EXPECT_EQ(pv2[1], 512);
  EXPECT_EQ(pv2[2], 256);

  ASSERT_EQ(pv1.size(), 0);
  ASSERT_EQ(pv1.capacity(), 0);
}

TEST(PackedVectorTest, ConstAt) {
  PackedVector<uint32_t, 10> pv{1023, 512, 256};
  EXPECT_EQ(pv.at(0), 1023);
  EXPECT_EQ(pv.at(1), 512);
  EXPECT_EQ(pv.at(2), 256);

  EXPECT_THROW(pv.at(3), std::out_of_range);
  EXPECT_THROW(pv.at(100), std::out_of_range);
}

TEST(PackedVectorTest, Resize) {
  PackedVector<uint32_t, 10> pv{1, 2, 3, 4, 5};

  pv.resize(3);
  ASSERT_EQ(pv.size(), 3);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 2);
  EXPECT_EQ(pv[2], 3);

  pv.resize(5);
  ASSERT_EQ(pv.size(), 5);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 2);
  EXPECT_EQ(pv[2], 3);
  EXPECT_EQ(pv[3], 0);
  EXPECT_EQ(pv[4], 0);

  pv.resize(7);
  ASSERT_EQ(pv.size(), 7);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 2);
  EXPECT_EQ(pv[2], 3);
  EXPECT_EQ(pv[3], 0);
  EXPECT_EQ(pv[4], 0);
  EXPECT_EQ(pv[5], 0);
  EXPECT_EQ(pv[6], 0);
}

TEST(PackedVectorTest, Reserve) {
  PackedVector<uint32_t, 10> pv{1, 2, 3, 4, 5};

  pv.reserve(10);
  ASSERT_EQ(pv.capacity(), 10);

  pv.reserve(20);
  ASSERT_EQ(pv.capacity(), 20);

  pv.reserve(5);
  ASSERT_EQ(pv.capacity(), 20);
}

TEST(PackedVectorTest, ShrinkToFit) {
  PackedVector<uint32_t, 10> pv{1, 2, 3, 4, 5};

  pv.reserve(20);
  ASSERT_EQ(pv.capacity(), 20);

  pv.resize(3);
  pv.shrink_to_fit();
  ASSERT_EQ(pv.capacity(), 3);

  pv.reserve(10);
  pv.shrink_to_fit();
  ASSERT_EQ(pv.capacity(), 3);
}

TEST(PackedVectorIteratorTest, Iterator) {
  PackedVector<uint32_t, 10> pv{1, 2, 3, 4, 5};

  auto it = pv.begin();
  auto end = pv.end();

  int expected = 1;
  while (it != end) {
    EXPECT_EQ(*it, expected);
    ++it;
    ++expected;
  }
}

TEST(PackedVectorIteratorTest, ReverseIterator) {
  PackedVector<uint32_t, 10> pv{1, 2, 3, 4, 5};

  auto rit = pv.rbegin();
  auto rend = pv.rend();

  int expected = 5;
  while (rit != rend) {
    EXPECT_EQ(*rit, expected);
    ++rit;
    --expected;
  }
}

TEST(PackedVectorTest, Insert) {
  PackedVector<int, 4> pv{1, 2, 3};

  pv.insert(1, 4);

  EXPECT_EQ(pv.size(), 4);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 4);
  EXPECT_EQ(pv[2], 2);
  EXPECT_EQ(pv[3], 3);
}

TEST(PackedVectorTest, InsertIterator) {
  PackedVector<int, 4> pv{1, 2, 3};

  auto it = pv.begin() + 1;
  it = pv.insert(it, 4);

  EXPECT_EQ(pv.size(), 4);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 4);
  EXPECT_EQ(pv[2], 2);
  EXPECT_EQ(pv[3], 3);
  EXPECT_EQ(it, pv.begin() + 1);
}

TEST(PackedVectorTest, Erase) {
  PackedVector<int, 4> pv{1, 2, 3};

  pv.erase(1); 

  EXPECT_EQ(pv.size(), 2);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 3);
}
TEST(PackedVectorTest, EraseIterator) {
  PackedVector<int, 4> pv{1, 2, 3};

  auto it = pv.begin() + 1;
  it = pv.erase(it);

  EXPECT_EQ(pv.size(), 2);
  EXPECT_EQ(pv[0], 1);
  EXPECT_EQ(pv[1], 3);
  EXPECT_EQ(it, pv.begin() + 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
