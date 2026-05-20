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

bool Tokeniser::is_operator(const char character) {
    return std::ranges::count(OP_SYMBOLS, character) > 0;
}

std::vector<Tokeniser::Token> Tokeniser::tokenise() {
    for (const auto& character : input) {
        if (std::isspace(character)) continue;
        if (isdigit(character)) {
            if (!current_word.empty() && !isdigit(current_word.back())) {
                tokens.push_back({current_token_type, current_word});
                current_word.clear();
            }
            current_token_type = NUM;
            current_word.push_back(character);
            continue;
        }
        if (is_operator(character)) {
            if (!current_word.empty()) {
                tokens.push_back({current_token_type, current_word});
                current_word.clear();
            }
            current_token_type = OPERATOR;
            current_word.push_back(character);
            continue;
        }
        if (character == '(') {
            if (!current_word.empty()) {
                tokens.push_back({current_token_type, current_word});
                current_word.clear();
            }
            current_token_type = LEFT_PARENTHESIS;
            current_word.push_back(character);
            continue;
        }
        if (character == ')') {
            if (!current_word.empty()) {
                tokens.push_back({current_token_type, current_word});
                current_word.clear();
            }
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
