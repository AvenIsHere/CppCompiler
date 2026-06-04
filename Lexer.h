//
// Created by aven on 19/05/2026.
//

#ifndef CPPCOMPILER_TOKENISER_H
#define CPPCOMPILER_TOKENISER_H
#include <optional>
#include <string>
#include <vector>

#include "Token.h"
#include "TokenType.h"

class Lexer {

public:

    explicit Lexer(std::string input_string);

    void tokenise();
    std::vector<Token> get_tokens();

private:

    const static inline std::unordered_map<std::string, TokenType> KEYWORDS = {
        #define KEYWORD_DEF(str, token, cat) {str, TokenType::token},
        KEYWORD_LIST
        #undef KEYWORD_DEF
    };

    const static inline std::unordered_map<std::string, TokenType> OPERATORS = {
        #define OPERATOR_DEF(str, token, cat) {str, TokenType::token},
        OPERATOR_LIST
        #undef OPERATOR_DEF
    };

    const static inline std::unordered_map<std::string, TokenType> PUNCTUATION = {
        #define PUNCT_DEF(str, token, cat) {str, TokenType::token},
        PUNCT_LIST
        #undef PUNCT_DEF
    };

    void next_character();
    void submit_token();

    [[nodiscard]] static bool starts_operator(unsigned char character);
    void lex_operator();

    [[nodiscard]] static bool can_start_ident(unsigned char given_character);
    [[nodiscard]] static bool is_ident_char(unsigned char given_character);
    void check_keyword();
    void lex_ident();

    void lex_number();

    char get_escaped_char();
    void lex_string_literal();
    void lex_char_literal();

    static std::optional<TokenType> starts_pattern(unsigned char given_character);
    [[nodiscard]] bool ends_pattern(unsigned char given_character) const;

    std::string input;
    size_t input_pos;
    unsigned char character;

    std::vector<Token> tokens;
    std::string current_word;
    TokenType current_token_type;

    size_t line;
    size_t column;

};


#endif //CPPCOMPILER_TOKENISER_H
