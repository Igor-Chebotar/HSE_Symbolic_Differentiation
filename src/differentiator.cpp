#include "../include/Expression.hpp"
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

// Объявление парсера
template<typename T>
Expression<T> parseExpression(const std::string& input);

void print_usage() {
    std::cout << "Usage:\n";
    std::cout << "  --eval \"expression\" var1=val1 var2=val2 ...\n";
    std::cout << "  --diff \"expression\" --by var\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "--eval") {
            if (argc < 3) {
                std::cerr << "Missing expression.\n";
                return 1;
            }

            std::string expr_str = argv[2];
            std::map<std::string, double> vars;

            // Сначала разбираем переменные
            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                size_t eq = arg.find('=');
                if (eq == std::string::npos) {
                    std::cerr << "Invalid variable format: " << arg << "\n";
                    return 1;
                }
                std::string var = arg.substr(0, eq);
                double val = std::stod(arg.substr(eq + 1));
                vars[var] = val;
            }

            // Парсим выражение
            Expression<double> expr = parseExpression<double>(expr_str);

            // Вычисляем с переменными
            double result = expr.evaluate(vars);
            std::cout << result << "\n";
        }
        else if (mode == "--diff") {
            if (argc < 5 || std::string(argv[3]) != "--by") {
                std::cerr << "Usage: --diff \"expr\" --by var\n";
                return 1;
            }

            std::string expr_str = argv[2];
            std::string variable = argv[4];

            Expression<double> expr = parseExpression<double>(expr_str);
            Expression<double> derivative = expr.differentiate(variable);
            std::cout << derivative.toString() << "\n";
        }
        else {
            print_usage();
            return 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
