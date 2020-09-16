cmake_minimum_required(VERSION 3.18) 

project(Lispy VERSION 1.0) 
set(CMAKE_C_STANDARD 11) 

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(Lispy
    src/parsing.c
    src/eval.c
    src/ast.c
    ext/mpc-0.9.0/mpc.c
) 

target_include_directories(Lispy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ext/mpc-0.9.0)
target_link_libraries(Lispy PRIVATE editline)

# aux_source_directory(src SRC)
# add_executable(Lispy ${SRC}) 