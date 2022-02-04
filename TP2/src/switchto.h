#define STACK_SIZE 8192
#define STATE_NOT_START 0
#define STATE_PAUSED 1
#define STATE_RUNNING 2
#define STATE_FINNISH 3

typedef void(*func_t)(void*);

typedef struct Ctx_s {
	func_t entryPointFunct;
	void *args;
	int state;
	char stack[STACK_SIZE]; //runtime stack
	void * rbp;
	void * rsp;
}Ctx_s;

void init_ctx(Ctx_s *ctx, func_t f, void *args);
void switch_to_ctx(Ctx_s *ctx_from, Ctx_s *ctx_to);
void start_state(Ctx_s *ctx);
