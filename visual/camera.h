#pragma once
#include <GL/gl.h>
#include <math.h>


class camera {
public:
    camera(float, float, float, float, float);
    void apply();

    void rotate(float, float);
    void move(int, int, float);
    void lift(int, float);

private:
    float x, y, z;
    float x_rot, z_rot;
};
