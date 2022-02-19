#ifndef __MINILIB_H__
#define __MINILIB_H__

#ifndef NULL
#define NULL ((void*)0)
#endif

extern void __assert__(int x, char *f, unsigned line);

#define assert(x) __assert__(x, __FILE__, __LINE__) 

void clear_screen();				/* clear screen */
void putc(char aChar);				/* print a single char on screen */
void puts(char *aString);			/* print a string on the screen */
void puthex(unsigned aNumber);		/* print an Hex number on screen */
void putud(unsigned aNumber);       /* print a unsigned decimal on screen */

#endif
