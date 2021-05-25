#include "block.h"

int8_t &block::operator[](rc_types::edges edge) {
        return edges_[edge];
}

void block::rotate(rc_types::dimensions r_dimension) {
    switch (r_dimension)
    {
        case rc_types::X:
            //down_, right_, up_, left_
            std::swap(edges_[rc_types::DWN], edges_[rc_types::RGT]);
            std::swap(edges_[rc_types::DWN], edges_[rc_types::UPP]);
            std::swap(edges_[rc_types::DWN], edges_[rc_types::LFT]);
            break;
        case rc_types::Y://change
            std::swap(edges_[rc_types::BCK], edges_[rc_types::DWN]);
            std::swap(edges_[rc_types::BCK], edges_[rc_types::FRT]);
            std::swap(edges_[rc_types::BCK], edges_[rc_types::UPP]);
            break;
        case rc_types::Z:
            std::swap(edges_[rc_types::FRT], edges_[rc_types::RGT]);
            std::swap(edges_[rc_types::FRT], edges_[rc_types::BCK]);
            std::swap(edges_[rc_types::FRT], edges_[rc_types::LFT]);
            break;
    }
}
