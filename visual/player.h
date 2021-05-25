#pragma once
#include "camera.h"

class player {
public:
    player(camera);
    void rotate_camera(int, int, float);
    camera& get_camera();

    void move();
private:
    camera camera_;
};
