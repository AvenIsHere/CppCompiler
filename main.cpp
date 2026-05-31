#include <vector>

#include "CompilerProgram.h"

int main(const int argc, char** argv) {

    const std::vector<std::string> args(argv + 1, argv + argc);
    return CompilerProgram(args).run();
}