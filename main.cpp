#include "common.h"
#include "chunk.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    Chunk chunk;

    const size_t constant = chunk.add_constant(1.2);
    chunk.write(static_cast<uint8_t>(OpCode::OP_CONSTANT), 123);
    chunk.write(constant, 123);
    chunk.write(static_cast<uint8_t>(OpCode::OP_RETURN), 123);
#ifdef DEBUG
    chunk.disassemble("test chunk");
#endif
    vm.interpret(&chunk);
    return 0;
}
