#pragma once
#include<iostream>
#include"../types.h"
#include<array>
#include <vector>

class block
{
public:

    void rotate(rc_types::dimensions);

    int8_t& operator[] (rc_types::edges edge);

    std::array<int8_t, 6> edges_;

    //int i, j, k;
    std::array<unsigned int, 6> texture_types;
    std::array<std::array<float, 8>, 6> texture_positions;
};
