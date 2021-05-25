#include "helper_functions.h"

std::string helper_function::clockwise_direction(std::string& item)
{
    if (item[0] == 'F') return "F";
    if (item[0] == 'R') return "R";
    if (item[0] == 'U') return "U";
    if (item[0] == 'B') return "B";
    if (item[0] == 'L') return "L";
    if (item[0] == 'D') return "D";
    return "";
}

std::string helper_function::opposite_direction(std::string& item)
{
    if (item == "F") return "F'";
    if (item == "F'") return "F";
    if (item == "R") return "R'";
    if (item == "R'") return "R";
    if (item == "U") return "U'";
    if (item == "U'") return "U";
    if (item == "B") return "B'";
    if (item == "B'") return "B";
    if (item == "L") return "L'";
    if (item == "L'") return "L";
    if (item == "D") return "D'";
    if (item == "D'") return "D";
}

void helper_function::formatting(std::vector<std::string>& commands)
{
    size_t n = commands.size(), index = 0;
    std::vector<std::string> copy;

    while(index < n)
    {
        std::string sample = commands[index];
        int count = 0;
        while(index < n && commands[index] == sample)
        {
            ++count;
            ++index;
        }
        count %= 4;
        switch (count)
        {
            case 1:
                copy.push_back(sample);
                break;
            case 2:
                copy.push_back(clockwise_direction(sample) + "2");
                break;
            case 3:
                copy.push_back(opposite_direction(sample));
                break;
        }
    }
    std::swap(copy, commands);
}

int helper_function::countOfColoredEdges(block& blk)
{
    int count = 0;
    for (auto& it : blk.edges_)
        if (it != rc_types::BLACK) ++count;
    return count;
}


bool helper_function::isEdge (block& blk)
{
    return countOfColoredEdges(blk) == 2;
}

bool helper_function::hasTileSpecifiedColor(block& blk, rc_types::colors color)
{
    for (auto& it : blk.edges_)
        if (it == color) return true;
    return false;
}

bool helper_function::isInUpperCross(block& blk)
{
    if (blk[rc_types::UPP] != rc_types::WHITE) return false;
    if (blk[rc_types::RGT] != rc_types::RED
        && blk[rc_types::FRT] != rc_types::GREEN
        && blk[rc_types::LFT] != rc_types::ORANGE
        && blk[rc_types::BCK] != rc_types::BLUE) return false;
    return true;
}

void helper_function::to_index(int& ind)
{
    switch(ind)
    {
        case 4:
            ind = 0;
            break;
        case 2:
            ind = 1;
            break;
        case 5:
            ind = 2;
            break;
    }
}

bool helper_function::hasColors (block& blk, rc_types::colors clr1, rc_types::colors clr2, rc_types::colors clr3)
{
    bool f1 = false, f2 = false, f3 = false;
    for (auto& it : blk.edges_)
    {
        if (it == clr1) f1 = true;
        if (it == clr2) f2 = true;
        if (it == clr3) f3 = true;
    }
    return (f1 && f2 && f3);
}

