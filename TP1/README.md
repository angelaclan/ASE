# TP 01 ASE


## Display_stack Explaination
ESP is the register which point to the top of the stack (Stack Pointer).
EBP is the register which point to the top of the stack(Base Pointer).

This program disply the size of stack frame as well as printing the position of pointer ESP and EBP.
From the outcome we can see the position of EBP is larger than ESP, and the stack in intel 86 grows from top to down.
In between EBP and ESP it stocks arguments and variables. According to the type and type size, these data are stored differently.

## Try...Throw Explaination
This function simplifies the exuction process when encounter errors. In stead of return to previous invoked function, throw() jump back directly to the try() function and returns the value which returned by throw()

## Important Note

Following the new version of the gcc compiler, the 32-bit execution posed problem, for the programe to work, a neccesarry change in registers is made :
- esp changed by rsp; ebp changed by rbp, in the asm() instruction.

## Compilation
Launch command ```make```  will compile excutable file try, try_mul, display_stack.
