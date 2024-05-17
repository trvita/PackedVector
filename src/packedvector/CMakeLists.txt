add_library(packedvector
    packedvector/packedvector.cpp
    packedvector/packedvector.hpp
)
target_include_directories(packedvector
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/packedvector
)