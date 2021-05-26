#include"visualizer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int texture_white;
unsigned int texture_yellow;
unsigned int texture_red;
unsigned int texture_orange;
unsigned int texture_green;
unsigned int texture_blue;

void visualizer::draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
{
    glBegin(GL_LINE);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
    glEnd();
}

void visualizer::set_color(int8_t color)
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

void visualizer::draw_blocks_grid(block& blk, float x, float y, float z)
{
    float edges[] = {0,0,0, 0,0,1,  0,1,0, 0,1,1,  1,0,0, 1,0,1,  1,1,0, 1,1,1,
                     0,0,0, 0,1,0,  0,0,0, 1,0,0,  0,1,0, 1,1,0,  1,0,0, 1,1,0,
                     0,0,1, 0,1,1,  0,0,1, 1,0,1,  0,1,1, 1,1,1,  1,0,1, 1,1,1};

    for (int i = 0; i < 72; ++i)
        edges[i] *= block_size;

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

void visualizer::draw_cube(lay_manager& manager)
{
    for (int i = 0; i < manager.size(); ++i)
        for (int j = 0; j < manager.size(); ++j)
            for (int k = 0; k < manager.size(); ++k)
                draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);
}

void visualizer::draw_block(block& blk, float x, float y, float z)
{
    float upp[]     = {0,0,1, 1,0,1, 1,1,1, 0,1,1, //UPP
                       0,0,0, 1,0,0, 1,1,0, 0,1,0, //DOWN
                        0,1,0, 1,1,0, 1,1,1, 0,1,1, //RIGHT
                        0,0,0, 1,0,0, 1,0,1, 0,0,1, //LEFT
                        1,0,0, 1,1,0, 1,1,1, 1,0,1, //FRONT
                        0,0,0, 0,1,0, 0,1,1, 0,0,1}; //BACK



    for (int i = 0; i < 24; ++i)
    {
        upp[3 * i] *= block_size;    upp[3 * i + 1] *= block_size;    upp[3 * i + 2] *= block_size;
        upp[3 * i] += x;    upp[3 * i + 1] += y;    upp[3 * i + 2] += z;
    }
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, upp);

    float coords[] = {0,0, 0,0, 0,0, 0,0,
                      0,0, 0,0, 0,0, 0,0,
                      0,0, 0,0, 0,0, 0,0,
                      0,0, 0,0, 0,0, 0,0,
                      0,0, 0,0, 0,0, 0,0,
                      0,0, 0,0, 0,0, 0,0,};

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if (blk[rc_types::UPP] == rc_types::BLACK)
    {
        set_color(blk[rc_types::UPP]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    else
    {
        for (int i = 0; i < 8; ++i)
            coords[i] = blk.texture_positions[rc_types::UPP][i];

            //set_color(blk[rc_types::UPP]);
            set_color(rc_types::WHITE);

            glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::UPP]);
            glTexCoordPointer(2, GL_FLOAT, 0, coords);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    if (blk[rc_types::DWN] == rc_types::BLACK)
    {
        set_color(blk[rc_types::DWN]);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    }
    else
    {
        for (int i = 8; i < 16; ++i)
            coords[i] = blk.texture_positions[rc_types::DWN][i - 8];

        //set_color(blk[rc_types::DWN]);
        set_color(rc_types::WHITE);

        glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::DWN]);
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    }

    if (blk[rc_types::RGT] == rc_types::BLACK)
    {
        set_color(blk[rc_types::RGT]);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    }
    else
    {
        for (int i = 16; i < 24; ++i)
            coords[i] = blk.texture_positions[rc_types::RGT][i - 16];
            //set_color(blk[rc_types::RGT]);
            set_color(rc_types::WHITE);

            glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::RGT]);
            glTexCoordPointer(2, GL_FLOAT, 0, coords);
            glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    }

    if (blk[rc_types::LFT] == rc_types::BLACK)
    {
        set_color(blk[rc_types::LFT]);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    }
    else
    {
        for (int i = 24; i < 32; ++i)
            coords[i] = blk.texture_positions[rc_types::LFT][i - 24];
        //set_color(blk[rc_types::LFT]);
        set_color(rc_types::WHITE);

        glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::LFT]);
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    }

    if (blk[rc_types::FRT] == rc_types::BLACK)
    {
        set_color(blk[rc_types::FRT]);
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    }
    else
    {
        for (int i = 32; i < 40; ++i)
            coords[i] = blk.texture_positions[rc_types::FRT][i - 32];
        //set_color(blk[rc_types::FRT]);
        set_color(rc_types::WHITE);

        glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::FRT]);
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    }

    if (blk[rc_types::BCK] == rc_types::BLACK)
    {
        set_color(blk[rc_types::BCK]);
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
    }
    else
    {
        for (int i = 40; i < 48; ++i)
            coords[i] = blk.texture_positions[rc_types::BCK][i - 40];
        //set_color(blk[rc_types::BCK]);
        set_color(rc_types::WHITE);

        glBindTexture(GL_TEXTURE_2D, blk.texture_types[rc_types::BCK]);
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
    }

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    draw_blocks_grid(blk, x, y, z);

    x = -x; y = -y; z = -z;
    for (int i = 0; i < 24; ++i)
    {
        upp[3 * i] += x;    upp[3 * i + 1] += y;    upp[3 * i + 2] += z;
    }
}

