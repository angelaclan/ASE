#include "gdt.h"
#include "idt.h"
#include "minilib.h"
#include "list.h"
#include "yield.h"

extern void app_main(void);
void startYield ();


void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);
static Ctx_s *ctx_ping;
static Ctx_s *ctx_pong;
static Ctx_s *ctx_pang;



void empty_irq(int_regs_t *r) {
}

/* multiboot entry-point with datastructure as arg. */
void main(unsigned int * mboot_info)
{
    /* clear the screen */
    clear_screen();
    puts("Early boot.\n"); 

    /* Initialize the memory */
    puts("\t-> Setting up the GDT... ");
    gdt_init_default();
    puts("OK\n");

    /* Initializa the Interrupt Descriptor Table */
    puts("\t-> Setting up the IDT... ");
    setup_idt();
    puts("OK\n");

    puts("\n\n");

    /* Installs two empty handlers for the timer (0) and the keyboard (1) */
    idt_setup_handler(0, empty_irq);
    idt_setup_handler(1, empty_irq);

    /* Enables interrupts */ 
    __asm volatile("sti");

    /* minimal setup done ! */
    app_main();
    
    puts("Starting\n");
    startYield();

}




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
void startYield() {


	/*init ping and pong context*/
	initYieldContext(&context);
	ctx_ping = create_ctx(&context, f_ping, NULL);
	ctx_pong = create_ctx(&context, f_pong, NULL);
	ctx_pang = create_ctx(&context, f_pang, NULL);


    yield();

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
void f_pang(void *arg) {
	while (1) {
		puts("m");
		yield();
		puts("n");
		yield();
	}

}
