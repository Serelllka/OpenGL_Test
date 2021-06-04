#include "algorithm.h"

algorithm::algorithm(lay_manager & manager)
    : manager_(manager)
{}

void algorithm::makeRotation(std::string cmd)
{
    std::string str;
    int i = 0;
    while (i < cmd.size())
    {
        str = "";
        while (i < cmd.size() && cmd[i] != ' ')
        {
            str += cmd[i];
            ++i;
        }
        ++i;
        //std::cout << str << ' ';
        manager_.rotate(str);
        solution_log_.push_back(str);
    }
}

void algorithm::to_bottom_position(block blk)
{
    if (blk[UPP] == BLACK) return;
    std::vector<std::string> commands;
    if (blk[UPP] == WHITE)
    {
        if(blk[FRT] != BLACK)       makeRotation("F F");
        else if(blk[BCK] != BLACK)  makeRotation("B B");
        else if(blk[LFT] != BLACK)  makeRotation("L L");
        else if(blk[RGT] != BLACK)  makeRotation("R R");
        return;
    }
    else if(blk[FRT] != BLACK)  makeRotation("F R' D' R");
    else if(blk[BCK] != BLACK)  makeRotation("B L' D' L");
    else if(blk[LFT] != BLACK)  makeRotation("L F' D' F");
    else if(blk[RGT] != BLACK)  makeRotation("R B' D' B");

    /*
    else if(blk[FRT] != BLACK)  makeRotation("B L U B' U' L'");
    else if(blk[BCK] != BLACK)  makeRotation("F R U F' U' R'");
    else if(blk[LFT] != BLACK)  makeRotation("L F U L' U' F'");
    else if(blk[RGT] != BLACK)  makeRotation("R B U R' U' B'");
    */
}

void algorithm::first_stage()
{
    for (int q = 0; q < 4; ++q)
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                for (int k = 0; k < 3; ++k)
                {
                    if (isEdge(manager_[i][j][k]) &&
                            (hasColors(manager_[i][j][k], WHITE, WHITE, ORANGE)
                        ||  hasColors(manager_[i][j][k], WHITE, WHITE, GREEN)
                        ||  hasColors(manager_[i][j][k], WHITE, WHITE, RED)
                        ||  hasColors(manager_[i][j][k], WHITE, WHITE, BLUE))
                        && (!isInUpperCross(manager_[i][j][k])))
                    {
                        //std::cout << i << ' ' << j << ' ' << k << std::endl;
                        to_bottom_position(manager_[i][j][k]);
                        //std::cout << "\n";
                        to_upper_position(manager_[i][j][k]);
                        //std::cout << "\n";
                    }
                }
            }
        }
}

lay_manager &algorithm::manager() {
    return manager_;
}

