#include "../include/Expression.hpp"
#include <iostream>
#include <cassert>

int test_count = 0;
int passed_count = 0;

void check(const std::string& label, const std::string& actual, const std::string& expected) {
    ++test_count;
    if (actual == expected) {
        ++passed_count;
        std::cout << "[OK]   " << label << "\n";
    } else {
        std::cout << "[FAIL] " << label << "\n";
        std::cout << "       Expected: " << expected << "\n";
        std::cout << "       Got:      " << actual << "\n";
    }
}

void check(const std::string& label, double actual, double expected, double eps = 1e-6) {
    ++test_count;
    if (std::abs(actual - expected) < eps) {
        ++passed_count;
        std::cout << "[OK]   " << label << "\n";
    } else {
        std::cout << "[FAIL] " << label << "\n";
        std::cout << "       Expected: " << expected << ", Got: " << actual << "\n";
    }
}

int main() {
    using E = Expression<double>;

    // Базовые выражения
    E num(3.0);
    E var("x");
    check("Constant toString", num.toString(), "3");
    check("Variable toString", var.toString(), "x");

    // Арифметика
    check("Addition", (num + var).toString(), "(3 + x)");
    check("Multiplication", (var * var).toString(), "(x * x)");

    // Функции
    check("sin(x)", E::sin(var).toString(), "sin(x)");
    check("ln(x)", E::ln(var).toString(), "ln(x)");

    // Подстановка
    E expr = parseExpression<double>("x + 2");
    E substituted = expr.substitute("x", 5.0);
    check("Substitute x=5 in (x + 2)", substituted.toString(), "(5 + 2)");

    // Вычисление
    std::map<std::string, double> vars = {{"x", 10}, {"y", 2}};
    E expr2 = parseExpression<double>("x * y + 1");
    check("Evaluate x=10, y=2 for x*y+1", expr2.evaluate(vars), 21.0);

    // Дифференцирование
    E expr3 = parseExpression<double>("x * sin(x)");
    E deriv = expr3.differentiate("x");
    check("Differentiate x*sin(x)", deriv.toString(), "x * cos(x) + sin(x)");

    std::cout << "\nPassed " << passed_count << " of " << test_count << " tests.\n";
    return 0;
}
