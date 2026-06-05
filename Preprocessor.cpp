//
// Created by aven on 05/06/2026.
//

#include "Preprocessor.h"

#include <fstream>
#include <utility>

Preprocessor::Preprocessor(std::string input, std::filesystem::path file_path, const std::vector<std::filesystem::path> &given_include_dirs)
    : code(std::move(input)), code_path(std::move(file_path)), include_dirs(given_include_dirs), output_code_pos(0), line(1), code_pos(0) {
}

std::string Preprocessor::process() {
    while (code_pos < code.length()) {
        const size_t start_of_line = code_pos;
        size_t temp_pos = skip_whitespace(code_pos);
        
        if (temp_pos < code.length() && code[temp_pos] == '#') {
            output_code.append(code.substr(output_code_pos, temp_pos - output_code_pos));
            code_pos = temp_pos;
            handle_directive();
            output_code_pos = code_pos;
        } else {
            code_pos = code.find('\n', code_pos);
            if (code_pos == std::string::npos) {
                code_pos = code.length();
            } else {
                code_pos++;
                line++;
            }
        }
    }
    output_code.append(code.substr(output_code_pos));
    return output_code;
}

std::string Preprocessor::include_file(const std::filesystem::path& file_path) {
    if (std::ranges::find(included_files, file_path) == included_files.end()) {
        included_files.push_back(file_path);
        std::ifstream file(file_path);
        if (!file.is_open()) throw std::runtime_error("Could not open included file: " + file_path.string());
        std::ostringstream file_stream;
        file_stream << file.rdbuf();
        return file_stream.str();
    }
    return "\n";
}

std::filesystem::path Preprocessor::get_file_path(size_t pos) {
    size_t path_pos = skip_whitespace(pos);
    if (path_pos >= code.length()) throw std::runtime_error(std::format("Unexpected end of file in include directive on line {}", line));

    bool local_file;

    auto character = code[path_pos];
    if (character == '"') local_file = true;
    else if (character == '<') local_file = false;
    else throw std::runtime_error(std::format("Invalid token after include directive on line {}", line));

    std::string given_path;
    path_pos++;
    while (path_pos < code.length()) {
        character = code[path_pos];
        if (local_file ? character == '"' : character == '>') break;
        if (character == '\n') throw std::runtime_error(std::format("Unterminated include directive on line {}", line));
        given_path.push_back(character);
        path_pos++;
    }

    if (path_pos >= code.length()) throw std::runtime_error(std::format("Unterminated include directive on line {}", line));

    std::filesystem::path include_path(given_path);
    if (include_path.is_absolute()) return include_path;

    if (local_file) {
        include_path = code_path.parent_path() / include_path;
        if (exists(include_path)) return weakly_canonical(include_path);
        include_path = std::filesystem::path(given_path);
    }

    for (const auto& dir : include_dirs) {
        include_path = dir / include_path;
        if (exists(include_path)) return weakly_canonical(include_path);
        include_path = std::filesystem::path(given_path);
    }

    throw std::runtime_error(std::format("Unknown file path in include directive on line {}", line));
}

void Preprocessor::handle_directive() {
    if (code[code_pos] != '#') throw std::runtime_error("handle_directive called on non-directive");
    size_t directive_pos = code_pos + 1;
    directive_pos = skip_whitespace(directive_pos);

    std::string directive;
    while (directive_pos < code.length() && isalpha(code[directive_pos])) {
        directive.push_back(code[directive_pos]);
        directive_pos++;
    }
    if (directive == "include") {
        const std::filesystem::path include_path = get_file_path(directive_pos);
        std::string file_content = include_file(include_path);

        if (file_content != "\n") {
            Preprocessor sub_preprocessor(file_content, include_path, include_dirs);
            sub_preprocessor.included_files = included_files;
            file_content = sub_preprocessor.process();
            included_files = sub_preprocessor.included_files;
        }

        const size_t end_pos = code.find('\n', directive_pos);

        output_code.append(file_content);
        if (end_pos == std::string::npos) {
            code_pos = code.length();
        } else {
            code_pos = end_pos;
        }
    }
}

size_t Preprocessor::skip_whitespace(size_t pos) const {
    while (pos < code.length() && code[pos] == ' ') {
        pos++;
    }
    return pos;
}
