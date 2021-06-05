#pragma once
#include "../cube_logic/cube.h"
#include "../algorithm/algorithm.h"
#include<fstream>
#include<iostream>

namespace input_methods
{
    rc_types::colors liter_to_color(char);
    char color_to_liter(rc_types::colors);
    void generate_cube_from_file(const std::string&, cube&);
    void save_cube_to_file(const std::string&, cube&);
    bool check_for_correctness(algorithm& tmp);
    void output_to_console(cube&);
}