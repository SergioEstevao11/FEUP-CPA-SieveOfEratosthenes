# Makefile
# CPA @ M.EIC, 2023
# Authors: Miguel Rodrigues & Sérgio Estêvão
CXX=g++
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: clean all

%: src/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@.out

clean:
	$(RM) *.out