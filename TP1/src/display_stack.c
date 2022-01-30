/*
 * asmVariableSet.c
 *
 *  Created on: Jan 30, 2022
 *      Author: angela
 *
 *
 *
 *      the stack in intel 86 grows down.
 *      inbetween esp and ebp we have the frame of the calling function.
 *      In this frame we find at least arguments and variables.
 *		According to the type and type size this information is stored differently.
 *
 *
 */

#include <stdint.h>
#include <stdio.h>
	static uint32_t  sp,  sbp ;
	uint32_t stackPointer,  stackBasePointer, p;
int main() {

    register uint32_t val asm("ebp");

    asm ("movl %%esp, %[stackPointer] \n\t"
    			 "movl %%ebp, %[stackBasePointer] "
    		: [stackPointer] "=r" (stackPointer), [stackBasePointer] "=r" (stackBasePointer)
    		: "r" (p)
    		: "%esp"
    		);

	printf("VAL: %zu x = %zu; y = %zu size=%d \n",val,  stackPointer, stackBasePointer, stackBasePointer - stackPointer);
	sp = stackPointer;
	sbp = stackBasePointer;
	storeStackPointer(stackPointer,  stackBasePointer);
}


void storeStackPointer ( ) {

		asm ("movl %%esp, %[stackPointer] \n\t"
			 "movl %%ebp, %[stackBasePointer] "
		: [stackPointer] "=r" (stackPointer), [stackBasePointer] "=r" (stackBasePointer)
		: "r" (p)
		: "%esp"
		);


		printf("x = %zu; y = %zu size=%d \n", stackPointer, stackBasePointer, stackBasePointer - stackPointer);
		printf("x = %d; y = %d size=%d \n", sp - stackPointer, sbp - stackBasePointer, stackBasePointer - stackPointer);

}
