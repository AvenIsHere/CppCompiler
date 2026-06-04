//
// Created by aven on 19/05/2026.
//

#include "Lexer.h"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <utility>

Lexer::Lexer(std::string input_string) : input(std::move(input_string)), input_pos(0), current_token_type(TokenType::UNKNOWN), line(1), column(0) {
    character = static_cast<unsigned char>(input[0]);
    tokenise();
}

void Lexer::next_character() {
    input_pos++;
    column++;
    if (character == '\n') column = 0;
    character = static_cast<unsigned char>(input[input_pos]);
}

void Lexer::submit_token() {
    tokens.emplace_back(current_token_type, current_word);
    current_token_type = TokenType::UNKNOWN;
    current_word.clear();
}

bool Lexer::starts_operator(unsigned char character) {
    return std::ranges::any_of(OPERATORS, [character](const auto& token) {
        return !token.first.empty() && static_cast<unsigned char>(token.first[0]) == character;
    });
}

void Lexer::lex_operator() {
    auto last_valid_match = OPERATORS.end();
    const std::string_view remaining_input = std::string_view(input).substr(input_pos);
    for (size_t op_len = 1; op_len <= remaining_input.length(); op_len++) {
        std::string_view op = remaining_input.substr(0, op_len);
        auto match = std::ranges::find_if(OPERATORS, [op](const auto& token) {
            return token.first == op;
        });
        if (match != OPERATORS.end()) last_valid_match = match;
        const bool any_prefix = std::ranges::any_of(OPERATORS, [op](const auto& token) {
            return std::string_view(token.first).starts_with(op);
        });
        if (!any_prefix) break;
    }
    assert(last_valid_match != OPERATORS.end() && "lex_operator called at non-operator position");
    current_token_type = last_valid_match->second;
    current_word = last_valid_match->first;
    input_pos += current_word.length();
    submit_token();
}

bool Lexer::can_start_ident(const unsigned char given_character) {
    return std::isalpha(given_character) || given_character == '_';
}

bool Lexer::is_ident_char(const unsigned char given_character) {
    return std::isalnum(given_character) || given_character == '_';
}

void Lexer::check_keyword() {
    const auto& keyword_type = KEYWORDS.find(current_word);
    if (keyword_type == KEYWORDS.end()) return;
    current_token_type = keyword_type->second;
}

void Lexer::lex_ident() {
    current_token_type = TokenType::IDENTIFIER;
    while (is_ident_char(character)) {
        current_word.push_back(static_cast<char>(character));
        next_character();
    }
    check_keyword();
    submit_token();
}

void Lexer::lex_number() {
    current_token_type = TokenType::INT;
    while (std::isdigit(character) || character == '.') {
        if (character == '.') current_token_type = TokenType::FLOAT;
        current_word.push_back(static_cast<char>(character));
        next_character();
    }
    submit_token();
}

std::optional<TokenType> Lexer::starts_pattern(const unsigned char given_character) {
    if (given_character == '\"') return TokenType::STR_LIT;
    if (given_character == '\'') return TokenType::CHAR_LIT;
    if (std::isdigit(given_character)) return TokenType::INT;
    if (std::isalpha(given_character) || given_character == '_') return TokenType::IDENTIFIER;
    return std::nullopt;
}

bool Lexer::ends_pattern(const unsigned char given_character) const {
    if (current_token_type == TokenType::STR_LIT && given_character == '\"') return true;
    if (current_token_type == TokenType::CHAR_LIT && given_character == '\'') return true;
    if (current_token_type == TokenType::INT && !isdigit(given_character)) return true;
    if (current_token_type == TokenType::IDENTIFIER && !is_ident_char(given_character)) return true;
    return false;
}

void error_at(const char character, const size_t line, const size_t column) {
    throw std::runtime_error(
        std::string("Invalid character: ") + character +std::string(" at ") +
        std::to_string(line) + std::string(":") + std::to_string(column));
}

void Lexer::tokenise() {
    while (input_pos < input.length()) {
        if (current_token_type == TokenType::UNKNOWN) {
            if (starts_operator(character)) {
                lex_operator();
                continue;
            }
            if (can_start_ident(character)) {
                lex_ident();
                continue;
            }
            if (std::isdigit(character)) {
                lex_number();
                continue;
            }
            if (std::isspace(character) || character == '\n') {
                next_character();
                continue;
            }
        }
    }
    if (!current_word.empty()) tokens.emplace_back(current_token_type, current_word);
}

std::vector<Token> Lexer::get_tokens() {
    return tokens;
}
