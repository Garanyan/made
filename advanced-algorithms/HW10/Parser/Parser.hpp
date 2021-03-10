#pragma once

#include <iostream>
#include <sstream>
#include "Expression.hpp"

class Expression;

class Parser{
public:
    AssignmentExpression start(std::string &line, int &error);

private:
    Expression* parseSum();

    Expression* parseProduct();

    Expression* factor();

    Expression* parentheses();

    void SetLine(std::string &line) { buffer = std::stringstream(line); };

    NumberExpression* number();

    VariableExpression* variable();

    bool nextCharIs(char expected);

    bool isEndReached();

    int getPos();

    void setPos(int pos);

    std::stringstream buffer;
};
