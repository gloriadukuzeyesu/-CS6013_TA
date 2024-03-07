cmake_minimum_required(VERSION 3.24)
project(MallocReplacement)

set(CMAKE_CXX_STANDARD 17)

# add the executable
add_executable(MallocReplacement main.cpp msdMalloc.cpp msdMalloc.h)


#target_link_libraries(MallocReplacement /usr/local/lib/libbenchmark.a)
#add_library(msdMalloc.cpp main.cpp Tests.cpp)
#target_link_libraries(msdMalloc.cpp benchmark)



