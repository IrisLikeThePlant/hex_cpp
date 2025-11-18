#include <iostream>
#include <fstream>
#include <sstream>

#include "common.h"
#include "chunk.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        std::printf("> ");

        if (!std::fgets(line, sizeof(line), stdin)) {
            std::printf("\n");
            break;
        }
        std::string strLine(line); 
        vm.interpret(strLine);
    }
}

static void run_file(const char* path) {
    std::stringstream buffer;

    // reading the file contents
    std::ifstream fileStream(path);
    if (fileStream)
        buffer << fileStream.rdbuf();
    else {
        std::cerr << "Could not open file \"" << path << "\"." << std::endl;
        exit(74);
    }
    fileStream.close();

    std::string source = buffer.str();
    InterpretResult result = vm.interpret(source);

    if (result == InterpretResult::INTERPRET_COMPILE_ERROR) exit(65);
    if (result == InterpretResult::INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        repl();
    }
    else if (argc == 2) {
        run_file(argv[1]);
    }
    else {
        std::cerr << "Usage: hex [path]" << std::endl;
    }

    return 0;
}
