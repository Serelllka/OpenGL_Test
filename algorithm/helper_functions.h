#pragma once
#include<vector>
#include<string>
#include "../cube_logic/lay_manager.h"

namespace helper_function
{

    std::string clockwise_direction(std::string &);

    std::string opposite_direction(std::string &);

    int countOfColoredEdges(block &);
    bool isEdge(block &);

    bool hasColors (block&, rc_types::colors, rc_types::colors, rc_types::colors);
    bool hasTileSpecifiedColor(block &, rc_types::colors);
    bool isInUpperCross(block &);


    void to_index(int &);
    void formatting(std::vector<std::string> &);
}