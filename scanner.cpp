#include "scanner.h"

#include <cstring>
#include <format>

bool Scanner::is_at_end() const {
    return mCurrent == mSource.end();
}

char Scanner::advance() {
    const char character = *mCurrent;
    ++mCurrent;
    return character;
}

bool Scanner::match(const char expected) {
    if (is_at_end()) return false;
    if (*mCurrent != expected) return false;

    ++mCurrent;
    return true;
}

void Scanner::skip_whitespace() {
    for (;;) {
        switch (char c = peek()) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                ++mLine;
                advance();
                break;
            case '/':
                if (peek_next() == '/')
                    while (peek() != '\n' && !is_at_end())
                        advance();
                else
                    return;
                break;
            default:
                return;
        }
    }
}

char Scanner::peek() const {
    return *mCurrent;
}

char Scanner::peek_next() const {
    if (is_at_end()) return '\0';
    const std::string::iterator next_it = mCurrent + 1;
    return *next_it;
}

TokenType Scanner::check_keyword(const int start, const int length, const std::string_view rest, const TokenType type) const {
    if (mCurrent - mStart == start + length) {
        const auto keywordBegin = mStart + start;
        const auto keywordEnd = keywordBegin + length;

        if (std::equal(keywordBegin, keywordEnd, rest.begin(), rest.end()))
            return type;
    }

    return TokenType::IDENTIFIER;
}


Token Scanner::make_token(const TokenType type) const {
    Token token {type, std::string(mStart, mCurrent), mLine};
    return token;
}

Token Scanner::make_error_token(const std::string &message) const {
    Token token { TokenType::ERROR, message, mLine};
    return token;
}

Token Scanner::make_string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n')
            ++mLine;
        advance();
    }

    if (is_at_end()) return make_error_token("Unterminated string");

    // found "
    advance();
    return make_token(TokenType::STRING);
}

static bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

Token Scanner::make_number() {
    while (is_digit(peek())) advance();

    // look for fractional part
    if (peek() == '.' && is_digit(peek_next())) {
        advance();

        while (is_digit(peek())) advance();
    }

    return make_token(TokenType::NUMBER);
}

TokenType Scanner::identifier_type() {
    switch (*mStart) {
        case 'a': return check_keyword(1, 2, "nd", TokenType::AND);
        case 'c': return check_keyword(1, 3, "ast", TokenType::RETURN);
        case 'e': return check_keyword(1, 3, "lse", TokenType::ELSE);
        case 'f':
            if (mCurrent - mStart > 1) {
                switch (char newChar = *(mStart + 1)) {
                    case 'a': return check_keyword(2, 3, "lse", TokenType::FALSE);
                    case 'o': return check_keyword(2, 1, "r", TokenType::FOR);
                    default: return TokenType::ERROR;
                }
            }
            break;
        case 'g': return check_keyword(1, 7, "rimoire", TokenType::CLASS);
        case 'i': return check_keyword(1, 1, "f", TokenType::IF);
        case 'm': return check_keyword(1, 5, "atron", TokenType::SUPER);
        case 'n': return check_keyword(1, 2, "ix", TokenType::NIL);
        case 'o': return check_keyword(1, 1, "r", TokenType::OR);
        case 'r': return check_keyword(1, 3, "une", TokenType::VAR);
        case 's':
            if (mCurrent - mStart > 1) {
                switch (char newChar = *(mStart + 1)) {
                    case 'p': return check_keyword(2, 3, "ell", TokenType::CLASS);
                    case 'u': return check_keyword(2, 4, "mmon", TokenType::PRINT);
                    default: return TokenType::ERROR;
                }
            }
            break;
        case 't':
            if (mCurrent - mStart > 1) {
                switch (char newChar = *(mStart + 1)) {
                    case 'h': return check_keyword(2, 2, "is", TokenType::THIS);
                    case 'r': return check_keyword(2, 2, "ue", TokenType::TRUE);
                    default: return TokenType::ERROR;
                }
            }
            break;
        case 'w': return check_keyword(1, 4, "hile", TokenType::WHILE);

        default: return TokenType::ERROR;
    }
}

Token Scanner::make_identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();
    return make_token(identifier_type());
}


Token Scanner::scan_token() {
    skip_whitespace();

    mStart = mCurrent;
    if (is_at_end()) return make_token(TokenType::END_OF_FILE);

    char c = advance();
    if (is_alpha(c))
        return make_identifier();
    if (is_digit((c)))
        return make_number();

    switch (c) {
        case '(': return make_token(TokenType::LEFT_PAREN);
        case ')': return make_token(TokenType::RIGHT_PAREN);
        case '{': return make_token(TokenType::LEFT_BRACE);
        case '}': return make_token(TokenType::RIGHT_BRACE);
        case ';': return make_token(TokenType::SEMICOLON);
        case ',': return make_token(TokenType::COMMA);
        case '.': return make_token(TokenType::DOT);
        case '-': return make_token(TokenType::MINUS);
        case '+': return make_token(TokenType::PLUS);
        case '/': return make_token(TokenType::SLASH);
        case '*': return make_token(TokenType::STAR);
        case '!': return make_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=': return make_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<': return make_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>': return make_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"': return make_string();

        default: return make_error_token("Unexpected character");
    }
}
