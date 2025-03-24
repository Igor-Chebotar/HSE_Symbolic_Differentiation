#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "Expression.hpp"

template<typename T>
class Parser {
public:
    explicit Parser(const std::string& input);
    Expression<T> parse();

private:
    std::string input_;
    size_t pos_;

    char peek() const;
    char get();
    void skipWhitespace();

    Expression<T> parseExpression();
    Expression<T> parseTerm();
    Expression<T> parseFactor();
    Expression<T> parsePrimary();
};

#endif // PARSER_HPP
