#include "cube.h"
#include "../types.h"

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

bool cube::check() {
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
