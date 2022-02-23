#include "gdt.h"
#include "idt.h"
#include "minilib.h"
#include "list.h"
#include "yield.h"

#define COUNTER 15
extern void app_main(void);

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);
static Ctx_s *ctx_ping;
static Ctx_s *ctx_pong;
static Ctx_s *ctx_pang;
static int counter = 0;
static char mustYield = 0;/*flag set by timer for forcing yield*/
void yield ();
static YieldCtx context;


void yield () {
	mustYield = 0;
	__yield(&context);
}

/* function as a system call, where sets the flag and decides yield or not
 * so process need to revoke system call otherwise it will never yield*/
void preemtablePuts (char* content ) {

	if (mustYield) yield();
	puts(content);
}


void empty_irq(int_regs_t *r) {
}

/*critical sections*/
void timer_irq_handelr () {
	irq_disable();
	counter ++;
	if (counter > 10) {
		counter = 0;
		mustYield = 1;
	}
	irq_enable();
}

void start_sched(){
	yield();
	timer_irq_handelr();
}

/* multiboot entry-point with datastructure as arg. */
void main(unsigned int * mboot_info)
{
    /* clear the screen */
    clear_screen();
    puts("Early boot.\n");

    /* Initialize the memory */
    preemtablePuts("\t-> Setting up the GDT... ");
    gdt_init_default();
    puts("OK\n");

    /* Initializa the Interrupt Descriptor Table */
    puts("\t-> Setting up the IDT... ");
    setup_idt();
    puts("OK\n");

    puts("\n\n");

    
    /* Installs two empty handlers for the timer (0) and the keyboard (1) */
    idt_setup_handler(0, timer_irq_handelr);
    idt_setup_handler(1, empty_irq);

    /* Enables interrupts */ 
    __asm volatile("sti");

    /* minimal setup done ! */
    app_main();

    puts("Starting\n");

    /*init ping and pong context*/
    initYieldContext(&context);
    ctx_ping = create_ctx(&context, f_ping, NULL);
    ctx_pong = create_ctx(&context, f_pong, NULL);
    ctx_pang = create_ctx(&context, f_pang, NULL);

    irq_enable();
    //yield();
    start_sched();


}



void f_ping(void *args) {

	while (1) {
		preemtablePuts("A");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
	
		preemtablePuts("B");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
	
		preemtablePuts("C");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
	
	}

}

void f_pong(void *args) {
	while (1) {
		preemtablePuts("1");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
		
		preemtablePuts("2");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
		
	}

}
void f_pang(void *arg) {
	while (1) {
		preemtablePuts("m");
		for (int i = 0; i < (1 << COUNTER); i++)
            ;
	
		preemtablePuts("n");
		for (int i = 0; i < (1 << COUNTER); i++)
			;


	}

}
