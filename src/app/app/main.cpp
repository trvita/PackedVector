#include <cstdlib>
#include <iostream>
#include <malloc.h>
#include <packedvector.hpp>
#include <vector>
using namespace packedvector;

template <typename T, size_t N>
size_t memPackedVector(const PackedVector<T, N> &packedVec) {
  return sizeof(packedVec) + malloc_usable_size(packedVec.getData().get());
}

template <typename T, size_t N>
void print(const PackedVector<T, N> &packedVec, const std::string name) {
  std::cout << name << "by " << N << " bits" << std::endl;
  std::cout << "elems: ";
  for (size_t i = 0; i < packedVec.size(); ++i) {
    std::cout << packedVec[i] << " ";
  }

  std::cout << "memory: " << memPackedVector(packedVec) << " bytes"
            << std::endl;
  std::cout << std::endl;
}

int main() {

  PackedVector<int, 3> intVector{1, 2, 3, 4, 5};
  PackedVector<unsigned int, 3> uintVector{1, 2, 3, 4, 5};

  PackedVector<long, 9> longVector{100, 200, 300, 400, 500};
  PackedVector<unsigned long, 9> ulongVector{100, 200, 300, 400, 500};

  PackedVector<long long, 19> longLongVector{100000, 200000, 300000, 400000,
                                             500000};
  PackedVector<unsigned long long, 19> ulongLongVector{100000, 200000, 300000,
                                                       400000, 500000};

  PackedVector<short, 3> shortVector{1, 2, 3, 4, 5};

  PackedVector<unsigned short, 3> ushortVector{1, 2, 3, 4, 5};

  PackedVector<char, 7> charVector{'a', 'b', 'c', 'd', 'e'};

  PackedVector<unsigned char, 7> ucharVector{65, 66, 67, 68, 69};

  PackedVector<std::int8_t, 7> int8Vector{97, 98, 99, 100, 101};

  PackedVector<std::uint8_t, 7> uint8Vector{65, 66, 67, 68, 69};

  PackedVector<std::int16_t, 9> int16Vector{100, 200, 300, 400, 500};

  PackedVector<std::uint16_t, 9> uint16Vector{100, 200, 300, 400, 500};

  PackedVector<std::int32_t, 19> int32Vector{100000, 200000, 300000, 400000,
                                             500000};

  PackedVector<std::uint32_t, 19> uint32Vector{100000, 200000, 300000, 400000,
                                               500000};

  PackedVector<std::int64_t, 36> int64Vector{1000000000, 2000000000, 300000000};

  PackedVector<std::uint64_t, 36> uint64Vector{1000000000, 2000000000,
                                               300000000};
  print(intVector, "Int Vector ");
  print(uintVector, "Unsigned Int Vector ");
  print(longVector, "Long Vector ");
  print(ulongVector, "Unsigned Long Vector ");
  print(longLongVector, "Long Long Vector ");
  print(ulongLongVector, "Unsigned Long Long Vector ");
  print(shortVector, "Short Vector ");
  print(ushortVector, "Unsigned Short Vector ");
  print(charVector, "Char Vector ");
  print(ucharVector, "Unsigned Char Vector ");
  print(int8Vector, "Int8_t Vector ");
  print(uint8Vector, "Uint8_t Vector ");
  print(int16Vector, "Int16_t Vector ");
  print(uint16Vector, "Uint16_t Vector ");
  print(int32Vector, "Int32_t Vector ");
  print(uint32Vector, "Uint32_t Vector ");
  print(int64Vector, "Int64_t Vector ");
  print(uint64Vector, "Uint64_t Vector ");

  return 0;
}
