//
// Created by aven on 31/05/2026.
//

#include "Token.h"

Token::Token(const TokenType given_type, const std::string &given_value) {
    type = given_type;
    value = given_value;
}

std::string Token::type_to_string(const TokenType given_type) {
    switch (given_type) {
        #define PATTERN_DEF(token, cat) case TokenType::token: return #token;
        PATTERN_LIST
        #undef PATTERN_DEF
        #define PUNCT_DEF(str, token, cat) case TokenType::token: return #token;
        PUNCT_LIST
        #undef PUNCT_DEF
        #define OPERATOR_DEF(str, token, cat) case TokenType::token: return #token;
        OPERATOR_LIST
        #undef OPERATOR_DEF
        #define KEYWORD_DEF(str, token, cat) case TokenType::token: return #token;
        KEYWORD_LIST
        #undef KEYWORD_DEF
    }
    return "undefined";
}
