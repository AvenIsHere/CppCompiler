//
// Created by aven on 19/05/2026.
//

#include "Lexer.h"

#include <algorithm>
#include <ranges>

Lexer::Lexer(const std::string &input_string) {
    input = input_string;
    current_token_type = {};
    line = 1;
    column = 0;
    tokenise();
}

bool Lexer::can_start_ident(const char given_char) {
    return isalpha(given_char) || given_char == '_';
}

bool Lexer::is_ident_char(const char given_char) {
    return isalnum(given_char) || given_char == '_';
}

bool Lexer::is_ident(const std::string &word) {
    if (word.empty()) return false;
    if (!can_start_ident(word[0])) return false;
    for (int i = 1; i < word.length(); i++) {
        if (!is_ident_char(word[i])) return false;
    }
    return true;
}



bool Lexer::is_token_type(const std::vector<Token>& token_vec, TokenType type) {
    return std::ranges::any_of(token_vec.begin(), token_vec.end(), [&type](const auto& pair) {
        const auto& [ token, value ] = pair;
        return type == token;
    });
}

std::optional<TokenType> Lexer::is_specific_token(const std::vector<Token>& token_vec, const std::string &word) {
    for (const auto& [token, value] : token_vec) {
        if (word == value) return token;
    }
    return std::nullopt;
}

std::vector<Token> Lexer::get_possible_tokens(const std::vector<Token> &token_vec, const std::string &word) {
    std::vector<Token> return_tokens;
    for (const auto& token : token_vec) {
        if (token.value.starts_with(word)) return_tokens.push_back(token);
    }
    return return_tokens;
}

bool Lexer::is_operator_char(char given_char) {
    return std::ranges::any_of(OPERATORS.begin(), OPERATORS.end(), [&given_char](const auto& op) {
        const auto& [token, value] = op;
        return value[0] == given_char;
    });
}

std::optional<Token> Lexer::get_operator(const size_t pos) const {
    std::string current_op(1, input[pos]);
    size_t current_pos = pos;
    std::vector<Token> possible_tokens = get_possible_tokens(OPERATORS, current_op);
    if (possible_tokens.empty()) return std::nullopt;
    while (possible_tokens.size() > 1) {
        current_pos++;
        current_op.push_back(input[current_pos]);
        possible_tokens = get_possible_tokens(OPERATORS, current_op);
    }
    if (possible_tokens.empty()) {
        current_op.pop_back();
        possible_tokens = get_possible_tokens(OPERATORS, current_op);
        if (possible_tokens.empty()) return std::nullopt;
    }
    if (possible_tokens[0].value == current_op) return possible_tokens[0];
    return std::nullopt;
}

void Lexer::new_word() {
    if (const auto keyword = is_specific_token(KEYWORDS, current_word); keyword != std::nullopt)
        current_token_type = keyword.value();
    if (!current_word.empty()) tokens.emplace_back(current_token_type, current_word);
    current_word.clear();
}

void error_at(const char character, const size_t line, const size_t column) {
    throw std::runtime_error(
        std::string("Invalid character: ") + character +std::string(" at ") +
        std::to_string(line) + std::string(":") + std::to_string(column));
}

std::vector<Token> Lexer::tokenise() {
    size_t x = 0;
    while (x < input.length()) {
        const char character = input[x];
        column++;
        if (is_operator_char(character)) {
            new_word();
            const auto& op = get_operator(x);
            if (op == std::nullopt) error_at(character, line, column);
            tokens.push_back(op.value());
            column += op.value().value.length();
            x += op.value().value.length();
            continue;
        }
        x++;
        if (character == '\n') {
            line++;
            column = 0;
            continue;
        }
        if (std::isspace(character)) {
            new_word();
            continue;
        }
        if (character == ';') {
            new_word();
            tokens.emplace_back(TokenType::SEMI_COLON, std::string(1, character));
            continue;
        }
        if (current_word.empty() ? can_start_ident(character) :
            current_token_type == TokenType::IDENTIFIER && is_ident_char(character)) {
            current_token_type = TokenType::IDENTIFIER;
            current_word.push_back(character);
            continue;
        }
        if (isdigit(character)) {
            if (!current_word.empty() && !isdigit(current_word.back())) new_word();
            current_token_type = TokenType::NUM;
            current_word.push_back(character);
            continue;
        }
        if (character == '(') {
            new_word();
            tokens.emplace_back(TokenType::LEFT_PARENTHESIS, std::string(1, character));
            continue;
        }
        if (character == ')') {
            new_word();
            tokens.emplace_back(TokenType::RIGHT_PARENTHESIS, std::string(1, character));
            continue;
        }
        error_at(character, line, column);

    }
    if (!current_word.empty()) tokens.emplace_back(current_token_type, current_word);
    return tokens;
}

std::vector<Token> Lexer::get_tokens() {
    return tokens;
}
