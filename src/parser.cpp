#include "../include/Expression.hpp"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <memory>
#include <map>

template<typename T>
class Parser {
public:
    explicit Parser(const std::string& input) : input_(input), pos_(0) {}

    Expression<T> parse() {
        Expression<T> result = parseExpression();
        skipWhitespace();
        if (pos_ != input_.size()) {
            throw std::runtime_error("Unexpected characters at end of expression.");
        }
        return result;
    }

private:
    std::string input_;
    size_t pos_;

    void skipWhitespace() {
        while (pos_ < input_.size() && isspace(input_[pos_])) ++pos_;
    }

    char peek() {
        skipWhitespace();
        return pos_ < input_.size() ? input_[pos_] : '\0';
    }

    char get() {
        skipWhitespace();
        return pos_ < input_.size() ? input_[pos_++] : '\0';
    }

    bool match(char expected) {
        if (peek() == expected) {
            ++pos_;
            return true;
        }
        return false;
    }

    Expression<T> parseExpression() {
        Expression<T> lhs = parseTerm();
        while (true) {
            char op = peek();
            if (op == '+') {
                get();
                lhs = lhs + parseTerm();
            } else if (op == '-') {
                get();
                lhs = lhs - parseTerm();
            } else break;
        }
        return lhs;
    }

    Expression<T> parseTerm() {
        Expression<T> lhs = parseFactor();
        while (true) {
            char op = peek();
            if (op == '*') {
                get();
                lhs = lhs * parseFactor();
            } else if (op == '/') {
                get();
                lhs = lhs / parseFactor();
            } else break;
        }
        return lhs;
    }

    Expression<T> parseFactor() {
        Expression<T> base = parsePrimary();
        while (peek() == '^') {
            get();
            Expression<T> exponent = parsePrimary();
            base = base ^ exponent;
        }
        return base;
    }

    Expression<T> parsePrimary() {
        if (match('(')) {
            Expression<T> expr = parseExpression();
            if (!match(')')) throw std::runtime_error("Expected closing ')'");
            return expr;
        }

        if (isalpha(peek())) {
            std::string id = parseIdentifier();

            if (match('(')) {
                Expression<T> arg = parseExpression();
                if (!match(')')) throw std::runtime_error("Expected closing ')' in function call");

                if (id == "sin") return Expression<T>::sin(arg);
                if (id == "cos") return Expression<T>::cos(arg);
                if (id == "ln")  return Expression<T>::ln(arg);
                if (id == "exp") return Expression<T>::exp(arg);

                throw std::runtime_error("Unknown function: " + id);
            }

            return Expression<T>(id); // variable
        }

        return parseNumber();
    }

    std::string parseIdentifier() {
        std::string id;
        while (std::isalnum(peek()) || peek() == '_') {
            id += get();
        }
        return id;
    }

    Expression<T> parseNumber() {
        std::string num_str;
        while (std::isdigit(peek()) || peek() == '.' || peek() == '-') {
            num_str += get();
        }
        std::istringstream iss(num_str);
        T value;
        iss >> value;
        if (iss.fail()) throw std::runtime_error("Invalid number: " + num_str);
        return Expression<T>(value);
    }
};

// Функция для использования извне
template<typename T>
Expression<T> parseExpression(const std::string& input) {
    Parser<T> parser(input);
    return parser.parse();
}

// Явные инстанцирования
template Expression<double> parseExpression(const std::string&);
template Expression<std::complex<double>> parseExpression(const std::string&);
