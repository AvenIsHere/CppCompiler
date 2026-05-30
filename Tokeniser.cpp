//
// Created by aven on 19/05/2026.
//

#include "Tokeniser.h"

#include <algorithm>
#include <bits/streambuf_iterator.h>

Tokeniser::Tokeniser(const std::string &input_string) {
    input = input_string;
    current_token_type = {};
    tokenise();
}

template <typename T>
bool Tokeniser::in_list(std::vector<T> list, T item) {
    return std::ranges::count(list, item) > 0;
}

bool Tokeniser::is_ident_char(const char given_char) {
    return isalnum(given_char) || given_char == '_';
}

bool Tokeniser::is_ident(const std::string &word) {
    if (word.empty()) return false;
    if (in_list(KEYWORDS, word)) return false;
    if (!isalpha(word[0]) && word[0] != '_') return false;
    for (int i = 1; i < word.length(); i++) {
        if (!is_ident_char(word[i])) return false;
    }
    return true;
}

void Tokeniser::new_word() {
    if (current_token_type == IDENTITY && in_list(KEYWORDS, current_word)) current_token_type = KEYWORD;
    if (!current_word.empty()) tokens.push_back({current_token_type, current_word});
    current_word.clear();
}

std::vector<Tokeniser::Token> Tokeniser::tokenise() {
    for (const auto& character : input) {
        if (std::isspace(character)) {
            new_word();
            continue;
        }
        if (isdigit(character)) {
            if (!current_word.empty() && !is_ident(current_word) && !isdigit(current_word.back())) {
                tokens.push_back({current_token_type, current_word});
                current_word.clear();
            }
            if (is_ident(current_word)) current_token_type = IDENTITY;
            else current_token_type = NUM;
            current_word.push_back(character);
            continue;
        }
        if (in_list(OP_SYMBOLS, character)) {
            new_word();
            current_token_type = OPERATOR;
            current_word.push_back(character);
            continue;
        }
        if (character == '(') {
            new_word();
            current_token_type = LEFT_PARENTHESIS;
            current_word.push_back(character);
            continue;
        }
        if (character == ')') {
            new_word();
            current_token_type = RIGHT_PARENTHESIS;
            current_word.push_back(character);
            continue;
        }
        throw std::runtime_error(std::string("Invalid character: ") + character);
    }
    if (!current_word.empty()) tokens.push_back({current_token_type, current_word});
    return tokens;
}

std::vector<Tokeniser::Token> Tokeniser::get_tokens() {
    return tokens;
}
