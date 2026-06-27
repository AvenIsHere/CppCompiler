//
// Created by aven on 27/06/2026.
//

#include "Preprocessor.h"

#include <utility>

Preprocessor::Preprocessor(std::string given_input, std::filesystem::path given_file_path) : input(std::move(given_input)), state(PreprocessorState::NONE), file_path(std::move(given_file_path)) {}

std::string Preprocessor::process() {

    std::string output;

    int line = 0;
    int column = 0;
    int index = 0;

    char character = input.at(index);

    while (character != '\0') {


        index++;
        character = input.at(index);
    }

    return output;

}
