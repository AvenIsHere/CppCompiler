//
// Created by aven on 19/05/2026.
//

#include "Lexer.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <ranges>
#include <string>
#include <utility>

Lexer::Lexer(std::string input_string) : input(std::move(input_string)), input_pos(0), current_token_type(TokenType::UNKNOWN), line(1), column(1) {
    character = input.empty() ? 0 : static_cast<unsigned char>(input[0]);
    tokenise();
}

void Lexer::next_character() {
    if (input_pos >= input.length()) {
        character = 0;
        return;
    }
    if (character == '\n') {
        column = 1;
        line++;
    } else {
        column++;
    }
    input_pos++;
    character = (input_pos < input.length()) ? static_cast<unsigned char>(input[input_pos]) : 0;
}

void Lexer::submit_token() {
    tokens.emplace_back(current_token_type, current_word);
    current_token_type = TokenType::UNKNOWN;
    current_word.clear();
}


bool Lexer::lex_operator() {
    auto last_valid_match = OPERATORS.end();
    const std::string_view remaining_input = std::string_view(input).substr(input_pos);
    for (size_t op_len = 1; op_len <= remaining_input.length(); op_len++) {
        std::string op(remaining_input.substr(0, op_len));
        auto match = OPERATORS.find(op);
        if (match != OPERATORS.end()) last_valid_match = match;
        
        const bool any_prefix = std::ranges::any_of(OPERATORS, [&op](const auto& token) {
            return token.first.starts_with(op);
        });
        if (!any_prefix) break;
    }
    if (last_valid_match == OPERATORS.end()) return false;
    current_token_type = last_valid_match->second;
    current_word = last_valid_match->first;
    for (size_t i = 0; i < current_word.length(); i++) next_character();
    submit_token();
    return true;
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
    while (!ends_pattern(character)) {
        current_word.push_back(static_cast<char>(character));
        next_character();
    }
    check_keyword();
    submit_token();
}

void Lexer::lex_number() {
    current_token_type = TokenType::INT;
    while (!ends_pattern(character)) {
        if (character == '.') {
            if (current_token_type == TokenType::FLOAT) {
                throw std::runtime_error(std::format("Invalid number at {}:{}", line, column));
            }
            current_token_type = TokenType::FLOAT;
        }
        current_word.push_back(static_cast<char>(character));
        next_character();
    }
    submit_token();
}

char Lexer::get_escaped_char() {
    next_character();
    switch (character) {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case 'r':
            return '\r';
        case '\\':
            return '\\';
        case '\'':
            return '\'';
        case '\"':
            return '\"';
        case '0':
            return '\0';
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'v':
            return '\v';
        default:
            throw std::runtime_error(std::format(R"(unknown escape sequence: '\{}' at {}:{})", character, line, column));
    }
}

void Lexer::lex_string_literal() {
    current_token_type = TokenType::STR_LIT;
    next_character();
    while (character != 0 && !ends_pattern(character)) {
        if (character == '\\') {
            const auto push_char = get_escaped_char();
            current_word.push_back(push_char);
            next_character();
            continue;
        }
        current_word.push_back(static_cast<char>(character));
        next_character();
    }
    if (character == 0) throw std::runtime_error(std::format("Unterminated string literal at {}:{}", line, column));
    next_character();
    submit_token();
}

void Lexer::lex_char_literal() {
    current_token_type = TokenType::CHAR_LIT;
    next_character();
    if (character == 0) throw std::runtime_error(std::format("Unterminated character literal at {}:{}", line, column));
    if (character == '\'') throw std::runtime_error(std::format("Empty character literal at {}:{}", line, column));
    auto final_char = character;
    if (character == '\\') final_char = get_escaped_char();
    current_word.push_back(static_cast<char>(final_char));
    next_character();
    if (character == 0 || !ends_pattern(character)) throw std::runtime_error(std::format("Invalid character literal at {}:{}", line, column));
    next_character();
    submit_token();
}

std::optional<TokenType> Lexer::starts_pattern(const unsigned char given_character) const {
    if (given_character == '\"') return TokenType::STR_LIT;
    if (given_character == '\'') return TokenType::CHAR_LIT;
    if (std::isdigit(given_character)) return TokenType::INT;
    if (given_character == '.') {
        if (input_pos + 1 < input.length() && std::isdigit(static_cast<unsigned char>(input[input_pos + 1]))) {
            return TokenType::FLOAT;
        }
    }
    if (std::isalpha(given_character) || given_character == '_') return TokenType::IDENTIFIER;
    return std::nullopt;
}

bool Lexer::ends_pattern(const unsigned char given_character) const {
    if (current_token_type == TokenType::STR_LIT && given_character == '\"') return true;
    if (current_token_type == TokenType::CHAR_LIT && given_character == '\'') return true;
    if ((current_token_type == TokenType::INT || current_token_type == TokenType::FLOAT) && !(std::isdigit(given_character) || given_character == '.')) return true;
    if (current_token_type == TokenType::IDENTIFIER && !is_ident_char(given_character)) return true;
    return false;
}

void error_at(const unsigned char character, const size_t line, const size_t column) {
    throw std::runtime_error(
        std::string("Invalid character: ") + static_cast<char>(character) +std::string(" at ") +
        std::to_string(line) + std::string(":") + std::to_string(column));
}

bool Lexer::skip_comment() {
    if (character != '/' || input_pos + 1 >= input.length()) return false;

    if (input[input_pos + 1] == '/') {
        while (character != 0 && character != '\n') next_character();
        return true;
    }

    if (input[input_pos + 1] == '*') {
        bool closed = false;
        next_character(); // /
        next_character(); // *
        while (character != 0) {
            if (character == '*') {
                next_character();
                if (character == '/') {
                    next_character();
                    closed = true;
                    break;
                }
                continue;
            }
            next_character();
        }
        if (!closed) throw std::runtime_error(std::format("Unterminated multi-line comment at {}:{}", line, column));
        return true;
    }

    return false;
}

bool Lexer::lex_punctuation() {
    const std::string char_str(1, static_cast<char>(character));
    if (const auto it = PUNCTUATION.find(char_str); it != PUNCTUATION.end()) {
        current_token_type = it->second;
        current_word = char_str;
        next_character();
        submit_token();
        return true;
    }
    return false;
}

void Lexer::tokenise() {
    while (input_pos < input.length() && character != 0) {
        if (std::isspace(character)) {
            next_character();
            continue;
        }

        if (skip_comment()) continue;

        if (const auto pattern = starts_pattern(character)) {
            if (*pattern == TokenType::STR_LIT) lex_string_literal();
            else if (*pattern == TokenType::CHAR_LIT) lex_char_literal();
            else if (*pattern == TokenType::INT || *pattern == TokenType::FLOAT) lex_number();
            else if (*pattern == TokenType::IDENTIFIER) lex_ident();
            continue;
        }

        if (lex_operator()) continue;
        if (lex_punctuation()) continue;

        error_at(character, line, column);
    }
}

std::vector<Token> Lexer::get_tokens() {
    return tokens;
}
