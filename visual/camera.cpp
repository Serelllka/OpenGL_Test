#include <cmath>
#include "camera.h"

camera::camera(float x, float y, float z, float x_rot, float z_rot)
    : x(x), y(y), z(z), x_rot(x_rot), z_rot(z_rot)
{}

void camera::rotate(float x_angle, float z_angle)
{
    this->z_rot += z_angle;
    if(this->z_rot < 0) this->z_rot += 360;
    if(this->z_rot > 360) this->z_rot -= 360;
    this->x_rot += x_angle;
    if(this->x_rot < 0) this->x_rot += 360;
    if(this->x_rot > 360) this->x_rot -= 360;
}

void camera::apply() const
{
    glRotatef(-this->x_rot, 1,0,0);
    glRotatef(-this->z_rot, 0,0,1);
    glTranslatef(-this->x, -this->y, -this->z);
}

void camera::move(int forward_move, int right_move, float speed)
{
    float angle = -this->z_rot / 180 * M_PI;
    if (forward_move > 0)
        angle += right_move > 0 ? M_PI_4 : (right_move < 0 ? -M_PI_4 : 0);
    if (forward_move < 0)
        angle += M_PI + (right_move > 0 ? -M_PI_4 : (right_move < 0 ? M_PI_4 : 0));
    if (forward_move == 0)
    {
        angle += right_move > 0 ? M_PI_2 : -M_PI_2;
        if (right_move == 0) speed = 0;
    }

    if (speed != 0)
    {
        this->x += std::sin(angle) * speed;
        this->y += std::cos(angle) * speed;
    }
}

void camera::lift(int up_move, float speed)
{
    if (up_move != 0) {
        this->z += speed * (float)up_move;
    }
}


