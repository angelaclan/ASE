/*
 * try.h
 *
 *  Created on: Jan 30, 2022
 *      Author: angela
 */




/**
 * rbp = 64bits bp address base pointer
 * rsp = 64bits sp address stack pointer
 *
 **/

/*
 * registry -> [pointer]-> c variable
 * */
#define GET_STACK_POINTERS(stack, base) asm ( \
	"mov %%rsp, %[stackPointer] " "\n\t"  \
	"mov %%rbp, %[stackBasePointer] "\
	: [stackPointer] "=r" (stack), [stackBasePointer] "=r" (base)\
	)
/*
 *  c variable ->[pointer]->registry
 * */

#define SET_STACK_POINTERS(stack, base) asm ( \
	"mov %[stackPointer], %%rsp " "\n\t"  \
	"mov %[stackBasePointer], %%rbp"\
	: \
	: [stackPointer] "r" (stack), [stackBasePointer] "r" (base)\
	)


struct ctx_s {
	void * stackBasePointer;
	void * stackPointer;
};

typedef  int(func_t)(int) ;

int try(struct ctx_s *pctx, func_t *f, int arg);
int throw(struct ctx_s *pctx, int r);


