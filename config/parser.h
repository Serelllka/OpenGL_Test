#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

class parser
{
public:
    explicit parser(const std::string&);

    template<class T>
       void read_param(T& param, const std::string& param_name)
    {
           param = ptree_.get<T>(param_name);
    }

private:
    boost::property_tree::ptree ptree_;
    std::ofstream ostream_;
    std::ifstream istream_;
};
