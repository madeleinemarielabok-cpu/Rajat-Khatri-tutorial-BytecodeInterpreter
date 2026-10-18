#include "InterpreterInclude.h"
using namespace std;
int main()
{
    vector<Instruction> program = {
        {PUSH,100},  // Address
        {PUSH,42},   // Value
        {STORE,0},   // Store 42 at memory[100], consumes both
        {PUSH,100},  // Push the address again for LOAD
        {LOAD,0},    // Pops address 100, pushes memory[100] (42)
        {PRINT,0},   // Prints 42
    };

    BytecodeInterpreter interpreter(program);
    try
    {
        interpreter.execute();
    }
    catch(const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}