void algorithm::to_upper_position(block& blka)
{
    std::vector<std::string> commands;
    block* blk = &blka;
    if (blk->edges_[FRT] == WHITE && blk->edges_[RGT] != BLACK)
        makeRotation("R' D R");//
    if (blk->edges_[FRT] != BLACK && blk->edges_[RGT] == WHITE)
        makeRotation("F D F'");//

    if (blk->edges_[FRT] == WHITE && blk->edges_[LFT] != BLACK)
        makeRotation("L D L'");
    if (blk->edges_[FRT] != BLACK && blk->edges_[LFT] == WHITE)
        makeRotation("F' D F");

    if (blk->edges_[BCK] == WHITE && blk->edges_[LFT] != BLACK)
        makeRotation("L' D L");
    if (blk->edges_[BCK] != BLACK && blk->edges_[LFT] == WHITE)
        makeRotation("B D B'");

    if (blk->edges_[BCK] == WHITE && blk->edges_[RGT] != BLACK)
        makeRotation("R D R'");
    if (blk->edges_[BCK] != BLACK && blk->edges_[RGT] == WHITE)
        makeRotation("B' D B");

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if(isEdge(manager_[i][j][0]) &&
                    (hasColors(manager_[i][j][0], WHITE, WHITE, ORANGE)
                 ||  hasColors(manager_[i][j][0], WHITE, WHITE, GREEN)
                 ||  hasColors(manager_[i][j][0], WHITE, WHITE, RED)
                 ||  hasColors(manager_[i][j][0], WHITE, WHITE, BLUE))) blk = &manager_[i][j][0];

    if (blk->edges_[DWN] != WHITE)
    {
        if (blk->edges_[FRT] != BLACK)
            makeRotation("D F' R F R'");
        if (blk->edges_[BCK] != BLACK)
            makeRotation("D B' L B L'");
        if (blk->edges_[RGT] != BLACK)
            makeRotation("D R' B R B'");
        if (blk->edges_[LFT] != BLACK)
            makeRotation("D L' F L F'");
    }

    int cp, lp, count;
    for (int i = 0; i < blk->edges_.size(); ++i)
        if (blk->edges_[i] != BLACK && blk->edges_[i] != WHITE)
        {
            cp = i;
            lp = blk->edges_[i];
        }
    to_index(cp);
    to_index(lp);
    count = lp - cp;
    if (count < 0) count += 4;
    for (int i = 0; i < count; ++i) makeRotation("D");

    switch(lp)
    {
        case 0:
            makeRotation("F F");
            break;
        case 1:
            makeRotation("R R");
            break;
        case 2:
            makeRotation("B B");
            break;
        case 3:
            makeRotation("L L");
            break;
    }
}

void algorithm::second_stage()
{
    for (int q = 0; q < 4; ++q)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    if (hasColors(manager_[i][j][k], BLUE, ORANGE, WHITE)
                        || hasColors(manager_[i][j][k], ORANGE, GREEN, WHITE)
                        || hasColors(manager_[i][j][k], GREEN, RED, WHITE)
                        || hasColors(manager_[i][j][k], RED, BLUE, WHITE))
                        to_corner_position(manager_, manager_[i][j][k]);
    }
}

void algorithm::to_corner_position(lay_manager& manager, block& blk)
{
    if (second_stage_checker(blk)) return;
    std::vector<std::string> commands;
    if (blk[UPP] != BLACK && blk[BCK] != BLACK && blk[LFT] != BLACK)
        makeRotation("L' D' L");

    if (blk[UPP] != BLACK && blk[FRT] != BLACK && blk[LFT] != BLACK)
        makeRotation("L D L'");

    if (blk[UPP] != BLACK && blk[FRT] != BLACK && blk[RGT] != BLACK)
        makeRotation("R' D' R");

    if (blk[UPP] != BLACK && blk[BCK] != BLACK && blk[RGT] != BLACK)
        makeRotation("B' D' B");

    int type = 0, count = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (hasColors(manager[i][j][0], BLUE, ORANGE, WHITE))
            {
                type = 1;
                if (i == 0 && j == 0) count = 0;
                if (i == 2 && j == 0) count = 1;
                if (i == 2 && j == 2) count = 2;
                if (i == 0 && j == 2) count = 3;
            }
            else if (hasColors(manager[i][j][0], ORANGE, GREEN, WHITE))
            {
                type = 2;
                if (i == 0 && j == 0) count = 3;
                if (i == 2 && j == 0) count = 0;
                if (i == 2 && j == 2) count = 1;
                if (i == 0 && j == 2) count = 2;
            }
            else if (hasColors(manager[i][j][0], GREEN, RED, WHITE))
            {
                type = 3;
                if (i == 0 && j == 0) count = 2;
                if (i == 2 && j == 0) count = 3;
                if (i == 2 && j == 2) count = 0;
                if (i == 0 && j == 2) count = 1;
            }
            else if (hasColors(manager[i][j][0], RED, BLUE, WHITE))
            {
                type = 4;
                if (i == 0 && j == 0) count = 1;
                if (i == 2 && j == 0) count = 2;
                if (i == 2 && j == 2) count = 3;
                if (i == 0 && j == 2) count = 0;
            }
            if (type) break;
        }
        if (type) break;
    }
    //00 BLUE ORANGE WHITE 1
    //20 ORANGE GREEN WHITE 2
    //22 GREEN RED WHITE 3
    //02 RED BLUE WHITE 4
    for (int i = 0; i < count; ++i)
        makeRotation("D'");

    switch (type)
    {
        case 1:
            while (manager[0][0][0][BCK] != WHITE && manager[0][0][0][LFT] != ORANGE)
                makeRotation("D' L' D D L");
            makeRotation("D' L' D L ");
            break;
        case 2:
            while (manager[2][0][0][LFT] != ORANGE && manager[2][0][0][FRT] != WHITE)
                makeRotation("F' D' D' F D");
            makeRotation("F' D' F");
            break;
        case 3:
            while (manager[2][2][0][FRT] != WHITE && manager[2][2][0][RGT] != RED)
                makeRotation("R' D' D' R D");
            makeRotation("D' R' D R");
            break;
        case 4:
            while (manager[0][2][0][BCK] != BLUE && manager[2][2][0][RGT] != WHITE)
                makeRotation("B' D' D' B D");
            makeRotation("D' B' D B");
            break;
    }
}

