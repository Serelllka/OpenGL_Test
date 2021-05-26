//
// Created by vprog on 5/26/2021.
//

#include "parser.h"

parser::parser(const std::string& file_name)
{
    boost::property_tree::read_xml(file_name, ptree_);
}
