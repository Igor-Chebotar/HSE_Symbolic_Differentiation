#include "../include/Expression.hpp"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <regex>

// ===== Реализация функций =====

template<typename T>
Expression<T> Expression<T>::sin(const Expression& expr) {
    return Expression("sin(" + expr.toString() + ")");
}

template<typename T>
Expression<T> Expression<T>::cos(const Expression& expr) {
    return Expression("cos(" + expr.toString() + ")");
}

template<typename T>
Expression<T> Expression<T>::ln(const Expression& expr) {
    return Expression("ln(" + expr.toString() + ")");
}

template<typename T>
Expression<T> Expression<T>::exp(const Expression& expr) {
    return Expression("exp(" + expr.toString() + ")");
}

// ===== Подстановка переменной =====

template<typename T>
Expression<T> Expression<T>::substitute(const std::string& var, const T& value) const {
    std::ostringstream oss;
    if constexpr (std::is_same_v<T, std::complex<double>>) {
        oss << "(" << value.real() << "+" << value.imag() << "i)";
    } else {
        oss << value;
    }

    std::string result = pImpl->expr;
    std::regex r("\\b" + var + "\\b");
    result = std::regex_replace(result, r, oss.str());

    return Expression(result);
}

// ===== Простейшее вычисление (для double) =====

template<>
double Expression<double>::evaluate(const std::map<std::string, double>& vars) const {
    std::string expr = pImpl->expr;

    for (const auto& [var, val] : vars) {
        std::ostringstream oss;
        oss << val;
        std::regex r("\\b" + var + "\\b");
        expr = std::regex_replace(expr, r, oss.str());
    }

    try {
        // Удалим скобки
        expr.erase(remove(expr.begin(), expr.end(), '('), expr.end());
        expr.erase(remove(expr.begin(), expr.end(), ')'), expr.end());

        return std::stod(expr);  // Работает ТОЛЬКО если осталось простое число
    } catch (...) {
        throw std::runtime_error("Cannot evaluate expression: " + expr);
    }
}


// ===== Символьная производная (double) =====

template<typename T>
Expression<T> Expression<T>::differentiate(const std::string& var) const {
    const std::string& e = pImpl->expr;

    // Базовые случаи
    if (e == var) return Expression(1.0);
    try {
        std::stod(e);
        return Expression(0.0);
    } catch (...) {}

    // Сумма
    size_t plus = e.find(" + ");
    if (plus != std::string::npos) {
        Expression<T> left(e.substr(0, plus));
        Expression<T> right(e.substr(plus + 3));
        return left.differentiate(var) + right.differentiate(var);
    }

    // Произведение
    size_t mul = e.find(" * ");
    if (mul != std::string::npos) {
        Expression<T> u(e.substr(0, mul));
        Expression<T> v(e.substr(mul + 3));
        return u.differentiate(var) * v + u * v.differentiate(var);
    }

    // sin(...)
    if (e.rfind("sin(", 0) == 0 && e.back() == ')') {
        std::string inner = e.substr(4, e.size() - 5);
        Expression<T> f(inner);
        return Expression<T>::cos(f) * f.differentiate(var);
    }

    // cos(...)
    if (e.rfind("cos(", 0) == 0 && e.back() == ')') {
        std::string inner = e.substr(4, e.size() - 5);
        Expression<T> f(inner);
        return Expression("-sin(" + inner + ")") * f.differentiate(var);
    }

    return Expression("d/d" + var + "(" + e + ")");
}


// ===== Символьная производная (complex<double>) =====

template<>
Expression<std::complex<double>> Expression<std::complex<double>>::differentiate(const std::string& var) const {
    const std::string& e = pImpl->expr;

    if (e == var) return Expression<std::complex<double>>(std::complex<double>(1.0, 0.0));

    try {
        std::stod(e);
        return Expression<std::complex<double>>(std::complex<double>(0.0, 0.0));
    } catch (...) {}

    if (e == "sin(" + var + ")") return Expression<std::complex<double>>("cos(" + var + ")");
    if (e == "cos(" + var + ")") return Expression<std::complex<double>>("-sin(" + var + ")");
    if (e == "ln(" + var + ")")  return Expression<std::complex<double>>("1/(" + var + ")");
    if (e == "exp(" + var + ")") return Expression<std::complex<double>>("exp(" + var + ")");

    std::regex power_expr(var + "\\^(\\d+)");
    std::smatch match;
    if (std::regex_match(e, match, power_expr)) {
        int n = std::stoi(match[1]);
        return Expression<std::complex<double>>(
            std::to_string(n) + " * " + var + "^" + std::to_string(n - 1)
        );
    }

    size_t mul_pos = e.find("*");
    if (mul_pos != std::string::npos) {
        std::string left = e.substr(0, mul_pos);
        std::string right = e.substr(mul_pos + 1);
        Expression<std::complex<double>> left_expr(left);
        Expression<std::complex<double>> right_expr(right);
        return left_expr.differentiate(var) * right_expr + left_expr * right_expr.differentiate(var);
    }

    return Expression<std::complex<double>>("d/d" + var + "(" + e + ")");
}

// ===== Явные инстанцирования =====

// double
template Expression<double> Expression<double>::sin(const Expression<double>&);
template Expression<double> Expression<double>::cos(const Expression<double>&);
template Expression<double> Expression<double>::ln(const Expression<double>&);
template Expression<double> Expression<double>::exp(const Expression<double>&);
template Expression<double> Expression<double>::substitute(const std::string&, const double&) const;
template Expression<double> Expression<double>::differentiate(const std::string&) const;
template double Expression<double>::evaluate(const std::map<std::string, double>&) const;

// complex<double>
template Expression<std::complex<double>> Expression<std::complex<double>>::sin(const Expression<std::complex<double>>&);
template Expression<std::complex<double>> Expression<std::complex<double>>::cos(const Expression<std::complex<double>>&);
template Expression<std::complex<double>> Expression<std::complex<double>>::ln(const Expression<std::complex<double>>&);
template Expression<std::complex<double>> Expression<std::complex<double>>::exp(const Expression<std::complex<double>>&);
template Expression<std::complex<double>> Expression<std::complex<double>>::substitute(const std::string&, const std::complex<double>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::differentiate(const std::string&) const;
