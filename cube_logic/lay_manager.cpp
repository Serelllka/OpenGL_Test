//
// Created by vprog on 5/10/2021.
//

#include "lay_manager.h"

lay_manager::lay_manager(cube* cb)
    : cube_(cb), size_(cb->size()) {}

void lay_manager::rotate(int i, int j, int k, int times) {
    for (size_t timer = 0; timer < times; ++timer)
    {
        std::vector<std::vector<block>> matrix = this->get_lay(i, j, k);
        std::vector<std::vector<block>> tmp(size_, std::vector<block>(size_, block()));
        for (size_t it_i = 0; it_i < size_; ++it_i)
            for (size_t it_j = 0; it_j < size_; ++it_j)
                tmp[size_ - it_j - 1][it_i] = matrix[it_i][it_j];

        for (size_t it_i = 0; it_i < size_; ++it_i)
        {
            for (size_t it_j = 0; it_j < size_; ++it_j)
            {
                if (i != -1)
                {
                    (*cube_)[i][it_i][it_j] = tmp[it_i][it_j];
                    (*cube_)[i][it_i][it_j].rotate(rc_types::X);
                }
                else if (j != -1)
                {
                    (*cube_)[it_i][j][it_j] = tmp[it_i][it_j];
                    (*cube_)[it_i][j][it_j].rotate(rc_types::Y);
                }
                else if (k != -1)
                {
                    (*cube_)[it_i][it_j][k] = tmp[it_i][it_j];
                    (*cube_)[it_i][it_j][k].rotate(rc_types::Z);
                }
            }
        }
    }
}

std::vector<std::vector<block>> lay_manager::get_lay(int i, int j, int k) {
    std::vector<std::vector<block>> matrix(size_, std::vector<block>(size_, block()));

    for (size_t it_i = 0; it_i < size_; ++it_i)
    {
        for (size_t it_j = 0; it_j < size_; ++it_j)
        {
            if (i != -1)
                matrix[it_i][it_j] = (*cube_)[i][it_i][it_j];
            else if (j != -1)
                matrix[it_i][it_j] = (*cube_)[it_i][j][it_j];
            else if (k != -1)
                matrix[it_i][it_j] = (*cube_)[it_i][it_j][k];
        }
    }

    return std::move(matrix);
}

void lay_manager::set_lay(std::vector<std::vector<block>>& matrix, int i, int j, int k) {
    for (size_t it_i = 0; it_i < size_; ++it_i)
    {
        for (size_t it_j = 0; it_j < size_; ++it_j)
        {
            if (i != -1)
                (*cube_)[i][it_i][it_j] = matrix[it_i][it_j];
            else if (j != -1)
                (*cube_)[it_i][j][it_j] = matrix[it_i][it_j];
            else if (k != -1)
                (*cube_)[it_i][it_j][k] = matrix[it_i][it_j];
        }
    }
}

cube &lay_manager::get_cube() {
    return *cube_;
}

size_t lay_manager::size() {
    return size_;
}

void lay_manager::rotate(std::string cmd) {
    if (cmd == "U")  this->rotate(-1, -1, 2, 3);   //
    if (cmd == "U'") this->rotate(-1, -1, 2);      //
    if (cmd == "U2")  this->rotate(-1, -1, 2, 2);   //

    if (cmd == "D")  this->rotate(-1, -1, 0);      //
    if (cmd == "D'") this->rotate(-1, -1, 0, 3);   //
    if (cmd == "D2") this->rotate(-1, -1, 0, 2);   //

    if (cmd == "R")  this->rotate(-1, 2, 0);       //
    if (cmd == "R'") this->rotate(-1, 2, 0, 3);    //
    if (cmd == "R2") this->rotate(-1, 2, 0, 2);    //

    if (cmd == "L")  this->rotate(-1, 0, 0, 3);    //
    if (cmd == "L'") this->rotate(-1, 0, 0);       //
    if (cmd == "L2") this->rotate(-1, 0, 0, 2);       //

    if (cmd == "F")  this->rotate(2, -1, -1, 3);   //
    if (cmd == "F'") this->rotate(2, -1, -1);      //
    if (cmd == "F2") this->rotate(2, -1, -1, 2);   //

    if (cmd == "B")  this->rotate(0, -1, -1);            //
    if (cmd == "B'") this->rotate(0, -1, -1, 3);   //
    if (cmd == "B2") this->rotate(0, -1, -1, 2);   //
}

std::vector<std::vector<block>> lay_manager::get_lay(rc_types::edges dir) {
    switch (dir)
    {
        case rc_types::UPP:
            return get_lay(-1, -1, 2);
        case rc_types::DWN:
            return get_lay(-1, -1, 0);
        case rc_types::RGT:
            return get_lay(-1, 2, -1);
        case rc_types::LFT:
            return get_lay(-1, 0, -1);
        case rc_types::FRT:
            return get_lay(2, -1, -1);
        case rc_types::BCK:
            return get_lay(0, -1, -1);
    }
}

std::vector<std::vector<block>> &lay_manager::operator[](size_t k) {
    return (*cube_)[k];
}

void lay_manager::rotate(std::vector<std::string> commands) {
    for (auto& it : commands)
        this->rotate(it);
}
