# Makefile
# CPA @ M.EIC, 2023
# Authors: Miguel Rodrigues & Sérgio Estêvão
CXX=g++
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra -Werror -pedantic
LDLIBS=-lpapi

.PHONY: clean all

all: soe_basic

%: src/%.cpp 
	$(CXX) $(CXXFLAGS) $< -o bin/$@.out $(LDLIBS)

clean:
	$(RM) bin/*.out