bool algorithm::second_stage_checker(block & blk)
{
    if (blk[UPP] != WHITE) return false;
    if (blk[FRT] != GREEN   && blk[FRT] != BLACK) return false;
    if (blk[LFT] != ORANGE  && blk[LFT] != BLACK) return false;
    if (blk[BCK] != BLUE    && blk[BCK] != BLACK) return false;
    if (blk[RGT] != RED     && blk[RGT] != BLACK) return false;
    return true;
}

void algorithm::third_stage()
{
    std::vector<std::string> commands;
    std::vector<std::string> front_left = {"L'","U","L","U","F","U'","F'"};
    std::vector<std::string> front_right = {"R","U'","R'","U'","F'","U","F"};

    std::vector<std::string> right_back = {"R'","U","R","U","B","U'","B'"};
    std::vector<std::string> back_left = {"L","U'","L'","U'","B'","U","B"};

    std::vector<std::string> blue_orange = {"D'","L'","D","L","D","B","D'", "B'"};
    std::vector<std::string> blue_red = {"D","R","D'","R'","D'","B'","D", "B"};

    std::vector<std::string> red_blue = {"D'","B'","D","B","D","R","D'", "R'"};
    std::vector<std::string> red_green = {"D","F","D'","F'","D'","R'","D", "R"};

    std::vector<std::string> green_red = {"D'","R'","D","R","D","F","D'","F'"};
    std::vector<std::string> green_orange = {"D","L","D'","L'","D'","F'","D","F"};

    std::vector<std::string> orange_green = {"D'","F'","D","F","D","L","D'","L'"};
    std::vector<std::string> orange_blue = {"D","B","D'","B'","D'","L'","D","L"};

    for (int p = 0; p < 4; ++p)
    {
        int k = 0;
        while (!((manager_[0][1][0][BCK] == BLUE && manager_[0][1][0][DWN] != YELLOW)
                 || (manager_[1][0][0][LFT] == ORANGE && manager_[1][0][0][DWN] != YELLOW)
                 || (manager_[2][1][0][FRT] == GREEN && manager_[2][1][0][DWN] != YELLOW)
                 || (manager_[1][2][0][RGT] == RGT && manager_[1][2][0][DWN] != YELLOW))) {
            makeRotation("D");
            ++k;
            if (k == 4 && !toBottom(manager_)) {return;}
        }
        if (manager_[1][0][0][LFT] == ORANGE) {
            if (manager_[1][0][0][DWN] == GREEN) {
                for (auto &it : orange_green)
                    makeRotation(it);
            }
            else if (manager_[1][0][0][DWN] == BLUE) {
                for (auto &it : orange_blue)
                    makeRotation(it);
            }
        }
        if (manager_[0][1][0][BCK] == BLUE) {
            if (manager_[0][1][0][DWN] == ORANGE) {
                for (auto &it : blue_orange)
                    makeRotation(it);
            }
            else if (manager_[0][1][0][DWN] == RED) {
                for (auto &it : blue_red)
                    makeRotation(it);
            }
        }
        if (manager_[1][2][0][RGT] == RED) {
            if (manager_[1][2][0][DWN] == BLUE) {
                for (auto &it : red_blue)
                    makeRotation(it);
            }
            else if (manager_[1][2][0][DWN] == GREEN) {
                for (auto &it : red_green)
                    makeRotation(it);
            }
        }
        if (manager_[2][1][0][FRT] == GREEN) {
            if (manager_[2][1][0][DWN] == RED) {
                for (auto &it : green_red)
                    makeRotation(it);
            }
            else if (manager_[2][1][0][DWN] == ORANGE) {
                for (auto &it : green_orange)
                    makeRotation(it);
            }
        }
    }
}

