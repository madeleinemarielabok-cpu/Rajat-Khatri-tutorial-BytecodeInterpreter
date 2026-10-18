#ifndef INTERPRETER_H 
#define INTERPRETER_H
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
using namespace std;
// The operation performed by a bytecode instruction.
enum Opcode
{
    PUSH,// Push operand onto the stack.
    ADD,// Add the top two stack values.
    SUB,// Subtract the top value from the value below it.
    MUL,// Multiply the top two stack values.
    DIV,// Divide the second value by the top value.
    JUMP, // Jump to the instruction at operand.
    JUMP_IF_ZERO,// Jump if the top stack value is zero.
    JUMP_IF_NOT_ZERO,// Jump if the top stack value is not zero.
    CALL,// Call a function at the operand address.
    RETURN, // Return from a function call.
    STORE,// Store a value in memory.
    LOAD,// Load a value from memory.
    PRINT,// Print the top stack value.
    HALT,// Stop program execution.
};
// Represents one bytecode instruction.
struct Instruction
{
    Opcode op;  // Operation to execute.
    int operand;// Value used by PUSH
};
// Executes bytecode instructions using a stack and memory.
class BytecodeInterpreter
{
    private:
    vector<Instruction> bytecode;// The program's instructions.
    stack<int> stack;// Data stack used by operations.
    std::stack<int> callStack; // Stores return addresses for CALL instructions.
    unordered_map<int,int> memory;// Maps memory addresses to stored values.
    size_t ip;  // Instruction pointer for the next instruction.

    public:
        // Creates an interpreter with the specified bytecode program.
        BytecodeInterpreter(const vector<Instruction> &code);
        // Executes instructions until HALT or the end of the program.
        void execute();
};
#endif //INTERPRETER_H