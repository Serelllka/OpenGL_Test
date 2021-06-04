#pragma once
#include "camera.h"

class player {
public:
    explicit player(camera);
    void rotate_camera(int, int, float);
    camera& get_camera();

    void move();
private:
    camera camera_;
};
