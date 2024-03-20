#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "json_parser.hpp"
#include "json_read.hpp"

int main() {
    std::cout << std::boolalpha;

    std::string str1 = json_read("../doc/test.json");

    std::string str2 = R"delimiter(
{
    "a":{
        "a":"a",
        "1":true,
        "str":114.514
    },
    "b":"b",
    "c":123,
    "d":[
        "1",
        "2",
        "3"
    ]
}
    )delimiter";

    DynamicJsonDocument D1(str1);

    std::cout << D1["result"]["future"][2]["wid"]["day"].as<std::string>() << std::endl;
    D1["result"]["future"][2]["wid"]["day"].as<std::string>() = "11";
    std::cout << D1["result"]["future"][2]["wid"]["day"].as<std::string>() << std::endl;


    DynamicJsonDocument D2(str2);

    std::cout << D2["a"]["str"].as<double>() << std::endl;

    return 0;
}