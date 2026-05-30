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

    std::vector<std::string> args;

public:

    CompilerProgram(std::vector<std::string> args_flags);

    enum Mode {
        FILE,
        STRING,
    };

    static inline const std::string HELP_MSG = {
        "CppCompiler - Compiler for C++ programs \n"
        "Usage:\n"
        "cppcompiler [FILENAME] -[short flags] --[flags]\n"
    };

    std::unordered_map<std::string, std::function<void()>> FLAGS;
    std::unordered_map<char, std::string> SHORT_FLAGS;

    Mode mode;
    bool debug_mode = false;

    int handle_flag(const std::string &given_flag);
    static bool is_flag(const std::string &arg);

    int run() const;

};


#endif //CPPCOMPILER_COMPILERPROGRAM_H
