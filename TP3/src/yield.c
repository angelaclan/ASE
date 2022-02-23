
#include "yield.h"
#include "idt.h"

/*represent head of linked list and the current context*/
void initYieldContext(YieldCtx *ctx) {
	INIT_LIST_HEAD(&(ctx->contexts));
	ctx->lastAssignedContext = 0;
	ctx->current = NULL;
}

void init_ctx(Ctx_s *ctx_s, func_t f, void *args) {
	ctx_s->entryPointFunct = f;
	ctx_s->args = args;
	ctx_s->state = STATE_NOT_START;
	ctx_s->rsp = ctx_s->stack + STACK_SIZE - 8;
	ctx_s->rbp = ctx_s->stack + STACK_SIZE - 8;
}

void switch_to_ctx(Ctx_s *from, Ctx_s *to) {
	static Ctx_s *ctx_from = NULL;
	static Ctx_s *ctx_to = NULL;

	ctx_from = from;
	ctx_to = to;
	assert(ctx_to != NULL);
	assert(ctx_to->state < STATE_RUNNING);

	/*in case of ctx_from exist and is running, save its SP and set state == running */
	if (ctx_from != NULL) {
		assert(ctx_from->state == STATE_RUNNING);
		asm("mov %%esp, %0" "\n\t" "mov %%ebp, %1"
				:"=r"(ctx_from->rsp),"=r"(ctx_from->rbp)
				:
				:);
		ctx_from->state = STATE_PAUSED;
	}


	/*in the case of ctx_from is null, set directly the SP of the ctx_to context */
	asm("mov %0, %%esp" "\n\t" "mov %1, %%ebp"
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

/* The primitive create_ctx()creates a new context for the function f,
 * and inserts it into the list of “active” contexts.
 */
Ctx_s* create_ctx(YieldCtx *ctxx, func_t f, void *args) {
	Ctx_s *newCtx;

	newCtx = &(ctxx->stash[ctxx->lastAssignedContext]);
	ctxx->lastAssignedContext++;
	assert(ctxx->lastAssignedContext < STASH_SIZE);
	init_ctx(newCtx, f, args);
	INIT_LIST_HEAD(&(newCtx->myContext));
	/*add new context to the head of linked list (contexts)*/
	list_add(&(newCtx->myContext), &(ctxx->contexts));
	return newCtx;
}

/* passing yield context as argument for manipulating next and previous context easier*/

void __yield(YieldCtx *ctxt) {

	Ctx_s *previous = ctxt->current;
	Ctx_s *next;

	irq_disable();
	/* in the scenario where list is not NULL :
	 * then the next entry will be the current node -> next
	 * */
	if (ctxt->current != NULL) {
		next = list_next_entry(ctxt->current, myContext);
		/* if the next element is the handle/head of the linked list,
		 * then current will be set to NULL for entering the condition (ctxt->current == NULL)
		 * if the next element is not the head of the linked list,
		 * then the next element will become current element
		 */
		if (list_entry_is_head(next, &(ctxt->contexts), myContext)) {
			ctxt->current = NULL;

		} else {
			ctxt->current = next;
		}
	}

	/* in the scenario where list is NULL two scenarios let entering the handle of the linked list :
	 * one, when process just start to switching context,
	 * two, when arriving to the last entry of the list
	 * */
	if (ctxt->current == NULL) {
		/*get context 1*/
		ctxt->current = list_first_entry(&(ctxt->contexts), Ctx_s, myContext);
	}
	irq_enable();
	switch_to_ctx(previous, ctxt->current);

}
