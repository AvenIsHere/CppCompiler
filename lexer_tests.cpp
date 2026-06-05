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
    // EOF crash/OOB test
    test_lexer("Empty string", "");
    test_lexer("Single char", "a");

    // String literal consumption test
    test_lexer("String literal followed by char", "\"hello\"+");

    // Operator consumption test
    test_lexer("Operator + ident", "++a");

    // Multi-char operator
    test_lexer("Multi-char operator", "==");

    // OOB check
    test_lexer("End of string check", "123");
    test_lexer("Two words", "abc def");

    // Escape sequence at EOF
    test_lexer("Escape at EOF", "\"\\");

    // multiple decimal points in float
    test_lexer("mult decimal points", "1.2.3");

    // Unterminated string
    test_lexer("Unterminated string", "\"hello");

    // Full function
    test_lexer("Full function", "void main() { int x = 5; printf(\"%s\", x); }");

    // Lost characters in operators
    test_lexer("Lost characters", "+-");

    // Char literals
    test_lexer("Empty char literal", "''");
    test_lexer("Triple quote char literal", "'''");
    test_lexer("Valid char literal", "'a'");
    test_lexer("Escaped char literal", "'\\''");

    // Floating point starting with dot
    test_lexer("Floating point with dot", ".5");

    // Comments
    test_lexer("Single line comment", "int x; // comment\nint y;");
    test_lexer("Multi-line comment", "int x; /* comment */ int y;");

    return 0;
}
