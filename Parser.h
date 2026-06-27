//
// Created by aven on 21/05/2026.
//

#ifndef CPPCOMPILER_PARSER_H
#define CPPCOMPILER_PARSER_H
#include <vector>

#include "Token.h"


class Parser {

    std::vector<Token> tokens;

public:

    explicit Parser(std::vector<Token> given_tokens);

};


#endif //CPPCOMPILER_PARSER_H
