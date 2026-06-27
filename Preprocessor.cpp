//
// Created by aven on 27/06/2026.
//

#include "Preprocessor.h"

#include <sstream>
#include <utility>

Preprocessor::Preprocessor(std::string given_input) : input(std::move(given_input)), state(PreprocessorState::NONE) {}

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
