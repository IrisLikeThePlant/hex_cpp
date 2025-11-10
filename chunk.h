#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "value.h"

enum class OpCode : uint8_t {
    OP_CONSTANT,
    OP_RETURN,
};

class Chunk {
    std::vector<uint8_t> code;
    std::vector<int> lines;
    ValueArray constants;

public:
    Chunk() = default;
    ~Chunk() {
        code.clear();
        lines.clear();
    };

    void write(uint8_t byte, int line);
    size_t add_constant(Value value);
    void print_value(int offset) const;
    [[nodiscard]] uint8_t get_code_at(int offset) const;

#ifdef DEBUG
    void disassemble(const std::string &name) const;
    [[nodiscard]] int disassemble_instruction(int offset) const;
#endif

};