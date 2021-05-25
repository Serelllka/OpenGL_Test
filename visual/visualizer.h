#pragma once
#include "../cube_logic/lay_manager.h"
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void draw_line(float, float, float, float, float, float);

void set_color(int8_t color);

void draw_block(block&, float, float, float z);
void draw_blocks_grid(block&, float, float, float);

bool rotate_visualization(lay_manager&, std::string);

void draw_cube(lay_manager&);

void texture_initialization();