//
// Created by aven on 05/06/2026.
//

#ifndef CPPCOMPILER_PREPROCESSOR_H
#define CPPCOMPILER_PREPROCESSOR_H
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>


class Preprocessor {
public:

    Preprocessor(std::string input, std::filesystem::path file_path, const std::vector<std::filesystem::path> &given_include_dirs);

    std::string process();

    std::vector<std::filesystem::path> included_files;

private:

    size_t skip_whitespace(size_t pos) const;

    void handle_directive();

    std::filesystem::path get_file_path(size_t pos);
    std::string include_file(const std::filesystem::path& file_path);

    void expand_macro(std::string input, std::string output);

    void handle_if(std::string condition);

    std::string code;
    std::filesystem::path code_path;
    std::vector<std::filesystem::path> include_dirs;

    std::string output_code;
    size_t output_code_pos;

    size_t line;
    size_t code_pos;

    std::unordered_map<std::string, std::string> macros;
    std::vector<bool> if_stack;

};


#endif //CPPCOMPILER_PREPROCESSOR_H
