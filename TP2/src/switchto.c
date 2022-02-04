#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <switchto.h>

void init_ctx(Ctx_s *ctx_s, func_t f, void *args) {
	ctx_s->entryPointFunct = f;
	ctx_s->args = args;
	ctx_s->state = STATE_NOT_START;
	ctx_s->rsp = ctx_s->stack + STACK_SIZE - 8;
	ctx_s->rbp = ctx_s->stack + STACK_SIZE - 8;
}

void switch_to_ctx(Ctx_s * from, Ctx_s * to) {
	static Ctx_s *ctx_from = NULL;
	static Ctx_s *ctx_to = NULL;
	ctx_from = from;
	ctx_to = to;

	assert(ctx_to != NULL);
	assert(ctx_to->state < STATE_RUNNING);

	/*in case of ctx_from exist and is running, save its SP and set state == running */
	if (ctx_from != NULL) {
		assert(ctx_from->state == STATE_RUNNING);
		asm("mov %%rsp, %0" "\n\t" "mov %%rbp, %1"
				:"=r"(ctx_from->rsp),"=r"(ctx_from->rbp)
				 :
				 :);
		ctx_from->state = STATE_PAUSED;
	}
	/*in the case of ctx_from is null, set directly the SP of the ctx_to context */
	asm("mov %0, %%rsp" "\n\t" "mov %1, %%rbp"
	:
	:"r"(ctx_to->rsp),"r"(ctx_to->rbp)
	:);


	if (ctx_to->state == STATE_NOT_START) {
		start_state(ctx_to);
	/*while the state of the target context is running, "return" simply brings back its context*/
	} else {
		assert(ctx_to->state == STATE_PAUSED);
		ctx_to->state = STATE_RUNNING;
	}
}

void start_state(Ctx_s *ctx) {
	ctx->state = STATE_RUNNING;
	ctx->entryPointFunct(ctx->args);
}






