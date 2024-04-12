# CXX = /opt/homebrew/opt/llvm/bin/clang++
CXX = /opt/homebrew/bin/g++-13
CFLAGS = -std=c++20
OPENMP_COMPILING = -fopenmp
OPENMP_LINKING = -lomp
OPEN_LIBRARY_PATH = -L/opt/homebrew/opt/libomp/lib
OPEN_INCLUDE_PATH = -I/opt/homebrew/opt/libomp/include/

pro: main.cpp
	$(CXX) $(CFLAGS) $(OPENMP_COMPILING) $(OPEN_LIBRARY_PATH) $(OPENMP_LINKING) $(OPEN_INCLUDE_PATH) -o pro main.cpp

.PHONY: clean
clean:
	rm -rf *.o  pro project


#CC = gcc
#CFLAGS = -Wall -Wextra -std=c99 -fopenmp
#
#project: main.cpp
#	$(CC) $(CFLAGS) -o project main.cpp
