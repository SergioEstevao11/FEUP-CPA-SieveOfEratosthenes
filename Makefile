# Makefile
# CPA @ M.EIC, 2023
# Authors: Miguel Rodrigues & Sérgio Estêvão
CXX=g++
CXXFLAGS=-std=c++20 -O3 -Wall -Wextra -Werror -pedantic -Wconversion
LDLIBS=-lpapi

.PHONY: clean all

all: basic multiples segmented parallel

%: src/soe%.cpp 
	@mkdir -p bin/
	$(CXX) $(CXXFLAGS) $< -o bin/$@.out $(LDLIBS)

parallel: src/soeparallel.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $< -fopenmp -o bin/$@.out $(LDLIBS)

clean:
	$(RM) bin/*.out
