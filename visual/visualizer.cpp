#include"visualizer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int texture;

void draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
{
    glBegin(GL_LINE);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
    glEnd();
}

void set_color(int8_t color)
{
    switch (color)
    {
        case rc_types::WHITE:
            glColor3f(1, 1, 1);
            break;
        case rc_types::YELLOW:
            glColor3f(1, 1, 0);
            break;
        case rc_types::RED:
            glColor3f(1, 0, 0);
            break;
        case rc_types::ORANGE:
            glColor3f(1, 0.75, 0);
            break;
        case rc_types::GREEN:
            glColor3f(0, 1, 0);
            break;
        case rc_types::BLUE:
            glColor3f(0, 0, 1);
            break;
        case rc_types::BLACK:
            glColor3f(0, 0, 0);
            break;
    }
}

void draw_blocks_grid(block& blk, float x, float y, float z)
{
    float edges[] = {0,0,0, 0,0,1,  0,1,0, 0,1,1,  1,0,0, 1,0,1,  1,1,0, 1,1,1,
                     0,0,0, 0,1,0,  0,0,0, 1,0,0,  0,1,0, 1,1,0,  1,0,0, 1,1,0,
                     0,0,1, 0,1,1,  0,0,1, 1,0,1,  0,1,1, 1,1,1,  1,0,1, 1,1,1};

    set_color(rc_types::BLACK);
    glLineWidth(3);

    glEnableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
    glTranslatef(x, y, z);
    glVertexPointer(3, GL_FLOAT, 0, &edges);
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 2, 2);
    glDrawArrays(GL_LINES, 4, 2);
    glDrawArrays(GL_LINES, 6, 2);

    glDrawArrays(GL_LINES, 8, 2);
    glDrawArrays(GL_LINES, 10, 2);
    glDrawArrays(GL_LINES, 12, 2);
    glDrawArrays(GL_LINES, 14, 2);

    glDrawArrays(GL_LINES, 16, 2);
    glDrawArrays(GL_LINES, 18, 2);
    glDrawArrays(GL_LINES, 20, 2);
    glDrawArrays(GL_LINES, 22, 2);
    glTranslatef(-x, -y, -z);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_cube(lay_manager& manager)
{
    for (int i = 0; i < manager.size(); ++i)
        for (int j = 0; j < manager.size(); ++j)
            for (int k = 0; k < manager.size(); ++k)
                draw_block(manager[i][j][k], i, j, k);
}

void draw_block(block& blk, float x, float y, float z)
{
    float upp[]     = {0,0,1, 1,0,1, 1,1,1, 0,1,1,  //UPP
                       0,0,0, 1,0,0, 1,1,0, 0,1,0,  //DOWN
                       0,1,0, 1,1,0, 1,1,1, 0,1,1,  //RIGHT
                       0,0,0, 1,0,0, 1,0,1, 0,0,1,  //LEFT
                       1,0,0, 1,1,0, 1,1,1, 1,0,1,  //FRONT
                       0,0,0, 0,1,0, 0,1,1, 0,0,1}; //BACK

    float texCoord[] = {0,0, 0,1, 1,1, 1,0};

    for (int i = 0; i < 24; ++i)
    {
        upp[3 * i] += x;    upp[3 * i + 1] += y;    upp[3 * i + 2] += z;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);


    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    set_color(blk[rc_types::UPP]);
    glVertexPointer(3, GL_FLOAT, 0, &upp);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    set_color(blk[rc_types::DWN]);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

    set_color(blk[rc_types::RGT]);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

    set_color(blk[rc_types::LFT]);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

    set_color(blk[rc_types::FRT]);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

    set_color(blk[rc_types::BCK]);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    draw_blocks_grid(blk, x, y, z);

    x = -x; y = -y; z = -z;
    for (int i = 0; i < 24; ++i)
    {
        upp[3 * i] += x;    upp[3 * i + 1] += y;    upp[3 * i + 2] += z;
    }
}

bool rotate_visualization(lay_manager& manager, std::string cmd)
{
    static float theta = 1;
    glPushMatrix();
    int ratio = 1, bruh = 1;
    if (cmd == "U" || cmd == "U'" || cmd == "U2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 2; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "U") ratio = -1;
        if (cmd == "U2") bruh = 2;
        glTranslatef(1.5, 1.5, 0);
        glRotatef(ratio * theta * bruh, 0, 0, 1);
        glTranslatef(-1.5, -1.5, 0);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][j][2], i, j, 2);
    }
    else if (cmd == "D" || cmd == "D'" || cmd == "D2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 1; k < 3; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "D'") ratio = -1;
        if (cmd == "D2") bruh = 2;
        glTranslatef(1.5, 1.5, 0);
        glRotatef(ratio * theta * bruh, 0, 0, 1);
        glTranslatef(-1.5, -1.5, 0);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][j][0], i, j, 0);
    } // it's working!
    else if (cmd == "F" || cmd == "F'" || cmd == "F2")
    {
        for(int i = 0; i < 2; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "F") ratio = -1;
        if (cmd == "F2") bruh = 2;
        glTranslatef(0, 1.5, 1.5);
        glRotatef(ratio * theta * bruh, 1, 0, 0);
        glTranslatef(0, -1.5, -1.5);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[2][i][j], 2, i, j);
    }
    else if (cmd == "B" || cmd == "B'" || cmd == "B2")
    {
        for(int i = 1; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "B'") ratio = -1;
        if (cmd == "B2") bruh = 2;
        glTranslatef(0, 1.5, 1.5);
        glRotatef(ratio * theta * bruh, 1, 0, 0);
        glTranslatef(0, -1.5, -1.5);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[0][i][j], 0, i, j);
    }
    else if (cmd == "R" || cmd == "R'" || cmd == "R2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "R") ratio = -1;
        if (cmd == "R2") bruh = 2;
        glTranslatef(1.5, 0, 1.5);
        glRotatef(ratio * theta * bruh, 0, 1, 0);
        glTranslatef(-1.5, 0, -1.5);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][2][j], i, 2, j);
    }
    else if (cmd == "L" || cmd == "L'" || cmd == "L2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 1; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i, j, k);

        if (cmd == "L'") ratio = -1;
        if (cmd == "L2") bruh = 2;
        glTranslatef(1.5, 0, 1.5);
        glRotatef(ratio * theta * bruh, 0, 1, 0);
        glTranslatef(-1.5, 0, -1.5);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][0][j], i, 0, j);
    }
    glPopMatrix();

    theta += 8;
    if (theta > 90)
    {
        theta = 0;
        return true;
    }
    return false;
}

void texture_initialization()
{
    int width, height, cnt;
    unsigned char *data = stbi_load("../textures/rem.png", &width, &height, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}