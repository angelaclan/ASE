/*
 * try.c
 *
 *  Created on: Jan 30, 2022
 *      Author: angela
 */


#include <assert.h>
#include "try.h"
/*This function will execute the function f() with the parameter arg.
  The value returned by try() is the value returned by the function f()*/
int try(struct ctx_s *pctx, func_t *f, int arg) {
	GET_STACK_POINTERS(pctx->stackPointer, pctx->stackBasePointer);
	return f(arg);
}

/*This function will return to a function call context previously stored in the context pctx by function try().
 * The value r will then be the one "returned" by the invocation of the function through try()*/
int throw(struct ctx_s *pctx, int r){
	/*Save r value in a static var when change register for accessability later on*/

	static int throw_r = 0;

	throw_r = r;

	SET_STACK_POINTERS(pctx->stackPointer, pctx->stackBasePointer);
	assert(pctx->stackBasePointer > pctx->stackPointer);

	/*Since register changes, the copy of r value can be returned as it was saved staticly*/
	return throw_r;
}

