#pragma once

#include <string>
#include <memory>
#include <map>
#include <complex>

template<typename T>
class Expression {
public:
    // Конструкторы
    Expression(T value);
    Expression(const std::string& variable);
    Expression(const Expression& other);
    Expression& operator=(const Expression& other);
    Expression(Expression&& other) noexcept = default;
    Expression& operator=(Expression&& other) noexcept = default;
    ~Expression() = default;

    // Арифметические операторы
    Expression operator+(const Expression& rhs) const;
    Expression operator-(const Expression& rhs) const;
    Expression operator*(const Expression& rhs) const;
    Expression operator/(const Expression& rhs) const;
    Expression operator^(const Expression& rhs) const;

    // Математические функции
    static Expression sin(const Expression& expr);
    static Expression cos(const Expression& expr);
    static Expression ln(const Expression& expr);
    static Expression exp(const Expression& expr);

    // Преобразование к строке
    std::string toString() const;

    // Подстановка переменной, вычисление, дифференцирование
    Expression<T> substitute_all(const std::map<std::string, T>& vars) const;
    Expression substitute(const std::string& var, const T& value) const;
    T evaluate(const std::map<std::string, T>& vars) const;
    Expression differentiate(const std::string& variable) const;

private:
    struct Impl {
        std::string expr;
        Impl(const std::string& s) : expr(s) {}
    };
    std::unique_ptr<Impl> pImpl;
};

// Парсер выражений из строки
template<typename T>
Expression<T> parseExpression(const std::string& input);
