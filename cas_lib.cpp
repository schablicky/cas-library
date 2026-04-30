#include "cas_lib.h"


std::string NumberNode::toString() const {
    return std::to_string(value);
}


VariableNode::VariableNode(std::string n) : name(n) {}
std::string VariableNode::toString() const {
    return name;
}


OperatorNode::OperatorNode(char oper, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
    : op(oper), left(std::move(l)), right(std::move(r)) {}


std::string OperatorNode::toString() const {
    return "(" + left->toString() + " " + std::string(1, op) + " " + right->toString() + ")";
}

Expression::Expression(const std::string& inputText) {
    root = parseText(inputText);
}

std::string Expression::toString() const {
    return root ? root->toString() : "";
}

void Expression::simplify() {
    // Placeholder for simplification logic
}

std::unique_ptr<Node> Expression::parseText(const std::string& text) {
    // Placeholder for parsing logic
    return nullptr;
}
