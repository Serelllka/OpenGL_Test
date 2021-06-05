#include <windows.h>
#include <cmath>
#include <iostream>
#include "visual/visualizer.h"
#include "algorithm/algorithm.h"
#include "visual/player.h"
#include "config/parser.h"
#include "input/input_class.h"

float near_val, far_val;

std::vector<std::string> Randomize()
{
    std::vector<std::string> tmp;
    int n = std::rand() % 17 + 4;
    for (int i = 0; i < n; ++i)
    {
        int k = std::rand() % 6;
        switch (k)
        {
            case 1:
                tmp.emplace_back("U");
                break;
            case 2:
                tmp.emplace_back("D");
                break;
            case 3:
                tmp.emplace_back("R");
                break;
            case 4:
                tmp.emplace_back("L");
                break;
            case 5:
                tmp.emplace_back("F");
                break;
            case 6:
                tmp.emplace_back("B");
                break;
        }
    }

    return tmp;
}

void ShowWorld()
{
    float tile[] = {0,0,0, 0,1,0, 1,1,0, 1,0,0};

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &tile);

        for (int i = -5; i < 50; ++i)
        {
            for (int j = -5; j < 50; ++j)
            {
                glPushMatrix();
                    if((i + j) % 2) glColor3f(0, 0.5, 0);
                    else glColor3f(1, 1, 1);
                    glTranslatef(static_cast<float>(i), static_cast<float>(j), 0);
                    glVertexPointer(3, GL_FLOAT, 0, &tile);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
        }
}

