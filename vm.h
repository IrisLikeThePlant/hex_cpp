#pragma once

#include "chunk.h"
#include "compiler.h"

enum class InterpretResult : uint8_t {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
};

class VM {
    Chunk *mpChunk{};
    uint8_t* ip{};
    std::vector<Value> mValueStack;
public:
    VM() = default;
    ~VM() = default;

    InterpretResult interpret(std::string &source);
    InterpretResult run();
    void push(Value value);
    Value pop();
};

inline VM vm;