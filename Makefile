
CXX          = g++
CXXFLAGS     = --std=c++17 -Wall -Wextra -pedantic -g -O3

compile:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) filter.cpp -o bin/filter
	$(CXX) $(CXXFLAGS) preprocess.cpp -o bin/preprocess