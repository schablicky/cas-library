#ifndef CAS_LIB_H
#define CAS_LIB_H

#include <string>
#include <memory>


class Node {
public:
    virtual ~Node() = default;
    virtual std::string toString() const = 0; 
};


class NumberNode : public Node {
    double value;
public:
    NumberNode(double val);
    std::string toString() const override;
};


class VariableNode : public Node {
    std::string name;
public:
    VariableNode(std::string n);
    std::string toString() const override;
};


class OperatorNode : public Node {
    std::string operatir;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
public:
    OperatorNode(char oper, std::unique_ptr<Node> l, std::unique_ptr<Node> r);
    std::string toString() const override;
};


class Expression {
private:
    std::unique_ptr<Node> root;
    std::unique_ptr<Node> parseText(const std::string& text);

public:
    Expression(const std::string& inputText);
    std::string toString() const;
    void simplify();
};

#endif