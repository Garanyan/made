#pragma once

#include <map>
#include <string>
#include <utility>

class Expression{
public:
    Expression() = default;

    virtual ~Expression() = default;

    static double getVariable(const std::string &var, int &error);

    static void record(const std::string &var, double value) { vars[var] = value; };

    virtual double evaluate(int &error) = 0;

protected:
    static std::map<std::string, double> vars;
};

class NumberExpression : public Expression{
public:
    explicit NumberExpression(double val) : value(val) {};

    double evaluate(int &error) override { return value; };
private:
    double value;
};

class VariableExpression : public Expression{
public:
    explicit VariableExpression(std::string var) : variable(std::move(var)) {};

    double evaluate(int &error) override;

    std::string getVar() const { return variable; }

private:
    std::string variable;
};

class ArithmeticExpression : public Expression{
public:
    ArithmeticExpression(char op, Expression* left, Expression* right);

    ~ArithmeticExpression() override;

    double evaluate(int &error) override;

private:
    Expression* left, * right;
    char op;
};

class AssignmentExpression : public Expression{
public:
    AssignmentExpression(VariableExpression* var, Expression* value);

    ~AssignmentExpression() override;

    double evaluate(int &error) override;

private:
    VariableExpression* var;
    Expression* value;
};