bool visualizer::rotate_visualization(lay_manager& manager, std::string cmd)
{
    float edge_size = block_size * cube_size;
    static float theta = 1;
    glPushMatrix();
    int ratio = 1, bruh = 1;
    if (cmd == "U" || cmd == "U'" || cmd == "U2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 2; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "U") ratio = -1;
        if (cmd == "U2") bruh = 2;
        glTranslatef(edge_size/2, edge_size/2, 0);
        glRotatef(ratio * theta * bruh, 0, 0, 1);
        glTranslatef(-edge_size/2, -edge_size/2, 0);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][j][2], i * block_size, j * block_size, 2 * block_size);
    }
    else if (cmd == "D" || cmd == "D'" || cmd == "D2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 1; k < 3; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "D'") ratio = -1;
        if (cmd == "D2") bruh = 2;
        glTranslatef(edge_size/2, edge_size/2, 0);
        glRotatef(ratio * theta * bruh, 0, 0, 1);
        glTranslatef(-edge_size/2, -edge_size/2, 0);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][j][0], i * block_size, j * block_size, 0);
    }
    else if (cmd == "F" || cmd == "F'" || cmd == "F2")
    {
        for(int i = 0; i < 2; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "F") ratio = -1;
        if (cmd == "F2") bruh = 2;
        glTranslatef(0, edge_size/2, edge_size/2);
        glRotatef(ratio * theta * bruh, 1, 0, 0);
        glTranslatef(0, -edge_size/2, -edge_size/2);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[2][i][j], 2 * block_size, i * block_size, j * block_size);
    }
    else if (cmd == "B" || cmd == "B'" || cmd == "B2")
    {
        for(int i = 1; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "B'") ratio = -1;
        if (cmd == "B2") bruh = 2;
        glTranslatef(0, edge_size/2, edge_size/2);
        glRotatef(ratio * theta * bruh, 1, 0, 0);
        glTranslatef(0, -edge_size/2, -edge_size/2);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[0][i][j], 0, i * block_size, j * block_size);
    }
    else if (cmd == "R" || cmd == "R'" || cmd == "R2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "R") ratio = -1;
        if (cmd == "R2") bruh = 2;
        glTranslatef(edge_size/2, 0, edge_size/2);
        glRotatef(ratio * theta * bruh, 0, 1, 0);
        glTranslatef(-edge_size/2, 0, -edge_size/2);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][2][j], i * block_size, 2 * block_size, j * block_size);
    }
    else if (cmd == "L" || cmd == "L'" || cmd == "L2")
    {
        for(int i = 0; i < 3; ++i)
            for (int j = 1; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    draw_block(manager[i][j][k], i * block_size, j * block_size, k * block_size);

        if (cmd == "L'") ratio = -1;
        if (cmd == "L2") bruh = 2;
        glTranslatef(edge_size/2, 0, edge_size/2);
        glRotatef(ratio * theta * bruh, 0, 1, 0);
        glTranslatef(-edge_size/2, 0, -edge_size/2);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                draw_block(manager[i][0][j], i * block_size, 0, j * block_size);
    }
    glPopMatrix();

    theta += face_rotation_speed;
    if (theta > 90)
    {
        theta = 0;
        return true;
    }
    return false;
}

void visualizer::texture_initialization(unsigned int& wt, unsigned int& yt,unsigned int& rt,
                            unsigned int& ot, unsigned int& gt,unsigned int& bt)
{
    int width, height, cnt;
    unsigned char *data = stbi_load("../textures/white.jpg", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_white);
    glBindTexture(GL_TEXTURE_2D, texture_white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                                0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    data = stbi_load("../textures/yellow.png", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_yellow);
    glBindTexture(GL_TEXTURE_2D, texture_yellow);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    data = stbi_load("../textures/red.png", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_red);
    glBindTexture(GL_TEXTURE_2D, texture_red);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    data = stbi_load("../textures/orange.jpg", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_orange);
    glBindTexture(GL_TEXTURE_2D, texture_orange);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    data = stbi_load("../textures/green.jpg", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_green);
    glBindTexture(GL_TEXTURE_2D, texture_green);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    data = stbi_load("../textures/blue.png", &width, &height, &cnt, 0);

    glGenTextures(1, &texture_blue);
    glBindTexture(GL_TEXTURE_2D, texture_blue);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    wt = texture_white;
    yt = texture_yellow;
    rt = texture_red;
    bt = texture_blue;
    gt = texture_green;
    ot = texture_orange;
}