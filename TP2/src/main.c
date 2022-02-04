#include <stdio.h>
#include <stdlib.h>
#include <switchto.h>

Ctx_s ctx_ping;
Ctx_s ctx_pong;
void f_ping(void *arg);
void f_pong(void *arg);



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

void main(int argc, char *argv[]) {
	printf("Init Ping \n");
	init_ctx(&ctx_ping, f_ping, NULL);
	printf("Init Pong \n");
	init_ctx(&ctx_pong, f_pong, NULL);
	printf("Starting Pong \n");
	switch_to_ctx(NULL, &ctx_pong);
	printf(" This will never print  \n");
	exit(EXIT_SUCCESS);

}




