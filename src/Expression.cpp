#include "../include/Expression.hpp"
#include <utility>
#include <sstream>
#include <type_traits>

// Конструктор из значения
template<typename T>
Expression<T>::Expression(T value) {
    std::ostringstream oss;
    if constexpr (std::is_same_v<T, std::complex<double>>) {
        oss << "(" << value.real() << "+" << value.imag() << "i)";
    } else {
        oss << value;
    }
    pImpl = std::make_unique<Impl>(oss.str());
}

template<typename T>
Expression<T>::Expression(const std::string& variable)
    : pImpl(std::make_unique<Impl>(variable)) {}

template<typename T>
Expression<T>::Expression(const Expression& other)
    : pImpl(std::make_unique<Impl>(other.pImpl->expr)) {}


template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        pImpl = std::make_unique<Impl>(other.pImpl->expr);
    }
    return *this;
}


// Арифметические операторы

template<typename T>
Expression<T> Expression<T>::operator+(const Expression& rhs) const {
    return Expression("(" + pImpl->expr + " + " + rhs.pImpl->expr + ")");
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression& rhs) const {
    return Expression("(" + pImpl->expr + " - " + rhs.pImpl->expr + ")");
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression& rhs) const {
    return Expression("(" + pImpl->expr + " * " + rhs.pImpl->expr + ")");
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression& rhs) const {
    return Expression("(" + pImpl->expr + " / " + rhs.pImpl->expr + ")");
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression& rhs) const {
    return Expression("(" + pImpl->expr + " ^ " + rhs.pImpl->expr + ")");
}

template<typename T>
std::string Expression<T>::toString() const {
    return pImpl->expr;
}
template<typename T>
Expression<T> Expression<T>::substitute(const std::string& var, const T& value) const {
    std::ostringstream oss;
    if constexpr (std::is_same_v<T, std::complex<double>>) {
        oss << "(" << value.real() << "+" << value.imag() << "i)";
    } else {
        oss << value;
    }

    std::string result = pImpl->expr;
    size_t pos = 0;
    while ((pos = result.find(var, pos)) != std::string::npos) {
        result.replace(pos, var.length(), oss.str());
        pos += oss.str().length();
    }

    return Expression(result);
}

template<typename T>
Expression<T> Expression<T>::substitute_all(const std::map<std::string, T>& vars) const {
    Expression<T> result = *this;
    for (const auto& [var, val] : vars) {
        result = result.substitute(var, val);
    }
    return result;
}

// double
template Expression<double>::Expression(double);
template Expression<double>::Expression(const std::string&);
template Expression<double>::Expression(const Expression<double>&);
template Expression<double>& Expression<double>::operator=(const Expression<double>&);

// complex<double>
template Expression<std::complex<double>>::Expression(std::complex<double>);
template Expression<std::complex<double>>::Expression(const std::string&);
template Expression<std::complex<double>>::Expression(const Expression<std::complex<double>>&);
template Expression<std::complex<double>>& Expression<std::complex<double>>::operator=(const Expression<std::complex<double>>&);

// Арифметика double
template Expression<double> Expression<double>::operator+(const Expression<double>&) const;
template Expression<double> Expression<double>::operator-(const Expression<double>&) const;
template Expression<double> Expression<double>::operator*(const Expression<double>&) const;
template Expression<double> Expression<double>::operator/(const Expression<double>&) const;
template Expression<double> Expression<double>::operator^(const Expression<double>&) const;
template std::string Expression<double>::toString() const;

// Арифметика std::complex<double>
template Expression<std::complex<double>> Expression<std::complex<double>>::operator+(const Expression<std::complex<double>>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::operator-(const Expression<std::complex<double>>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::operator*(const Expression<std::complex<double>>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::operator/(const Expression<std::complex<double>>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::operator^(const Expression<std::complex<double>>&) const;
template std::string Expression<std::complex<double>>::toString() const;


// Явные инстанцирования
template Expression<double> Expression<double>::substitute_all(const std::map<std::string, double>&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::substitute_all(const std::map<std::string, std::complex<double>>&) const;

template Expression<double> Expression<double>::substitute(const std::string&, const double&) const;
template Expression<std::complex<double>> Expression<std::complex<double>>::substitute(const std::string&, const std::complex<double>&) const;

