#pragma once
#include<iostream>
#include"../types.h"
#include<array>

class block
{
public:

    void rotate(rc_types::dimensions);

    int8_t& operator[] (rc_types::edges edge);

    std::array<int8_t, 6> edges_;
};
