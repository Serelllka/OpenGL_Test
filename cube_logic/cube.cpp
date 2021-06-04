#include "cube.h"

cube::cube(size_t size)
    : size_(size)
{
    matrix3d_.assign(size, std::vector<std::vector<block>>(size, std::vector<block>(size)));
}

void cube::generate_cube()
{
    for (size_t i = 0; i < size_; ++i)
        for (size_t j = 0; j < size_; ++j)
            for (size_t k = 0; k < size_; ++k)
            {
                for (auto& it : matrix3d_[i][j][k].edges_) it = rc_types::BLACK;
                if (k == size_ - 1) matrix3d_[i][j][k][rc_types::UPP] = rc_types::WHITE;
                if (k == 0)         matrix3d_[i][j][k][rc_types::DWN] = rc_types::YELLOW;
                if (j == size_ - 1) matrix3d_[i][j][k][rc_types::RGT] = rc_types::RED;
                if (j == 0)         matrix3d_[i][j][k][rc_types::LFT] = rc_types::ORANGE;
                if (i == size_ - 1) matrix3d_[i][j][k][rc_types::FRT] = rc_types::GREEN;
                if (i == 0)         matrix3d_[i][j][k][rc_types::BCK] = rc_types::BLUE;
            }
}

size_t cube::size() const {
    return size_;
}

std::vector<std::vector<block>> &cube::operator[](size_t k) {
    return matrix3d_[k];
}

[[maybe_unused]] bool cube::check() {
    for (size_t i = 0; i < size_; ++i)
        for (size_t j = 0; j < size_; ++j)
            for (size_t k = 0; k < size_; ++k)
            {
                //for (auto& it : matrix3d_[i][j][k].edges_) it = rc_types::BLACK;
                if (k == size_ - 1  && matrix3d_[i][j][k][rc_types::UPP] != rc_types::WHITE)    return false;
                if (k == 0          &&  matrix3d_[i][j][k][rc_types::DWN] != rc_types::YELLOW)  return false;
                if (j == size_ - 1  && matrix3d_[i][j][k][rc_types::RGT] != rc_types::RED)      return false;
                if (j == 0          && matrix3d_[i][j][k][rc_types::LFT] != rc_types::ORANGE)   return false;
                if (i == size_ - 1  && matrix3d_[i][j][k][rc_types::FRT] != rc_types::GREEN)    return false;
                if (i == 0          && matrix3d_[i][j][k][rc_types::BCK] != rc_types::BLUE)     return false;
            }
    return true;
}

void cube::generate_texture(unsigned int wt, unsigned int yt, unsigned int rt,
                            unsigned int ot, unsigned int gt, unsigned int bt)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                for (auto& it : matrix3d_[i][j][k].texture_types) it = 0;
                if (k == size_ - 1) matrix3d_[i][j][k].texture_types[rc_types::UPP] = wt;
                if (k == 0)         matrix3d_[i][j][k].texture_types[rc_types::DWN] = yt;
                if (j == size_ - 1) matrix3d_[i][j][k].texture_types[rc_types::RGT] = rt;
                if (j == 0)         matrix3d_[i][j][k].texture_types[rc_types::LFT] = ot;
                if (i == size_ - 1) matrix3d_[i][j][k].texture_types[rc_types::FRT] = gt;
                if (i == 0)         matrix3d_[i][j][k].texture_types[rc_types::BCK] = bt;
            }
        }
    }

    float texture_vertex_coordinates[] = {0,0, 0.33,0, 0.33,0.33, 0,0.33};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                matrix3d_[i][j][2].texture_positions[rc_types::UPP][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[i][j][2].texture_positions[rc_types::UPP][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);

                matrix3d_[i][j][0].texture_positions[rc_types::DWN][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[i][j][0].texture_positions[rc_types::DWN][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);

                matrix3d_[i][0][j].texture_positions[rc_types::LFT][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[i][0][j].texture_positions[rc_types::LFT][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);

                matrix3d_[i][2][j].texture_positions[rc_types::RGT][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[i][2][j].texture_positions[rc_types::RGT][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);

                matrix3d_[0][i][j].texture_positions[rc_types::BCK][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[0][i][j].texture_positions[rc_types::BCK][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);

                matrix3d_[2][i][j].texture_positions[rc_types::FRT][2 * k] =     static_cast<float>(texture_vertex_coordinates[2 * k] + i * 0.33);
                matrix3d_[2][i][j].texture_positions[rc_types::FRT][2 * k + 1] = static_cast<float>(texture_vertex_coordinates[2 * k + 1] + j * 0.33);
            }
        }
    }
}
