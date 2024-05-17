add_executable(packedvectorUnitTests
    packedvector/packedvector.cpp
)

target_link_libraries(
    packedvectorUnitTests
    PRIVATE
    GTest::gtest_main
    mypackedvector
)

include(GoogleTest)
gtest_discover_tests(packedvectorUnitTests)
