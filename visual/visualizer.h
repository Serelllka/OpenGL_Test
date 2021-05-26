#pragma once
#include "../cube_logic/lay_manager.h"
#include <windows.h>
#include <GL/gl.h>

class visualizer {
public:
    void draw_line(float, float, float, float, float, float);

    void set_color(int8_t color);

    void draw_block(block &, float, float, float);

    void draw_blocks_grid(block &, float, float, float);

    bool rotate_visualization(lay_manager &, std::string);

    void draw_cube(lay_manager &);

    void texture_initialization(unsigned int &, unsigned int &, unsigned int &, unsigned int &, unsigned int &,
                                unsigned int &);

    float face_rotation_speed;
    float block_size;
    int cube_size;
    std::string blue_texture, green_texture, orange_texture, red_texture, white_texture, yellow_texture;
};