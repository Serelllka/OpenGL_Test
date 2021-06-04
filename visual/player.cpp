//
// Created by vprog on 5/25/2021.
//

#include <windows.h>
#include "player.h"

player::player(camera cam)
    : camera_(cam)
{}

void player::rotate_camera(int centerX, int centerY, float speed)
{
    POINT cur;
    POINT base = {centerX, centerY};
    GetCursorPos(&cur);
    camera_.rotate((base.y - cur.y) / 10.0, (base.x - cur.x) / 10.0);
    SetCursorPos(base.x, base.y);
}

camera &player::get_camera()
{
    return camera_;
}

void player::move()
{
    camera_.move(GetKeyState('W') < 0 ? 1 : (GetKeyState('S') < 0 ? -1 : 0),
                 GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1 : 0),
                 0.1);
    camera_.lift(GetKeyState('R') < 0 ? 1 : (GetKeyState('F') < 0 ? -1 : 0), 0.1);
    this->rotate_camera(400, 400, 0.2);
}
