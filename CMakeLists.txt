cmake_minimum_required(VERSION 3.30)
project(SymbolicDifferentiation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включить предупреждения
add_compile_options(-Wall -Wextra -pedantic)

# Пути
include_directories(include)

# Основные исходники
set(SRC_FILES
        src/Expression.cpp
        src/operations.cpp
        src/parser.cpp
)

# Executable: differentiator
add_executable(differentiator
        src/differentiator.cpp
        ${SRC_FILES}
)

# Executable: test_runner
add_executable(test_runner
        tests/tests.cpp
        ${SRC_FILES}
)
