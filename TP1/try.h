
#ifndef CTX_INIT
#define CTX_INIT
#endif


typedef int (func_t)(int); /* A function that returns an int from an int */

struct ctx_s {
	void *ebp;
	void *esp;
	unsigned init; 
};

int try(struct ctx_s *pctx, func_t *f, int arg);
int throw(struct ctx_s *pctx, int r);

