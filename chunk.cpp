#include "chunk.h"

#include <iostream>

void Chunk::write(const uint8_t byte, const int line) {
    code.push_back(byte);
    lines.push_back(line);
}

size_t Chunk::add_constant(const Value value) {
    constants.write(value);
    return constants.count() - 1;
}

uint8_t Chunk::get_code_at(const int offset) const {
    return code[offset];
}

uint8_t* Chunk::get_code() {
    return &code[0];
}

Value Chunk::get_constant_at(const int offset) const {
    return constants.get_value_at(offset);
}

void Chunk::print_value(const int offset) const {
    constants.print_value(offset);
}




#ifdef DEBUG
void Chunk::disassemble(const std::string &name) const {
    std::cout << "== " << name.c_str() << " ==" << std::endl;

    for (int offset = 0; offset < code.size();) {
        offset = this->disassemble_instruction(offset);
    }
}

static int constant_instruction(const std::string &name, const Chunk* chunk, const int offset) {
    const uint8_t constant = chunk->get_code_at(offset + 1);
    std::printf("%-16s %4d '", name.c_str(), constant);
    chunk->print_value(constant);
    std::printf("'\n");
    return offset + 2; // OP_CONSTANT is two bytes
}

static int simple_instruction(const std::string &name, const int offset) {
    std::printf(/*"%s\n"*/ "%s", name.c_str());
    return offset + 1; // OP_RETURN is two bytes
}

int Chunk::disassemble_instruction(const int offset) const {
    std::printf("%04d ", offset);

    if (offset > 0 && lines[offset] == lines[offset - 1])
        printf("   | ");
    else
        printf("%4d ", lines[offset]);

    switch (const uint8_t instruction = code[offset]) {
        case static_cast<int>(OpCode::OP_CONSTANT):
            return constant_instruction("OP_CONSTANT", this, offset);
        case static_cast<int>(OpCode::OP_RETURN):
            return simple_instruction("OP_RETURN", offset);
        default:
            std::printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
#endif

