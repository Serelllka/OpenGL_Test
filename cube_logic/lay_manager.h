#pragma once
#include "cube.h"
#include "../types.h"
#include "block.h"


class lay_manager {
public:
    explicit lay_manager(cube* cb);

    void rotate(int = -1, int = -1, int = -1, int = 1);
    void rotate(std::vector<std::string>);
    void rotate(std::string);

    std::vector<std::vector<block>> get_lay(int, int, int);
    std::vector<std::vector<block>> get_lay(rc_types::edges);
    void set_lay(std::vector<std::vector<block>>& , int, int, int);

    std::vector<std::vector<block>>& operator[] (size_t);

    size_t size();

    cube& get_cube();

private:
    //std::vector<std::vector<block*>> matrix;
    cube* cube_;
    size_t size_;
};