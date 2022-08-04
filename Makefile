
CXX          = g++
CXXFLAGS     = --std=c++17 -Wall -Wextra -pedantic -g -O3 -march=haswell

compile:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) filter.cpp -o bin/filter
	$(CXX) $(CXXFLAGS) solver.cpp -o bin/solver