#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include "visual/visualizer.h"
#include "algorithm/algorithm.h"
#include "visual/camera.h"
#include "visual/player.h"
#include "config/parser.h"

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
                    glTranslatef(i, j, 0);
                    glVertexPointer(3, GL_FLOAT, 0, &tile);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
        }
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
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
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


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
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    parser prs("../config/config.cfg");
    /* reading rendering parameters */
    visualizer visual;
    float tick_speed, cube_rotation_angle, cube_rotation_speed;
    prs.read_param(tick_speed, "tick_speed");
    prs.read_param(cube_rotation_angle, "tick_speed");
    prs.read_param(cube_rotation_speed, "cube_rotation_speed");
    prs.read_param(visual.face_rotation_speed, "face_rotation_speed");
    prs.read_param(visual.block_size, "block_size");
    prs.read_param(visual.cube_size, "cube_size");

    glEnable(GL_DEPTH_TEST);

    glFrustum(-1,1, -1,1, 2,800);

    cube cb(3), tmp(3);
    cb.generate_cube();

    unsigned int wt, yt, rt, ot, gt, bt;
    visual.texture_initialization(wt, yt, rt, ot, gt, bt);
    cb.generate_texture(wt, yt, rt, ot, gt, bt);

    //tmp.generate_cube();

    lay_manager manager(&cb);
    algorithm algo(manager);
    algo.makeRotation("F R U B L B' D F' B' U2");
    algo.solution();

    formatting(algo.log());

    float hight = 4;
    std::string cmd = algo.log()[0];
    int i = 0;

    camera cam(0,0,1.7, 0,0);
    player gamer(cam);

    /* program main loop */
    bool f = true, flag = false;
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
            float edge_size = visual.block_size * visual.cube_size;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
                //MoveCamera();

                if (GetForegroundWindow() == hwnd)
                    gamer.move();
                gamer.get_camera().apply();
                ShowWorld();

                glTranslatef(6, 6, hight);
                glTranslatef(edge_size/2, edge_size/2, 0);
                glRotatef(cube_rotation_angle, 0, 0, 1);
                glTranslatef(-edge_size/2, -edge_size/2, 0);
                    if (flag)
                    {
                        f = false;
                        visual.draw_cube(manager);
                    }
                    else
                    {
                        f = visual.rotate_visualization(manager, cmd);
                    }
            glPopMatrix();

            if (f)
            {
                manager.rotate(algo.log()[i]);
                ++i;
                if (i < algo.log().size())
                    cmd = algo.log()[i];
                else
                    flag = true;
            }

            SwapBuffers(hDC);

            cube_rotation_angle += cube_rotation_speed;
            Sleep (tick_speed);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
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
            glFrustum(-k,k, -1,1, 2,800);
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
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

