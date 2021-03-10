#include "Parser.hpp"
#include "Expression.hpp"

AssignmentExpression Parser::start(std::string &line, int &error) {
    std::string line_without_space;
    line_without_space.reserve(line.size());
    for (char c: line){
        if (c != ' ' && c != '\t' && c != '\n'){
            line_without_space += c;
        }
    }
    SetLine(line_without_space);

    VariableExpression* var = nullptr;
    Expression* rhs = nullptr;
    if ((var = variable()) && nextCharIs('=') && (rhs = parseSum()) && isEndReached()){
        error = 0;
        return AssignmentExpression(var, rhs);
    }

    delete var;
    delete rhs;
    error = 1;
    return AssignmentExpression(nullptr, nullptr);
}

Expression* Parser::parseSum() {
    int mark = getPos();
    Expression* lhs = parseProduct();
    Expression* rhs;

    while (lhs){
        if (nextCharIs('+')){
            rhs = parseProduct();
            if (rhs)
                lhs = new ArithmeticExpression('+', lhs, rhs);
            else{
                delete lhs;
                lhs = nullptr;
            }
        }
        else if (nextCharIs('-')){
            rhs = parseProduct();
            if (rhs)
                lhs = new ArithmeticExpression('-', lhs, rhs);
            else{
                delete lhs;
                lhs = nullptr;
            }
        }
        else
            break;
    }

    if (!lhs){
        setPos(mark);
    }
    return lhs;
}

Expression* Parser::parseProduct() {
    int mark = getPos();
    Expression* lhs = factor();
    Expression* rhs;
    while (lhs){
        if (nextCharIs('*')){
            rhs = factor();
            if (rhs)
                lhs = new ArithmeticExpression('*', lhs, rhs);
            else{
                delete lhs;
                lhs = nullptr;
            }
        }
        else if (nextCharIs('/')){
            rhs = factor();
            if (rhs)
                lhs = new ArithmeticExpression('/', lhs, rhs);
            else{
                delete lhs;
                lhs = nullptr;
            }
        }
        else{
            break;
        }
    }
    if (!lhs){
        setPos(mark);
    }
    return lhs;
}

Expression* Parser::factor() {
    Expression* exp = nullptr;
    if ((exp = parentheses()) || (exp = variable()) || (exp = number()));
    return exp;
}

Expression* Parser::parentheses() {
    int mark = getPos();
    Expression* exp = nullptr;
    if (nextCharIs('(') && (exp = parseSum()) && (nextCharIs(')')));
    else if (exp){
        delete exp;
        exp = nullptr;
        setPos(mark);
    }
    return exp;
}

NumberExpression* Parser::number() {
    char next_char_candidate = buffer.peek();
    if (std::isdigit(next_char_candidate)){
        double value;
        buffer >> value;
        return new NumberExpression(value);
    }
    return nullptr;
}

VariableExpression* Parser::variable() {
    char next_char_candidate = buffer.peek();
    std::string variable_name;
    while (std::isalpha(next_char_candidate)){
        next_char_candidate = buffer.get();
        variable_name += next_char_candidate;
        next_char_candidate = buffer.peek();
    }
    if (!variable_name.empty()){
        return new VariableExpression(variable_name);
    }
    return nullptr;
}

bool Parser::nextCharIs(char expected) {
    char next_char_candidate = buffer.peek();
    if (next_char_candidate == expected){
        buffer.get();
        return true;
    }
    return false;
}

int Parser::getPos() {
    return buffer.tellg();
}

bool Parser::isEndReached() {
    return buffer.eof();
}

void Parser::setPos(int pos) {
    buffer.clear();
    buffer.seekg(pos);
}