bool algorithm::toBottom(lay_manager& manager)
{
    std::vector<std::string> green_orange = {"D","L","D'","L'","D'","F'","D","F"};
    std::vector<std::string> green_red = {"D'","R'","D","R","D","F","D'","F'"};
    std::vector<std::string> blue_orange = {"D'","L'","D","L","D","B","D'", "B'"};
    std::vector<std::string> blue_red = {"D","R","D'","R'","D'","B'","D", "B"};


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (isEdge(manager[i][j][1]) && third_stage_lay_checker(manager[i][j][1])
                && !third_stage_checker(manager[i][j][1])) {
                if (i == 2 && j == 0) {
                    for (auto &it : green_orange)
                        makeRotation(it);
                    return true;
                } else if (i == 2 && j == 2) {
                    for (auto &it : green_red)
                        makeRotation(it);
                    return true;
                } else if (i == 0 && j == 2) {
                    for (auto &it : blue_red)
                        makeRotation(it);
                    return true;
                } else if (i == 0 && j == 0) {
                    for (auto &it : blue_orange)
                        makeRotation(it);
                    return true;
                }
            }
        }
    }
    return false;
}

bool algorithm::third_stage_checker(block& blk)
{
    if (blk[BCK] == BLUE    && blk[LFT] == ORANGE) return true;
    if (blk[BCK] == BLUE    && blk[RGT] == RED)    return true;
    if (blk[FRT] == GREEN   && blk[RGT] == RED)    return true;
    if (blk[FRT] == GREEN   && blk[LFT] == ORANGE) return true;
    return false;
}

bool algorithm::third_stage_lay_checker(block& blk)
{
    return hasColors(blk, GREEN, RED, RED) || hasColors(blk, BLUE, RED, RED)
           || hasColors(blk, GREEN, ORANGE, ORANGE) || hasColors(blk, BLUE, ORANGE, ORANGE);
}

void algorithm::fourth_stage()
{
    std::vector<std::string> rotatinos = {"F", "L", "D", "L'", "D'", "F'"};
    if (has_a_cross()) return;
    for (int i = 0; i < 4; ++i)
    {
        if (has_a_corner() || has_a_line()) break;
        makeRotation("D");
    }
    if (!has_a_corner() && !has_a_line()) for(auto& it : rotatinos) manager_.rotate(it);
    if (has_a_corner())
        for(auto& it : rotatinos)
            makeRotation(it);
    for(auto& it : rotatinos)
    {
        makeRotation(it);
    }
}

bool algorithm::has_a_cross()
{
    if (manager_[0][1][0][DWN] != YELLOW) return false;
    if (manager_[1][0][0][DWN] != YELLOW) return false;
    if (manager_[2][1][0][DWN] != YELLOW) return false;
    if (manager_[1][2][0][DWN] != YELLOW) return false;
    return true;
}

