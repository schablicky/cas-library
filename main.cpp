#include <iostream>
#include "cas_lib.h"

int main() {
    Expression expr1("3 + 4 * (2 - 1)");
    Expression expr2("x * y + z");
    Expression expr3("(a + b) * (c - d) / e");

    std::cout << "Parsed expression: " << expr1.toString() << std::endl;
    std::cout << "Parsed expression: " << expr2.toString() << std::endl;
    std::cout << "Parsed expression: " << expr3.toString() << std::endl;

    expr1.simplify();
    expr2.simplify();
    expr3.simplify();

    std::cout << "Simplified expression: " << expr1.toString() << std::endl;
    std::cout << "Simplified expression: " << expr2.toString() << std::endl;
    std::cout << "Simplified expression: " << expr3.toString() << std::endl;
    
    return 0;
}