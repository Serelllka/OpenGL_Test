#pragma once
#include "../cube_logic/lay_manager.h"
#include <windows.h>
#include <GL/gl.h>

class visualizer {
public:
    static void draw_line(float, float, float, float, float, float);

    static void set_color(int8_t color);

    void draw_block(block &, float, float, float, bool = false) const;

    void draw_blocks_grid(block &, float, float, float) const;

    bool rotate_visualization(lay_manager &, const std::string&, bool = false) const;

    void draw_cube(lay_manager &, bool = false) const;

    void texture_initialization(unsigned int &, unsigned int &, unsigned int &, unsigned int &, unsigned int &,
                                unsigned int &) const;

    float face_rotation_speed;
    float block_size;
    int cube_size;
    std::string blue_texture, green_texture, orange_texture, red_texture, white_texture, yellow_texture;
};