bool algorithm::has_a_corner()
{
    /*
    if (manager_[0][1][0][DWN] != YELLOW) return false;
    if (manager_[1][0][0][DWN] != YELLOW) return false;
    if (manager_[2][1][0][DWN] != YELLOW) return false;
    if (manager_[1][2][0][DWN] != YELLOW) return false;
     */
    if (manager_[0][1][0][DWN] != YELLOW) return false;
    if (manager_[1][2][0][DWN] != YELLOW) return false;
    if (manager_[1][1][0][DWN] != YELLOW) return false;
    return true;
}

bool algorithm::has_a_line()
{
    if (manager_[1][0][0][DWN] != YELLOW) return false;
    if (manager_[1][1][0][DWN] != YELLOW) return false;
    if (manager_[1][2][0][DWN] != YELLOW) return false;
    return true;
}

void algorithm::fifth_stage()
{
    std::vector<std::string> right_back = {"F", "D", "F'", "D", "F", "D", "D", "F'", "D"};
    std::vector<std::string> back_left  = {"R", "D", "R'", "D", "R", "D", "D", "R'", "D"};
    while (manager_[2][1][0][FRT] != GREEN)
    {
        makeRotation("D");
    }
    if (manager_[1][2][0][RGT] != RED)
    {
        for (auto &it : right_back) {
            makeRotation(it);
        }
        if (manager_[1][2][0][RGT] != RED) {
            for (auto &it : back_left)
                makeRotation(it);
            for (auto &it : right_back)
                makeRotation(it);
        }
    }
    if (manager_[0][1][0][BCK] != BLUE)
    {
        for (auto &it : back_left)
            makeRotation(it);
    }
}

void algorithm::sixth_stage()
{
    std::vector<std::string> right_front = {"D", "F", "D'", "B'", "D", "F'", "D'", "B"};
    std::vector<std::string> back_left  = {"D", "R", "D'", "L'", "D", "R'", "D'", "L"};
    while (!hasColors(manager_[0][2][0], BLUE, RED, YELLOW))
    {
        if (hasColors(manager_[2][2][0], BLUE, RED, YELLOW))
            for (auto& it : back_left)
                makeRotation(it);
        for (auto& it : right_front)
            makeRotation(it);
    }
    int count = 0;
    while (!hasColors(manager_[0][0][0], BLUE, ORANGE, YELLOW))
    {
        for (auto& it : back_left)
            makeRotation(it);
        if (++count > 500) return;
    }
}

void algorithm::seventh_stage()
    {
        std::vector<std::string> right_front = {"B'", "U'", "B", "U"};
        for (int i = 0; i < 4; ++i)
        {
            while (manager_[0][0][0][DWN] != YELLOW)
            {
                for (auto& it : right_front)
                {
                    makeRotation(it);
                }
            }
            makeRotation("D'");
        }
    }

std::vector<std::string>& algorithm::log() {
    return solution_log_;
}

void algorithm::solution() {
    //std::cout << "\nfirst stage:\n";
    this->first_stage();
    //std::cout << "\nsecond stage:\n";
    this->second_stage();
    //std::cout << "\nthird stage:\n";
    this->third_stage();
    //std::cout << "\nfourth stage:\n";
    this->fourth_stage();
    //std::cout << "\nfifth stage:\n";
    this->fifth_stage();
    //std::cout << "\nsixth stage:\n";
    this->sixth_stage();
    //std::cout << "\nseventh stage:\n";
    this->seventh_stage();
}

bool algorithm::is_solved()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if (manager_[i][j][2][UPP] != rc_types::WHITE)  return false;
            if (manager_[i][j][0][DWN] != rc_types::YELLOW) return false;
            if (manager_[i][2][j][RGT] != rc_types::RED)    return false;
            if (manager_[i][0][j][LFT] != rc_types::ORANGE) return false;
            if (manager_[2][i][j][FRT] != rc_types::GREEN)  return false;
            if (manager_[0][j][j][BCK] != rc_types::BLUE)   return false;
        }
    }
    return true;
}
