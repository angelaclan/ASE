#define STACK_SIZE 8192
#define STATE_NOT_START 0
#define STATE_PAUSED 1
#define STATE_RUNNING 2
#define STATE_FINNISH 3


/*
 * registry -> [pointer]-> c variable
 * */
#define SAVE_STACK_POINTERS(stack, base) asm ( \
	"mov %%rsp, %[stackPointer] " "\n\t"  \
	"mov %%rbp, %[stackBasePointer] "\
	: [stackPointer] "=r" (stack), [stackBasePointer] "=r" (base)\
	)
/*
 *  c variable ->[pointer]->registry
 * */

#define SET_STACK_POINTERS(stack, base) asm ( \
	"mov  %[stackPointer], %%rsp" "\n\t"  \
	"mov  %[stackBasePointer], %%rbp"\
	: \
	: [stackPointer] "r" (stack), [stackBasePointer] "r" (base)\
	)

typedef  void(*func_t)(void*);

struct ctx_s {
	func_t entryPointFunct;
	void *args;
	int state;
	char stack[STACK_SIZE]; //runtime stack
	void * rbp;
	void * rsp;
};

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void init_ctx(struct ctx_s *ctx, func_t f, void *args);
void switch_to_ctx(struct ctx_s *ctx_from, struct ctx_s *ctx_to);
void f_ping(void *arg);
void f_pong(void *arg);

