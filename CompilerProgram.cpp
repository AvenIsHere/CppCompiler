//
// Created by aven on 19/05/2026.
//

#include "CompilerProgram.h"

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

#include "Token.h"
#include "Lexer.h"

CompilerProgram::CompilerProgram(std::vector<std::string> args_flags) {

    mode = FILE;
    FLAGS = {
        {"help", []{std::cout << HELP_MSG << std::endl;}},
        {"string", [this]{mode = STRING;}},
        {"file", [this]{mode = FILE;}},
        {"debug", [this] {debug_mode = true;}}
    };
    SHORT_FLAGS = {
        {'h', "help"},
        {'f', "file"},
        {'s', "string"},
        {'d', "debug"}
    };

    for (const auto [i, arg] : std::views::enumerate(args_flags)) {
        if (!is_flag(arg)) args.push_back(arg);
        else handle_flag(arg);
    }

}

bool CompilerProgram::is_double_flag(const std::string &given_flag) {
    if (given_flag.starts_with("--")) return true;
    if (!given_flag.starts_with("-")) throw std::logic_error("non-flag treated as flag.");
    return false;
}

int CompilerProgram::handle_flag(const std::string &given_flag) {
    auto flag_content = given_flag.substr(1);
    if (given_flag.starts_with("--")) {
        flag_content = given_flag.substr(2);
        if (FLAGS.contains(flag_content)) {
            FLAGS.at(flag_content)();
            return 0;
        }
        flag_content.erase(0);
    }
    std::string unknown_flags;

    for (const auto& character : flag_content) {
        if (SHORT_FLAGS.contains(character)) {
            FLAGS.at(SHORT_FLAGS.at(character))();
            continue;
        }
        unknown_flags.push_back(character);
    }
    if (unknown_flags.empty()) return 0;
    std::cerr << "unknown flag(s) \"" << unknown_flags << "\"" << std::endl;
    for (const auto &flag: SHORT_FLAGS | std::views::keys) std::cerr << flag << ", ";
    std::cerr << std::endl;
    return 1;
}

bool CompilerProgram::is_flag(const std::string &arg) {
    if (arg.length() > 1 && std::isdigit(arg[1])) return false;
    return arg.starts_with("-");
}

std::string file_to_string(std::fstream file) {
    std::ostringstream file_stream;
    file_stream << file.rdbuf();
    return file_stream.str();
}

int CompilerProgram::run() const {

    if (args.size() > 1) {
        std::cerr << "Too many arguments. Did you forget a -?" << std::endl;
        return 1;
    }

    if (args.empty()) {
        std::cerr << "No filename given." << std::endl;
        return 1;
    }

    std::string input;

    switch (mode) {
        case FILE: {
            std::fstream file(args[0], std::ios::in);
            input = file_to_string(std::move(file));
            break;
        }
        case STRING: {
            input = args[0];
            break;
        }
    }

    auto tokeniser = Lexer(input);
    std::vector<Token> output = tokeniser.get_tokens();

    if (debug_mode) {
        for (const auto&[token, value] : output) {
            std::cout << "{" << Token::type_to_string(token) << ", \"" << value << "\"}" << std::endl;
        }
    }

    return 0;
}
