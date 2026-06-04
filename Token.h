//
// Created by aven on 31/05/2026.
//

#ifndef CPPCOMPILER_TOKEN_H
#define CPPCOMPILER_TOKEN_H

#include <string>
#include <unordered_map>

#include "TokenType.h"

class Token {
public:

    TokenType type;
    std::string value;

    const static inline std::unordered_map<TokenType, TokenCategory> TOKEN_CATEGORY = {
        #define PATTERN_DEF(token, cat) {TokenType::token, TokenCategory::cat},
        PATTERN_LIST
        #undef PATTERN_DEF
        #define PUNCT_DEF(str, token, cat) {TokenType::token, TokenCategory::cat},
        PUNCT_LIST
        #undef PUNCT_DEF
        #define KEYWORD_DEF(str, token, cat) {TokenType::token, TokenCategory::cat},
        KEYWORD_LIST
        #undef KEYWORD_DEF
        #define OPERATOR_DEF(str, token, cat) {TokenType::token, TokenCategory::cat},
        OPERATOR_LIST
        #undef OPERATOR_DEF
    };

    Token(TokenType given_type, const std::string &given_value);

    static std::string type_to_string(TokenType given_type);

};


#endif //CPPCOMPILER_TOKEN_H
