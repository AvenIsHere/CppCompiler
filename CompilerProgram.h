//
// Created by aven on 19/05/2026.
//

#ifndef CPPCOMPILER_COMPILERPROGRAM_H
#define CPPCOMPILER_COMPILERPROGRAM_H
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


class CompilerProgram {
public:

    enum Mode {
        FILE,
        STRING,
    };

    static inline const std::string HELP_MSG = {
        "CppCompiler - Compiler for C++ programs \n"
        "Usage:\n"
        "cppcompiler [FILENAME] -[short flags] --[flags]\n"
    };

    static std::unordered_map<std::string, std::function<void()>> FLAGS;
    static std::unordered_map<char, std::string> SHORT_FLAGS;

    static Mode mode;
    static inline bool debug_mode = false;

    static int handle_flag(const std::string &given_flag);
    static int main(std::vector<std::string> args);

};


#endif //CPPCOMPILER_COMPILERPROGRAM_H
