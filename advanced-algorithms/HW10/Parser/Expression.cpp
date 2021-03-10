#include <iostream>
#include "Expression.hpp"

std::map<std::string, double> Expression::vars;

double Expression::getVariable(const std::string &var, int &error) {
    if (vars.find(var) != vars.end()){
        return vars[var];
    }
    else{
        error = 1;
        std::cout << "Variable \'" << var << "\' not defined" << std::endl;
    }
    return 0;
}

double VariableExpression::evaluate(int &error) {
    return Expression::getVariable(variable, error);
}

ArithmeticExpression::ArithmeticExpression(char ch, Expression* l, Expression* r)
        : left(l), right(r), op(ch) {
}

ArithmeticExpression::~ArithmeticExpression() {
    delete left;
    delete right;
}

double ArithmeticExpression::evaluate(int &error) {
    if (!left || !right){
        return 0;
    }

    double result = 0;
    auto a = left->evaluate(error);
    auto b = right->evaluate(error);
    switch (op){
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b != 0){
                result = a / b;
            }
            else{
                std::cout << "Error division by zero!" << std::endl;
                error = 1;
            }
            break;
    }
    return result;
}

AssignmentExpression::AssignmentExpression(VariableExpression* v, Expression* val)
        : var(v), value(val) {
}

AssignmentExpression::~AssignmentExpression() {
    delete var;
    delete value;
}

double AssignmentExpression::evaluate(int &error) {
    double x = 0;
    if (value){
        x = value->evaluate(error);
    }
    if (var && error == 0){
        Expression::record(var->getVar(), x);
    }
    return x;
}
