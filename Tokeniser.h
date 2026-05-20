//
// Created by aven on 19/05/2026.
//

#ifndef CPPCOMPILER_PARSER_H
#define CPPCOMPILER_PARSER_H
#include <string>
#include <vector>


class Tokeniser {

public:

    enum TokenType {
        NUM,
        OPERATOR,
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
    };

    struct Token {
        TokenType token;
        std::string value;
    };

    explicit Tokeniser(const std::string &input_string);
    std::vector<Token> get_tokens();

private:

    std::string input;

    std::vector<Token> tokens;
    std::string current_word;
    TokenType current_token_type;

    std::vector<Token> tokenise();



};


#endif //CPPCOMPILER_PARSER_H
