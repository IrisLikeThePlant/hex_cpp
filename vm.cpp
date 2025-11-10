#include "vm.h"

InterpretResult VM::run() {
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (mpChunk->get_constant_at(READ_BYTE()))

    for (;;) {
        #ifdef DEBUG
        std::printf("          ");
        for (const auto value: mValueStack) {
            std::printf("[ ");
            print_value(value);
            std::printf(" ]");
        }
        std::printf("\n");
        // mpChunk->disassemble_instruction(((int)(ip - mpChunk->get_code())));
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case static_cast<uint8_t>(OpCode::OP_CONSTANT): {
                const Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case static_cast<uint8_t>(OpCode::OP_RETURN): {
                print_value(pop());
                std::printf("\n");
                return InterpretResult::INTERPRET_OK;
            }
        }
    }

#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult VM::interpret(Chunk *pChunk) {
    mpChunk = pChunk;
    ip = mpChunk->get_code();
    return run();
}

void VM::push(const Value value) {
    mValueStack.push_back(value);
}

Value VM::pop() {
    const Value value = mValueStack.back();
    mValueStack.pop_back();
    return value;
}
