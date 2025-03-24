CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra

SRC = src/Expression.cpp src/operations.cpp src/parser.cpp
INC = include/Expression.hpp

all: differentiator test_runner

differentiator: src/differentiator.cpp $(SRC) $(INC)
	$(CXX) $(CXXFLAGS) -o differentiator $^

test_runner: tests/tests.cpp $(SRC) $(INC)
	$(CXX) $(CXXFLAGS) -o test_runner $^

test: test_runner
	./test_runner

clean:
	rm -f differentiator test_runner
