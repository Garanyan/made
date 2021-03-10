#include <string>
#include <iostream>
#include "Parser/Parser.hpp"

void test();

int main() {

    test();
    std::string str;
    Parser p;
    int error = 0;
    while (std::getline(std::cin, str)){
        auto expr = p.start(str, error);
        if (!error){
            auto result = expr.evaluate(error);
            if (!error){
                std::cout << result << std::endl;
            }
        }
        else{
            std::cout << "Can not parse statement." << std::endl;
        }
    }
    return 0;
}
