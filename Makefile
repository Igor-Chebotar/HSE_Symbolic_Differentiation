CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra -pedantic -g

SRC = src/Expression.cpp src/operations.cpp src/parser.cpp
OBJ = $(SRC:.cpp=.o)
INC = include/Expression.hpp

all: differentiator test_runner

differentiator: src/differentiator.cpp $(SRC) $(INC)
	$(CXX) $(CXXFLAGS) -o $@ src/differentiator.cpp $(SRC)

test_runner: tests/tests.cpp $(SRC) $(INC)
	$(CXX) $(CXXFLAGS) -o $@ tests/tests.cpp $(SRC)

test: test_runner
	./test_runner

clean:
	rm -f differentiator test_runner *.o
