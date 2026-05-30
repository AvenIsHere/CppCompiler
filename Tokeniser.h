//
// Created by aven on 19/05/2026.
//

#ifndef CPPCOMPILER_TOKENISER_H
#define CPPCOMPILER_TOKENISER_H
#include <string>
#include <vector>


class Tokeniser {

public:

    enum TokenType {
        NUM,
        OPERATOR,
        EQUALITY,
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        IDENTITY,
        KEYWORD,
    };

    struct Token {
        TokenType token;
        std::string value;
    };

    explicit Tokeniser(const std::string &input_string);
    std::vector<Token> get_tokens();

private:

    const static inline std::vector<std::string> KEYWORDS = {
        "int",
        "char",
        "short",
        "long",
        "float",
        "double",
        "wchar_t",
        "char8_t",
        "char16_t",
        "char32_t",
        "bool",
    };

    const static inline std::vector<char> OP_SYMBOLS = {
        '+',
        '-',
        '/',
        '*',
        '^'
    };

    template <typename T>
    static bool in_list(std::vector<T> list, T item);

    static bool is_ident_char(char given_char);
    static bool is_ident(const std::string &word);

    std::string input;

    std::vector<Token> tokens;
    std::string current_word;
    TokenType current_token_type;

    void new_word();

    std::vector<Token> tokenise();



};


#endif //CPPCOMPILER_TOKENISER_H
