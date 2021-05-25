#pragma once
#include <vector>
#include <string>
#include "helper_functions.h"
#include "../cube_logic/cube.h"
#include "../cube_logic/lay_manager.h"

using namespace rc_types;
using namespace helper_function;

class algorithm {
public:
    explicit algorithm(lay_manager&);

    void makeRotation(std::string);

    void to_bottom_position(block);
    void to_upper_position(block&);
    void first_stage();

    void second_stage();
    bool second_stage_checker(block&);
    void to_corner_position(lay_manager&, block&);

    bool third_stage_checker(block&);
    bool third_stage_lay_checker(block& blk);
    void third_stage();
    bool toBottom(lay_manager&);

    bool has_a_cross();
    bool has_a_corner();
    bool has_a_line();
    void fourth_stage();

    void fifth_stage();

    void sixth_stage();

    void seventh_stage();

    void solution();

    lay_manager& manager();
    std::vector<std::string>& log();
private:
    lay_manager manager_;
    std::vector<std::string> solution_log_;
};
