#include <assert.h>
#include "try.h"

/*This function will execute the function f() with the parameter arg.
  The value returned by try() is the value returned by the function f()*/
int try(struct ctx_s *pctx, func_t *f, int arg) {
	
	pctx >init = CTX_INIT;
	/*reference to top stackpointer and memorize context*/
	asm("mov %%rsp, %0" : "= r" (pctx->esp)); 
	/*reference to base stackpointer and memorize context*/
	asm("mov %%rbp, %0" : "= r" (pctx->ebp)); 
	/*call the function*/
	return f(arg);
}

/*This function will return to a function call context previously stored in the context pctx by function try(). The value r will then be the one "returned" by the invocation of the function through try()*/
int throw(struct ctx_s *pctx, int r){
	/*Save r value in a static var when change register for accessability later on*/
	static int throw_r = 0;
	assert(CTX_INIT == pctx->init);
	pctx->init = 0; 
	throw_r = r;
	/*Place ebp and esp from context into registers*/
	asm("movl %0, %%esp" : : "r" (pctx->ctx_esp));
    asm("movl %0, %%ebp" : : "r" (pctx->ctx_ebp));
	/*Since register changes, the copy of r value can be returned as it was saved staticly*/
	return throw_r;
	
	

}


