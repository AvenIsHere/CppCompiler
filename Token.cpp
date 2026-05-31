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
        case NUM: return "NUM";
        case LEFT_PARENTHESIS: return "LEFT_PARENTHESIS";
        case RIGHT_PARENTHESIS: return "RIGHT_PARENTHESIS";
        case IDENTIFIER: return "IDENTIFIER";
        case SEMI_COLON: return "SEMI_COLON";
        #define OPERATOR_DEF(str, token, cat) case token: return #token;
        OPERATOR_LIST
        #undef OPERATOR_DEF
        #define KEYWORD_DEF(str, token, cat) case token: return #token;
        KEYWORD_LIST
        #undef KEYWORD_DEF
    }
    return "undefined";
}
