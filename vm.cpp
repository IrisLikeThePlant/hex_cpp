#include "vm.h"

InterpretResult VM::run() {
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (mpChunk->get_constant_at(READ_BYTE()))
#define BINARY_OP(op) \
    do { \
        Value b = pop(); \
        Value a = pop(); \
        push(a op b); \
    } while (false)

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
            case static_cast<uint8_t>(OpCode::CONSTANT): {
                const Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case static_cast<uint8_t>(OpCode::ADD): {
                BINARY_OP(+);
                break;
            }
            case static_cast<uint8_t>(OpCode::SUBTRACT): {
                BINARY_OP(-);
                break;
            }
            case static_cast<uint8_t>(OpCode::MULTIPLY): {
                BINARY_OP(*);
                break;
            }
            case static_cast<uint8_t>(OpCode::DIVIDE): {
                BINARY_OP(/);
                break;
            }
            case static_cast<uint8_t>(OpCode::NEGATE): {
                push(-pop());
                break;
            }
            case static_cast<uint8_t>(OpCode::RETURN): {
                print_value(pop());
                std::printf("\n");
                return InterpretResult::INTERPRET_OK;
            }
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult VM::interpret(std::string &source) {
    Chunk chunk = Chunk();

    if (!parser.compile(source, &chunk)) {
        return InterpretResult::INTERPRET_COMPILE_ERROR;
    }

    mpChunk = &chunk;
    ip = mpChunk->get_code();

    const InterpretResult result = run();

    return result;
}

void VM::push(const Value value) {
    mValueStack.push_back(value);
}

Value VM::pop() {
    const Value value = mValueStack.back();
    mValueStack.pop_back();
    return value;
}
