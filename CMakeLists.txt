cmake_minimum_required(VERSION 4.2)
project(CppCompiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(CppCompiler main.cpp
        Tokeniser.cpp
        Tokeniser.h
        CompilerProgram.cpp
        CompilerProgram.h)
