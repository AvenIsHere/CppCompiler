//
// Created by aven on 27/06/2026.
//

#ifndef CPPCOMPILER_PREPROCESSOR_H
#define CPPCOMPILER_PREPROCESSOR_H
#include <string>

enum class PreprocessorState {
    NONE,
    DIRECTIVE,
    DIRECTIVE_VALUE,
    CODE,
    MACRO,
};

enum class Directive {
    DEF,
    UNDEF,
    IF,
    INCLUDE,
    IFNDEF,
};


class Preprocessor {

    std::string input;
    PreprocessorState state;

public:

    Preprocessor(std::string given_input);

    std::string process();

};


#endif //CPPCOMPILER_PREPROCESSOR_H
