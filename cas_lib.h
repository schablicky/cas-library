/**
 * @author Tomas Schablicky
 * @date 2026-04-30
 * @brief Header file for the Computer Algebra System (CAS) library.
 * @details This file defines the core classes and interfaces for the CAS library, including the Node hierarchy for representing mathematical expressions and the Expression class for parsing and simplifying expressions.
 */


#ifndef CAS_LIB_H
#define CAS_LIB_H

#include <string>
#include <memory>


/**
 * @class Node
 * @brief Base class for all nodes in the expression tree.
 * @details This class is an abstract base class that defines the interface for all nodes in the expression tree. It includes a virtual method to convert the node to a string representation.
 */
class Node {
public:
    virtual ~Node() = default;
    virtual std::string toString() const = 0; 
};


/**
 * @class NumberNode
 * @brief Represents a numeric constant in the expression tree.
 * @details This class inherits from Node and represents a numeric constant. It stores a double value and provides a method to convert it to a string representation.
 */
class NumberNode : public Node {
    double value;
public:
    NumberNode(double val);
    std::string toString() const override;
};


/**
 * @class VariableNode
 * @brief Represents a variable in the expression tree.
 * @details This class inherits from Node and represents a variable. It stores a string name and provides a method to convert it to a string representation.
 */
class VariableNode : public Node {
    std::string name;
public:
    VariableNode(std::string n);
    std::string toString() const override;
};


/**
 * @class OperatorNode
 * @brief Represents an operator in the expression tree.
 * @details This class inherits from Node and represents an operator. It stores a character for the operator and two unique pointers to child nodes.
 */
class OperatorNode : public Node {
    char op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
public:
    OperatorNode(char oper, std::unique_ptr<Node> l, std::unique_ptr<Node> r);
    std::string toString() const override;
};

/**
 * @class Expression
 * @brief Represents a mathematical expression.
 * @details This class provides methods to parse a string representation of a mathematical expression into an expression tree, convert the expression to a string, and simplify the expression. The parsing method uses a shunting-yard algorithm to handle operator precedence and parentheses.
 */
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
