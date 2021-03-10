#include <cassert>
#include "Parser/Parser.hpp"
#include "Parser/Expression.hpp"

bool testPlus() {
    std::string input_line = "qwe = 40 + 15";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return 55 == static_cast<int>(e.evaluate(error));
}

bool testMinus() {
    std::string input_line = "qwe = 12 - 14";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return -2 == static_cast<int>(e.evaluate(error));
}

bool testMult() {
    std::string input_line = "qwe = 9*10";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return 90 == static_cast<int>(e.evaluate(error));
}

bool testDivision() {
    std::string input_line = "qwe = 45/5";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return 9 == static_cast<int>(e.evaluate(error));
}

bool testReturn() {
    std::string input_line = "qwe = 1000";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return 1000 == static_cast<int>(e.evaluate(error));
}

bool testHistory() {
    std::string input_line = "qwe = 4";
    Parser parser;
    int error = 0;
    auto e_1 = parser.start(input_line, error);
    auto a = static_cast<int>(e_1.evaluate(error));
    assert(a == 4);

    std::string input_line_2 = "rewq = 7 + qwe";
    auto e_2 = parser.start(input_line_2, error);
    return 11 == static_cast<int>(e_2.evaluate(error));
}

bool testDivideByZero() {
    std::string input_line = "qwe = 1000 / 0";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    auto res = e.evaluate(error);
    return error != 0;
}

bool testUnkVar() {
    std::string input_line = "qwe = c";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    auto res = e.evaluate(error);
    return error != 0;
}

bool testSyntax() {
    std::string input_line = "a = = 89+-c";
    Parser parser;
    int error = 0;
    auto e = parser.start(input_line, error);
    return error != 0;
}

bool test() {

    assert(testHistory());

    assert(testReturn());

    assert(testDivision());

    assert(testMult());

    assert(testMinus());

    assert(testPlus());

    assert(testDivideByZero());

    assert(testUnkVar());

    assert(testSyntax());

    return true;
}
