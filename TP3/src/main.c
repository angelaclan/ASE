#include <stdio.h>
#include <stdlib.h>

#include "yield.h"
#include "list.h"


void f_ping(void *arg);
void f_pong(void *arg);

void main(int argc, char *argv[]) {
	/*declare a list*/
	LIST_HEAD(mylisthead);
	/*init ping and pong context*/
	Ctx_s* ctx_ping = create_ctx(mylisthead, f_ping, NULL);
	Ctx_s* ctx_pong = create_ctx(mylisthead, f_pong, NULL);;

    struct Ctx_s  *p = NULL ;
    list_for_each_entry (p, &mylisthead, myContext)
    {
        printf("rsp = %d rbp = %d\n" , p->rsp, p->rbp);
    }


}

void f_ping(void *args) {
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
	while (1) {
		puts("1");
		yield();
		puts("2");
		yield();
	}
}





