#include <stdio.h>
#include <switchto.h>
#include <stdlib.h>
#include <assert.h>


/*static struct ctx_s *currentContext = NULL;
 static struct ctx_s *newContext;*/

void init_ctx(struct ctx_s *ctx_s, func_t f, void *args) {
	ctx_s->entryPointFunct = f;
	ctx_s->args = args;
	ctx_s->state = STATE_NOT_START;
	ctx_s->rsp = ctx_s->stack + STACK_SIZE - 8;
	ctx_s->rbp = ctx_s->stack + STACK_SIZE - 8;
}

void switch_to_ctx(struct ctx_s * from, struct ctx_s * to) {
	static struct ctx_s *ctx_from = NULL;
	static struct ctx_s *ctx_to = NULL;
	ctx_from = from;
	ctx_to = to;
/*in case of ctx_from exist and is running, save its SP and set state == running */


	assert (ctx_to != NULL);
	assert(ctx_to->state < STATE_RUNNING);

	if (ctx_from != NULL) {
		assert(ctx_from->state == STATE_RUNNING);
		asm("mov %%rsp, %0" "\n\t" "mov %%rbp, %1"
				:"=r"(ctx_from->rsp),"=r"(ctx_from->rbp)
				 :
				 :);
		ctx_from->state = STATE_PAUSED;
	}

	asm("mov %0, %%rsp" "\n\t" "mov %1, %%rbp"
	:
	:"r"(ctx_to->rsp),"r"(ctx_to->rbp)
	:);


	if (ctx_to->state == STATE_NOT_START) {
		ctx_to->state = STATE_RUNNING;
		ctx_to->entryPointFunct (ctx_to->args);

	} else {
		assert(ctx_to->state == STATE_PAUSED);
		ctx_to->state = STATE_RUNNING;
		return;
	}
}

void f_ping(void *args) {
	while (1) {
		puts("A");
		switch_to_ctx(&ctx_ping, &ctx_pong);
		puts("B");
		switch_to_ctx(&ctx_ping, &ctx_pong);
		puts("C");
		switch_to_ctx(&ctx_ping, &ctx_pong);
	}

}

void f_pong(void *args) {
	while (1) {
		puts("1");
		switch_to_ctx(&ctx_pong, &ctx_ping );
		puts("2");
		switch_to_ctx(&ctx_pong, &ctx_ping);
	}
}

int main(int argc, char *argv[]) {
	printf("Init Ping \n");
	init_ctx(&ctx_ping, f_ping, NULL);
	printf("Init Pong \n");
	init_ctx(&ctx_pong, f_pong, NULL);
	printf("Starting Pong \n");
	switch_to_ctx(NULL, &ctx_pong);
	printf(" This will never print  \n");
	exit(EXIT_SUCCESS);
}

