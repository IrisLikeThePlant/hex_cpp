#pragma once

#include "common.h"

enum class TokenType : uint8_t {
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE,
    FOR, FUN, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS,
    TRUE, VAR, WHILE,

    ERROR,
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line = -1;
};

class Scanner {
    std::string::iterator mStart;
    std::string::iterator mCurrent;
    std::string mSource;
    int mLine;

    [[nodiscard]] bool is_at_end() const;
    [[nodiscard]] Token make_token(TokenType type) const;
    [[nodiscard]] Token make_error_token(const std::string &message) const;
    [[nodiscard]] Token make_string();
    [[nodiscard]] Token make_number();
    [[nodiscard]] Token make_identifier();
    char advance();
    [[nodiscard]] bool match(char expected);
    void skip_whitespace();
    [[nodiscard]] char peek() const;
    [[nodiscard]] char peek_next() const;
    [[nodiscard]] TokenType check_keyword(int start, int length, std::string_view rest, TokenType type) const;
    [[nodiscard]] TokenType identifier_type();

public:
    explicit Scanner(const std::string &source) {
        mSource = source;
        mStart = mSource.begin();
        mCurrent = mSource.begin();
        mLine = 1;
    }
    ~Scanner() = default;

    Token scan_token();
};
