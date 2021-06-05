#include "input_class.h"

rc_types::colors input_methods::liter_to_color(char c)
{
    switch (c)
    {
        case 'W':
            return rc_types::WHITE;
        case 'Y':
            return rc_types::YELLOW;
        case 'R':
            return rc_types::RED;
        case 'O':
            return rc_types::ORANGE;
        case 'G':
            return rc_types::GREEN;
        case 'B':
            return rc_types::BLUE;
    }
    return BLACK;
}

char input_methods::color_to_liter(rc_types::colors c)
{
    switch (c)
    {
        case rc_types::WHITE:
            return 'W';
        case rc_types::YELLOW:
            return 'Y';
        case rc_types::RED:
            return 'R';
        case rc_types::ORANGE:
            return 'O';
        case rc_types::GREEN:
            return 'G';
        case rc_types::BLUE:
            return 'B';
        case rc_types::BLACK:
            break;
    }
    return 0;
}

void input_methods::generate_cube_from_file(const std::string& file_name, cube& tmp)
{
    std::ifstream ifs;
    ifs.open(file_name.c_str());
    std::string str[3];
    for (int k = 0; k < 6; ++k)
    {
        ifs >> str[0] >> str[1] >> str[2];
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                switch (k)
                {
                    case 0:
                        tmp[i][j][2][rc_types::UPP] = liter_to_color(str[i][j]);
                        break;
                    case 1:
                        tmp[i][j][0][rc_types::DWN] = liter_to_color(str[i][j]);
                        break;
                    case 2:
                        tmp[i][2][j][rc_types::RGT] = liter_to_color(str[i][j]);
                        break;
                    case 3:
                        tmp[i][0][j][rc_types::LFT] = liter_to_color(str[i][j]);
                        break;
                    case 4:
                        tmp[2][i][j][rc_types::FRT] = liter_to_color(str[i][j]);
                        break;
                    case 5:
                        tmp[0][i][j][rc_types::BCK] = liter_to_color(str[i][j]);
                        break;
                }
            }
        }
    }
}

bool input_methods::check_for_correctness(algorithm& tmp)
{
    tmp.solution();
    if (tmp.is_solved()) return (tmp.log().size() < 400);
    return false;
}

void input_methods::save_cube_to_file(const std::string& file_name, cube& tmp)
{
    std::ofstream ofs;
    ofs.open(file_name.c_str());
    std::string str[3];
    for (int k = 0; k < 6; ++k)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                switch (k)
                {
                    case 0:
                        ofs << color_to_liter(static_cast<colors>(tmp[i][j][2][rc_types::UPP]));
                        break;
                    case 1:
                        ofs << color_to_liter(static_cast<colors>(tmp[i][j][0][rc_types::DWN]));
                        break;
                    case 2:
                        ofs << color_to_liter(static_cast<colors>(tmp[i][2][j][rc_types::RGT]));
                        break;
                    case 3:
                        ofs << color_to_liter(static_cast<colors>(tmp[i][0][j][rc_types::LFT]));
                        break;
                    case 4:
                        ofs << color_to_liter(static_cast<colors>(tmp[2][i][j][rc_types::FRT]));
                        break;
                    case 5:
                        ofs << color_to_liter(static_cast<colors>(tmp[0][i][j][rc_types::BCK]));
                        break;
                }
            }
            ofs << '\n';
        }
    }
}

void input_methods::output_to_console(cube & tmp)
{
    for (int k = 0; k < 6; ++k)
    {
        switch (k)
        {
            case 0:
                std::cout << "UPP:\n";
                break;
            case 1:
                std::cout << "DWN:\n";
                break;
            case 2:
                std::cout << "RGT:\n";
                break;
            case 3:
                std::cout << "LFT:\n";
                break;
            case 4:
                std::cout << "FRT:\n";
                break;
            case 5:
                std::cout << "BCK:\n";
                break;
        }
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                switch (k)
                {
                    case 0:
                        std::cout << color_to_liter(static_cast<colors>(tmp[i][j][2][rc_types::UPP]));
                        break;
                    case 1:
                        std::cout << color_to_liter(static_cast<colors>(tmp[i][j][0][rc_types::DWN]));
                        break;
                    case 2:
                        std::cout << color_to_liter(static_cast<colors>(tmp[i][2][j][rc_types::RGT]));
                        break;
                    case 3:
                        std::cout << color_to_liter(static_cast<colors>(tmp[i][0][j][rc_types::LFT]));
                        break;
                    case 4:
                        std::cout << color_to_liter(static_cast<colors>(tmp[2][i][j][rc_types::FRT]));
                        break;
                    case 5:
                        std::cout << color_to_liter(static_cast<colors>(tmp[0][i][j][rc_types::BCK]));
                        break;
                }
            }
            std::cout << '\n';
        }
    }
}
