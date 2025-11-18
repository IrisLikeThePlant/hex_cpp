#pragma once

#include <array>
#include <memory>
#include <string>

#include "scanner.h"
#include "vm.h"

enum class Precedence {
    NONE,           //
    ASSIGNMENT,     // =
    OR,             // or
    AND,            // and
    EQUALITY,       // == !=
    COMPARISON,     // < > <= >=
    TERM,           // + -
    FACTOR,         // * /
    UNARY,          // ! -
    CALL,           // . ()
    PRIMARY,
};

typedef void (*ParseFn)();

struct ParseRule {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
};



class Parser {
    Token mCurrent;
    Token mPrevious;
    Chunk *mCompilingChunk;
    std::unique_ptr<Scanner> mpScanner;

public:
    bool mHadError = false;
    bool mPanicMode = false;

    Parser() = default;
    ~Parser() = default;

    void advance();
    void expression();
    void consume(TokenType type, const std::string &message);
    void emit_byte(uint8_t byte) const;
    void emit_bytes(uint8_t byte1, uint8_t byte2) const;
    void end_compiler() const;
    static void number();
    static void grouping();
    static void unary();
    static void binary();
    void parse_precedence(Precedence precedence);
    [[nodiscard]] uint8_t make_constant(Value value) const;
    [[nodiscard]] bool compile(std::string &source, Chunk *chunk);

    [[nodiscard]] Token* get_current() {return &mCurrent;}
    [[nodiscard]] Token* get_previous() {return &mPrevious;}
};

inline Parser parser;

inline constexpr std::array rules = {
    ParseRule{Parser::grouping, nullptr,        Precedence::NONE}, //LEFT_PAREN
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //RIGHT_PAREN
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //LEFT_BRACE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //RIGHT_BRACE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //COMMA
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //DOT
    ParseRule{Parser::unary,    Parser::binary, Precedence::TERM}, //MINUS
    ParseRule{nullptr,          Parser::binary, Precedence::TERM}, //PLUS
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //SEMICOLON
    ParseRule{nullptr,          Parser::binary, Precedence::FACTOR}, //SLASH
    ParseRule{nullptr,          Parser::binary, Precedence::FACTOR}, //STAR
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //BANG
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //BANG_EQUAL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //EQUAL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //EQUAL_EQUAL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //GREATER
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //GREATER_EQUAL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //LESS
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //LESS_EQUAL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //IDENTIFIER
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //STRING
    ParseRule{Parser::number,   nullptr,        Precedence::NONE}, //NUMBER
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //AND
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //CLASS
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //ELSE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //FALSE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //FOR
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //FUN
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //IF
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //NIL
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //OR
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //PRINT
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //RETURN
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //SUPER
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //THIS
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //TRUE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //VAR
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //WHILE
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //ERROR
    ParseRule{nullptr,          nullptr,        Precedence::NONE}, //EOF
};