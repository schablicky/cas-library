/**
 * @author Tomas Schablicky
 * @date 2026-04-30
 * @brief Implementation file for the Computer Algebra System (CAS) library.
 * @details This file provides the implementation of the core classes and interfaces defined in cas_lib.h.
 */

#include "cas_lib.h"

#include <cctype>
#include <stdexcept>
#include <vector>


/**
 * @brief Converts a NumberNode to its string representation.
 * @details This method overrides the toString() method from the Node class and returns a string representation of the numeric value stored in the NumberNode.
 * @return A string representation of the numeric value stored in the NumberNode.
 */
NumberNode::NumberNode(double val) : value(val) {}
std::string NumberNode::toString() const {
    return std::to_string(value);
}


/**
 * @brief Constructs a VariableNode with the given name.
 * @details This constructor initializes the name of the variable stored in the VariableNode.
 * @param n The name of the variable to be stored in the VariableNode.
 * @return A VariableNode object initialized with the given name.
 */
VariableNode::VariableNode(std::string n) : name(n) {}
std::string VariableNode::toString() const {
    return name;
}


/**
 * @brief Constructs an OperatorNode with the given operator and child nodes.
 * @details This constructor initializes the operator and child nodes of the OperatorNode.
 * @param oper The operator to be stored in the OperatorNode.
 * @param l A unique pointer to the left child node.
 * @param r A unique pointer to the right child node.
 * @return An OperatorNode object initialized with the given operator and child nodes.
 */
OperatorNode::OperatorNode(char oper, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
    : op(oper), left(std::move(l)), right(std::move(r)) {}


/**
 * @brief Converts an OperatorNode to its string representation.
 * @details This method overrides the toString() method from the Node class and returns a string representation of the operator and its child nodes in the format "(left op right)".
 * @return A string representation of the operator and its child nodes.
 */
std::string OperatorNode::toString() const {
    return "(" + left->toString() + " " + std::string(1, op) + " " + right->toString() + ")";
}


/**
 * @brief Constructs an Expression object by parsing the input text.
 */
Expression::Expression(const std::string& inputText) {
    root = parseText(inputText);
}


/**
 * @brief Converts an Expression to its string representation.
 * @details This method calls the toString() method of the root node of the expression tree to get the string representation of the entire expression.
 * @return A string representation of the expression.
 */
std::string Expression::toString() const {
    return root ? root->toString() : "";
}


/**
 * @brief Simplifies the expression.
 * @todo Implement the simplification function.
 */
void Expression::simplify() {
    // Placeholder for simplification logic
}


/**
 * @brief Parses a string representation of a mathematical expression into an expression tree.
 * @details This method uses a shunting-yard algorithm to handle operator precedence and parentheses. It processes the input string character by character, building a stack of values and operators to construct the expression tree.
 * @param text The string representation of the mathematical expression to be parsed.
 * @return A unique pointer to the root node of the constructed expression tree.
 * @throws std::runtime_error If the input expression is invalid or contains mismatched parentheses.
 */
std::unique_ptr<Node> Expression::parseText(const std::string& text) {
    // Initialize stacks for values and operators
    std::vector<std::unique_ptr<Node>> valueStack;
    std::vector<char> operatorStack;

    // Define operator precedence and helper functions
    // In case of + or -, precedence is 1; in case of * or /, precedence is 2; otherwise, precedence is 0
    // 2 takes precedence over 1, and both take precedence over 0, which means a non-operator character has been encountered
    auto precedence = [](char op) -> int {
        switch (op) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            default:
                return 0;
        }
    };

    // Helper function to check if a character is an operator
    auto isOperator = [](char ch) -> bool {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    };

    // Helper function to apply an operator to the top two values on the value stack
    auto applyOperator = [&valueStack](char op) {
        // Ensure there are at least two values on the stack before applying the operator
        // Won't be in future versions, as the parser will be able to handle unary operators like factorial
        if (valueStack.size() < 2) {
            throw std::runtime_error("Invalid expression");
        }

        // Pop the top two values from the value stack and create a new OperatorNode with the operator and the two values as children
        // As per the shunting-yard algorithm, the right operand is popped first, followed by the left operand
        std::unique_ptr<Node> right = std::move(valueStack.back());
        valueStack.pop_back();
        std::unique_ptr<Node> left = std::move(valueStack.back());
        valueStack.pop_back();

        // Push the new OperatorNode onto the value stack
        valueStack.push_back(std::make_unique<OperatorNode>(op, std::move(left), std::move(right)));
    };

    // Process the input string character by character
    std::size_t i = 0;
    while (i < text.size()) {
        unsigned char current = static_cast<unsigned char>(text[i]);

        // Skip whitespace characters
        if (std::isspace(current)) {
            ++i;
            continue;
        }

        // If the current character is a digit or a decimal point, parse a number and push a NumberNode onto the value stack
        if (std::isdigit(current) || text[i] == '.') {
            std::size_t consumed = 0;
            double value = std::stod(text.substr(i), &consumed);
            valueStack.push_back(std::make_unique<NumberNode>(value));
            i += consumed; // Move the index forward by the number of characters consumed by std::stod
            continue;
        }

        // If the current character is an alphabetic character or an underscore, parse a variable name and push a VariableNode onto the value stack
        if (std::isalpha(current) || text[i] == '_') {
            std::size_t start = i;
            ++i;
            while (i < text.size()) {
                unsigned char next = static_cast<unsigned char>(text[i]);
                // Continue parsing the variable name as long as the next character is alphanumeric or an underscore
                if (!std::isalnum(next) && text[i] != '_') {
                    break;
                }
                ++i;
            }

            valueStack.push_back(std::make_unique<VariableNode>(text.substr(start, i - start)));
            continue;
        }

        // If the current character is an opening parenthesis, push it onto the operator stack
        if (text[i] == '(') {
            operatorStack.push_back(text[i]);
            ++i;
            continue;
        }

        // If the current character is a closing parenthesis, pop operators from the operator stack and apply them until an opening parenthesis is encountered
        if (text[i] == ')') {
            while (!operatorStack.empty() && operatorStack.back() != '(') {
                char op = operatorStack.back();
                operatorStack.pop_back();
                applyOperator(op);
            }

            // If the operator stack is empty or the top of the stack is not an opening parenthesis, the parentheses are mismatched
            if (operatorStack.empty() || operatorStack.back() != '(') {
                throw std::runtime_error("Mismatched parentheses");
            }

            operatorStack.pop_back();
            ++i;
            continue;
        }

        // If the current character is an operator, pop operators from the operator stack and apply them while they have higher or equal precedence than the current operator, then push the current operator onto the operator stack
        if (isOperator(text[i])) {
            char op = text[i];
            while (!operatorStack.empty() && isOperator(operatorStack.back()) &&
                   precedence(operatorStack.back()) >= precedence(op)) {
                char topOp = operatorStack.back();
                operatorStack.pop_back();
                applyOperator(topOp);
            }

            operatorStack.push_back(op);
            ++i;
            continue;
        }

        throw std::runtime_error("Unexpected character in expression");
    }

    // After processing the entire input string, pop any remaining operators from the operator stack and apply them
    while (!operatorStack.empty()) {
        char op = operatorStack.back();
        operatorStack.pop_back();

        if (op == '(' || op == ')') {
            throw std::runtime_error("Mismatched parentheses");
        }

        applyOperator(op);
    }

    // At this point, there should be exactly one value on the value stack, which is the root of the expression tree. If there are more or fewer values, the expression is invalid.
    if (valueStack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return std::move(valueStack.back());
}
