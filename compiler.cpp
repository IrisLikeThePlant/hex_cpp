#include "compiler.h"

void error_at(const Token *token, const std::string &message) {
    if (parser.mPanicMode) return;
    parser.mPanicMode = true;

    std::fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TokenType::END_OF_FILE) {
        std::fprintf(stderr, " at end");
    }
    else if (token->type == TokenType::ERROR) {
        ///
    }
    else {
        std::fprintf(stderr, " at '%.*s'", static_cast<int>(token->lexeme.length()), token->lexeme.c_str());
    }

    std::fprintf(stderr, ": %s\n", message.c_str());
    parser.mHadError = true;
}

void error_at_current(const std::string &message) {
    error_at(parser.get_current(), message);
}

void error(const std::string &message) {
    error_at(parser.get_previous(), message);
}

void Parser::advance() {
    mPrevious = mCurrent;

    for (;;) {
        mCurrent = mpScanner->scan_token();
        if (mCurrent.type != TokenType::ERROR) break;

        error_at_current(mCurrent.lexeme);
    }
}

void Parser::expression() {
    parse_precedence(Precedence::ASSIGNMENT);
}

void Parser::consume(const TokenType type, const std::string &message) {
    if (mCurrent.type == type) {
        advance();
        return;
    }

    error_at_current(message);
}

void Parser::emit_byte(const uint8_t byte) const {
    mCompilingChunk->write(byte, mPrevious.line);
}

void Parser::emit_bytes(const uint8_t byte1, const uint8_t byte2) const {
    emit_byte(byte1);
    emit_byte(byte2);
}

void emit_return() {
    parser.emit_byte(static_cast<uint8_t>(OpCode::RETURN));
}

void emit_constant(Value value) {
    parser.emit_bytes(static_cast<uint8_t>(OpCode::CONSTANT), parser.make_constant(value));
}

void Parser::end_compiler() const {
    emit_return();
#ifdef DEBUG
    if (!mHadError) {
        mCompilingChunk->disassemble("code");
    }
#endif
}

const ParseRule *get_rule(const TokenType type) {
    return &rules[static_cast<size_t>(type)];
}

void Parser::number() {
    const Value value = std::stod(parser.get_previous()->lexeme);
    emit_constant(value);
}

void Parser::grouping() {
    parser.expression();
    parser.consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
}

void Parser::unary() {
    const TokenType operatorType = parser.get_previous()->type;

    parser.parse_precedence(Precedence::UNARY);

    switch (operatorType) {
        case TokenType::MINUS: parser.emit_byte(static_cast<uint8_t>(OpCode::NEGATE)); break;
        default:
            return; // unreachable
    }
}

void Parser::binary() {
    const TokenType operatorType = parser.get_previous()->type;

    const ParseRule *rule = get_rule(operatorType);
    parser.parse_precedence(static_cast<Precedence>(static_cast<int>(rule->precedence) + 1));

    switch (operatorType) {
        case TokenType::PLUS: parser.emit_byte(static_cast<uint8_t>(OpCode::ADD)); break;
        case TokenType::MINUS: parser.emit_byte(static_cast<uint8_t>(OpCode::SUBTRACT)); break;
        case TokenType::STAR: parser.emit_byte(static_cast<uint8_t>(OpCode::MULTIPLY)); break;
        case TokenType::SLASH: parser.emit_byte(static_cast<uint8_t>(OpCode::DIVIDE)); break;
        default:
            return; //unreachable
    }
}


void Parser::parse_precedence(Precedence precedence) {
    advance();
    ParseFn prefix_rule = get_rule(get_previous()->type)->prefix;
    if (prefix_rule == nullptr) {
        error("Expect expression.");
        return;
    }

    prefix_rule();

    while (precedence <= get_rule(get_current()->type)->precedence) {
        advance();
        const ParseFn infix_rule = get_rule(get_previous()->type)->infix;
        infix_rule();
    }
}



uint8_t Parser::make_constant(const Value value) const {
    const size_t constant = mCompilingChunk->add_constant(value);
    if (constant > SIZE_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }
    return static_cast<uint8_t>(constant);
}


bool Parser::compile(std::string &source, Chunk* chunk) {
    mpScanner = std::make_unique<Scanner>(source);
    mCompilingChunk = chunk;

    mHadError = false;
    mPanicMode = false;

    advance();
    expression();
    consume(TokenType::END_OF_FILE, "Expect end of expression.");

    end_compiler();
    return !mHadError;
}