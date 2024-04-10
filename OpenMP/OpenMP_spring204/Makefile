# CXX = /opt/homebrew/opt/llvm/bin/clang++
CXX = /opt/homebrew/bin/g++-13
CFLAGS = -std=c++20
OPENMP_COMPILING = -fopenmp
OPENMP_LINKING = -lomp
OPEN_LIBRARY_PATH = -L/opt/homebrew/opt/libomp/lib
OPEN_INCLUDE_PATH = -I/opt/homebrew/opt/libomp/include/



# Capital -L is used to specify library search directories
# small -l is used to specify the name of the library to link against.
# -I is used to get the file.


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
