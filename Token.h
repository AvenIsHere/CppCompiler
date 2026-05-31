//
// Created by aven on 31/05/2026.
//

#ifndef CPPCOMPILER_TOKEN_H
#define CPPCOMPILER_TOKEN_H

#include <string>

#include "TokenType.h"

class Token {
public:

    TokenType type;
    std::string value;

    Token(TokenType given_type, const std::string &given_value);

    static std::string type_to_string(TokenType given_type);

};


#endif //CPPCOMPILER_TOKEN_H
