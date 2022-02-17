#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "yield.h"



void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);
static Ctx_s *ctx_ping;
static Ctx_s *ctx_pong;
static Ctx_s *ctx_pang;


static YieldCtx context;

void yield () {
	__yield(&context);
}
void main(int argc, char *argv[]) {


	/*init ping and pong context*/
	initYieldContext(&context);
	ctx_ping = create_ctx(&context, f_ping, NULL);
	ctx_pong = create_ctx(&context, f_pong, NULL);
	ctx_pang = create_ctx(&context, f_pang, NULL);


    yield();

}

void f_ping(void *args) {
	printf("ping running\n");
	while (1) {
		puts("A");
		yield();
		puts("B");
		yield();
		puts("C");
		yield();
	}

}

void f_pong(void *args) {
	printf("pong running\n");
	while (1) {
		puts("1");
		yield();
		puts("2");
		yield();
	}

}
void f_pang(void *arg) {
	printf("pang running\n");

	while (1) {
		puts("m");
		yield();
		puts("n");
		yield();
	}

}