bool tick_time(float tick_time)
{
    static int count_of_ticks = 0;
    count_of_ticks %= static_cast<int>(0.2 / tick_time + 1);
    count_of_ticks++;
    //std::cout << count_of_ticks;
    return !(count_of_ticks % static_cast<int>(0.2 / tick_time + 1));
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    Randomize();
    visualizer visual;
    bool textures_are_used = false;
    parser prs("../config/config.cfg");
    float tick_speed, cube_rotation_angle, cube_rotation_speed;
    prs.read_param(tick_speed, "tick_speed");
    prs.read_param(cube_rotation_angle, "tick_speed");
    prs.read_param(cube_rotation_speed, "cube_rotation_speed");
    prs.read_param(near_val, "near_value");
    prs.read_param(far_val, "far_value");
    prs.read_param(textures_are_used, "textures_are_used");
    prs.read_param(visual.face_rotation_speed, "face_rotation_speed");
    prs.read_param(visual.block_size, "block_size");
    prs.read_param(visual.cube_size, "cube_size");
    prs.read_param(visual.white_texture, "white_texture");
    prs.read_param(visual.red_texture, "red_texture");
    prs.read_param(visual.blue_texture, "blue_texture");
    prs.read_param(visual.green_texture, "green_texture");
    prs.read_param(visual.orange_texture, "orange_texture");
    prs.read_param(visual.yellow_texture, "yellow_texture");
    //std::cout << textures_are_used;

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          500,
                          500,
                          nullptr,
                          nullptr,
                          hInstance,
                          nullptr);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* reading rendering parameters */

    glEnable(GL_DEPTH_TEST);

    glFrustum(-1,1, -1,1, near_val,far_val);

    cube cube_for_solution(3); // this cube using for generate solution
    cube cube_for_rotation(3); // this cube using for making rotations
    lay_manager solution_manager(&cube_for_solution);
    lay_manager rotation_manager(&cube_for_rotation);
    algorithm algo(solution_manager);

    cube_for_solution.generate_cube();
    cube_for_rotation.generate_cube();

    unsigned int wt, yt, rt, ot, gt, bt;
    visual.texture_initialization(wt, yt, rt, ot, gt, bt);
    cube_for_rotation.generate_texture(wt, yt, rt, ot, gt, bt);

    float hight = 4;

    camera cam(0,0,1.7, 0,0);
    player gamer(cam);

    /* program main loop */
    bool is_solved = false, is_rotated = false;
    bool isPaused = true;
    int solve_iteration = 0;

    std::string cmd;
    float edge_size = visual.block_size * static_cast<float>(visual.cube_size);
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();

            if (GetForegroundWindow() == hwnd)
            {
                gamer.move();
                if (GetKeyState('P') < 0)
                    isPaused = true;
                if (GetKeyState('C') < 0)
                    isPaused = false;
                if (GetKeyState('Z') < 0)
                    input_methods::save_cube_to_file("../output.txt", solution_manager.get_cube());
                if (GetKeyState('O') < 0)
                    input_methods::output_to_console(solution_manager.get_cube());

                if ((GetKeyState('L') < 0) && isPaused)
                {
                    input_methods::generate_cube_from_file("../input.txt", cube_for_solution);
                    input_methods::generate_cube_from_file("../input.txt", cube_for_rotation);
                    solve_iteration = 0;
                    //is_rotated = false;
                    is_solved = false;
                    algo.log().clear();
                    if (!input_methods::check_for_correctness(algo))
                    {
                        std::cout << "The Rubick's cube can't be solved from this position";
                        return 1;
                    }
                    else
                    {
                        formatting(algo.log());
                        cmd = !algo.log().empty() ? algo.log()[0] : "";
                    }
                }

                if (GetKeyState('7') < 0)
                {
                    auto tmp = Randomize();
                    for (auto& it : tmp)
                    {
                        rotation_manager.rotate(it);
                    }
                    solution_manager.get_cube() = rotation_manager.get_cube();
                    solve_iteration = 0;
                    is_rotated = false;
                    is_solved = false;
                    algo.log().clear();
                    input_methods::check_for_correctness(algo);
                    formatting(algo.log());
                    cmd = !algo.log().empty() ? algo.log()[0] : "";
                }

                if (GetKeyState('1') < 0 || GetKeyState('2') < 0 || GetKeyState('3') < 0
                ||  GetKeyState('4') < 0 || GetKeyState('5') < 0 || GetKeyState('6') < 0)
                {
                    bool is_rotated_time = false;
                    if ((GetKeyState('1') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("U");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if ((GetKeyState('2') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("D");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if ((GetKeyState('3') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("R");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if ((GetKeyState('4') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("L");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if ((GetKeyState('5') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("F");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if ((GetKeyState('6') < 0) && isPaused)
                        if (tick_time(tick_speed))
                        {
                            is_rotated_time = true;
                            rotation_manager.rotate("B");
                            solution_manager.get_cube() = rotation_manager.get_cube();
                        }
                    if (is_rotated_time)
                    {
                        solve_iteration = 0;
                        is_rotated = false;
                        is_solved = false;
                        algo.log().clear();
                        input_methods::check_for_correctness(algo);
                        formatting(algo.log());
                        cmd = !algo.log().empty() ? algo.log()[0] : "";
                    }
                }
            }

            gamer.get_camera().apply();
            ShowWorld();

            glTranslatef(6, 6, hight);
            glTranslatef(edge_size / 2, edge_size / 2, 0);
            glRotatef(cube_rotation_angle, 0, 0, 1);
            glTranslatef(-edge_size / 2, -edge_size / 2, 0);
            if (!isPaused)
            {
                if (is_solved)
                {
                    is_rotated = false;
                    visual.draw_cube(rotation_manager, textures_are_used);
                }
                else
                {
                    is_rotated = visual.rotate_visualization(rotation_manager, cmd, textures_are_used);
                }
            }
            else
            {
                visual.draw_cube(rotation_manager, textures_are_used);
            }
            glPopMatrix();

            if (is_rotated && !isPaused)
            {
                if (solve_iteration < algo.log().size()) rotation_manager.rotate(algo.log()[solve_iteration]);
                ++solve_iteration;
                if (solve_iteration < algo.log().size())
                    cmd = algo.log()[solve_iteration];
                else
                    is_solved = true;
            }

            SwapBuffers(hDC);

            cube_rotation_angle += cube_rotation_speed;
            Sleep(static_cast<DWORD>(tick_speed));
        }
    }
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            glViewport(0,0, LOWORD(lParam), HIWORD(lParam));
            glLoadIdentity();
            float k;
            k = LOWORD(lParam) / (float)HIWORD(lParam);
            glFrustum(-k,k, -1,1, 0,800);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
        }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

