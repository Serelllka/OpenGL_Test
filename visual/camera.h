#pragma once
#include <GL/gl.h>
#include <cmath>


class camera {
public:
    camera(float, float, float, float, float);
    void apply() const;

    void rotate(float, float);
    void move(int, int, float);
    void lift(int, float);

private:
    float x, y, z;
    float x_rot, z_rot;
};
