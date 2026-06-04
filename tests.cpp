#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"
#include "Token.h"

void test_lexer(const std::string& name, const std::string& input) {
    std::cout << "Testing: " << name << " with input: [" << input << "]" << std::endl;
    try {
        Lexer lexer(input);
        auto tokens = lexer.get_tokens();
        for (const auto& token : tokens) {
            std::cout << "  Token: " << Token::type_to_string(token.type) << " Value: [" << token.value << "]" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  Error: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "  Unknown error occurred!" << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
}

int main() {
    // 1. EOF crash/OOB test
    test_lexer("Empty string", "");
    test_lexer("Single char", "a");

    // 2. String literal consumption test
    test_lexer("String literal followed by char", "\"hello\"+");

    // 3. Operator consumption test
    test_lexer("Operator + ident", "++a");

    // 4. Multi-char operator
    test_lexer("Multi-char operator", "==");

    // 5. OOB check
    test_lexer("End of string check", "123");
    test_lexer("Two words", "abc def");

    // 6. Escape sequence at EOF
    test_lexer("Escape at EOF", "\"\\");

    // multiple decimal points in float
    test_lexer("mult decimal points", "1.2.3");

    // Unterminated string
    test_lexer("Unterminated string", "\"hello");

    // Full function
    test_lexer("Full function", "void main() { int x = 5; printf(\"%s\", x); }");

    return 0;
}
