#include "InterpreterInclude.h"
using namespace std;
// Keep a copy of the program and begin execution at its first instruction.
BytecodeInterpreter::BytecodeInterpreter(const vector<Instruction> &code)
: bytecode(code), ip(0) {}
void BytecodeInterpreter::execute()
{
    // Execute instructions until the instruction pointer moves past the program.
    while(ip<bytecode.size())
    {
        Instruction instr= bytecode[ip];
        switch(instr.op)
        {
            case PUSH:
                // Place the instruction's operand on the data stack.
                stack.push(instr.operand);
                break;
            case ADD:
            {
                // Pop two values, add them, and push the result.
                if (stack.size()<2)
                    throw runtime_error("Stack underflow");
                int b = stack.top();
                stack.pop();
                int a = stack.top();
                stack.pop();
                stack.push(a+b);
                break;
            }
            case SUB:
            {
                // Subtract the top value from the value beneath it.
                if (stack.size()<2)
                    throw runtime_error("Stack underflow");
                int b = stack.top();
                stack.pop();
                int a = stack.top();
                stack.pop();
                stack.push(a-b);
                break;
            }
            case MUL:
            {
                if (stack.size()<2)
                    throw runtime_error("Stack underflow");
                // Note: this currently reads the top value twice and pops only once.
                // It likely should pop b before reading a, as in the ADD and SUB cases.
                int b = stack.top();
                int a = stack.top();
                stack.pop();
                stack.push(a*b);
                break;
            }
            case DIV:
            {
                // Divide the second-from-top value by the top value.
                if (stack.size()<2)
                    throw runtime_error("Stack underflow");
                int b = stack.top();
                stack.pop();
                int a = stack.top();
                stack.pop();
                stack.push(a/b);
                break;
            }
            case JUMP:
                // Set the instruction pointer to the target. The common
                // increment below advances it to the target itself.
                if(instr.operand<0|| instr.operand >= bytecode.size())
                    throw runtime_error("Invalid JUMP target");
                ip = instr.operand-1; //-1 because we increment ip at the end
                break;
            case JUMP_IF_ZERO:
                // Check the top value and branch if it is zero.
                if(stack.empty())
                    throw runtime_error("Stack underflow on JUMP_IF_ZERO");
                if(stack.top()==0)
                {
                    if(instr.operand<0||instr.operand>=bytecode.size())
                    throw runtime_error("Invalid JUMP_IF_ZERO target");
                    ip = instr.operand-1;
                }
                 // The condition is consumed whether or not the branch is taken.
                stack.pop(); // remove the checked value
                break;
            case JUMP_IF_NOT_ZERO:
                // Check the top value and branch if it is nonzero.
                if(stack.empty())
                    throw runtime_error("Stack underflow on JUMP_IF_NOT_ZERO");
                if(stack.top()!=0)
                {
                    if(instr.operand<0||instr.operand>=bytecode.size())
                    throw runtime_error("Invalid JUMP_IF_NOT_ZERO target");
                    ip=instr.operand-1;
                }
                 // The condition is consumed whether or not the branch is taken.
                stack.pop(); //remove the checked value
                break;
            case CALL:
                // Save the current instruction so RETURN can resume after CALL.
                if(instr.operand<0||instr.operand>=bytecode.size())
                    throw runtime_error("Invalid CALL target");
                callStack.push(ip); //save return address
                ip=instr.operand-1; //jump to function
                break;
            case RETURN:
                // Restore the saved instruction pointer; the increment below
                // advances execution to the instruction after the CALL.
                if(callStack.empty())
                    throw runtime_error("RETURN without CALL");
                ip = callStack.top(); // Restore return address
                callStack.pop();
                break;
            case STORE: // Store top value of stack into memory
            {
                // Pop a value and then an address, and write the value to memory.
                if(stack.size()<2)
                    throw runtime_error("Stack underflow on STORE");
                int value = stack.top();
                stack.pop();
                int address = stack.top();
                stack.pop();
                memory[address] = value;
                break;
            }
            case LOAD: // Load value from memory and push onto stack
            {
                // Pop an address and push the value stored at that address.
                if(stack.empty())
                    throw runtime_error("Stack underflow onm LOAD");
                int address = stack.top();
                stack.pop();
                if(memory.find(address)==memory.end())
                    throw runtime_error("Invalid memory access at address "+to_string(address));
                    stack.push(memory[address]);
                    break;
            }
            case PRINT:
                // Display the top value without removing it from the stack.
                if(stack.empty())
                {
                    std::cerr << "Warning: Attempted to PRINT an empty stack." << std::endl;
                    break;
                }
                cout << stack.top()<<endl;
                break;
            case HALT:
                // Stop execution immediately.
                return;
            default:
                throw runtime_error("Unkown instruction");
        } 
        // Advance to the next instruction after handling the current one.
        ip++;
    }
}