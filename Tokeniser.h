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

class Tokeniser {

public:

    explicit Tokeniser(const std::string &input_string);
    std::vector<Token> get_tokens();

private:

    const static inline std::vector<Token> KEYWORDS = {
        #define KEYWORD_DEF(str, token, cat) {token, str},
        KEYWORD_LIST
        #undef KEYWORD_DEF
    };

    const static inline std::vector<Token> OPERATORS = {
        #define OPERATOR_DEF(str, token, cat) {token, str},
        OPERATOR_LIST
        #undef OPERATOR_DEF
    };

    const static inline std::vector<std::vector<Token>> SPECIFIC_TOKENS = {
        KEYWORDS,
        OPERATORS
    };

    static bool can_start_ident(char given_char);
    static bool is_ident_char(char given_char);
    static bool is_ident(const std::string &word);

    static bool is_token_type(const std::vector<Token>& token_vec, TokenType type);
    static std::optional<TokenType> is_specific_token(const std::vector<Token>& token_vec, const std::string &word);
    static std::vector<Token> get_possible_tokens(const std::vector<Token>& token_vec, const std::string &word);

    static bool is_operator_char(char given_char);
    [[nodiscard]] std::optional<Token> get_operator(size_t pos) const;

    std::string input;

    std::vector<Token> tokens;
    std::string current_word;
    TokenType current_token_type;

    int line;
    int column;

    void new_word();

    std::vector<Token> tokenise();



};


#endif //CPPCOMPILER_TOKENISER_H
