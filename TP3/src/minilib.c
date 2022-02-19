#include "minilib.h"
#include "ioport.h"

void __assert__(int x, char *f, unsigned line)
{
    if (!x) {
        puts("Assert violated: ");
        puts("File ");
        puts(f);
        puts(" at line ");
        putud(line);
        for(;;);
    }
}

/* base address for the video output assume to be set as character oriented by the multiboot */
static unsigned char *video_memory = (unsigned char *) 0xB8000;

/* clear screen */
void clear_screen() {
  int i;
  for(i=0;i<80*25;i++) { 			/* for each one of the 80 char by 25 lines */
    video_memory[i*2+1]=0x0F;			/* color is set to black background and white char */
    video_memory[i*2]=(unsigned char)' '; 	/* character shown is the space char */
  }
}

/* print a string on the screen */
void puts(char *aString) {
  char *current_char=aString;
  while(*current_char!=0) {
    putc(*current_char++);
  }
}

/* print an number in hexa */
char *hex_digit="0123456789ABCDEF";
void puthex(unsigned aNumber) {
  int i;
  int started=0;
  for(i=28;i>=0;i-=4) {
    int k=(aNumber>>i)&0xF;
    if(k!=0 || started) {
      putc(hex_digit[k]);
      started=1;
    }
  }
  if(!started) putc('0');
}

void putud(unsigned aNumber)
{
    char s[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int i = 0;
    while (aNumber > 0) {
        s[i] = '0' + (aNumber % 10);
        i++;
        aNumber /= 10;
    }
    while (i > 0) 
        putc(s[--i]);
    
}

/* print a char on the screen */
static int cursor_x=0;					/* here is the cursor position on X [0..79] */
static int cursor_y=0;					/* here is the cursor position on Y [0..24] */

void setCursor()
{
  int cursor_offset = cursor_x+cursor_y*80;
  _outb(0x3d4,14);
  _outb(0x3d5,((cursor_offset>>8)&0xFF));
  _outb(0x3d4,15);
  _outb(0x3d5,(cursor_offset&0xFF));
}

void putc(char c)
{
  if(cursor_x>79) {
    cursor_x=0;
    cursor_y++;
  }
  if(cursor_y>24) {
    cursor_y=0;
    clear_screen();
  }
  switch(c) {					/* deal with a special char */
    case '\r': cursor_x=0; break;		/* carriage return */
    case '\n': cursor_x=0; cursor_y++; break; 	/* new ligne */	
    case 0x8 : if(cursor_x>0) cursor_x--; break;/* backspace */
    case 0x9 : cursor_x=(cursor_x+8)&~7; break;	/* tabulation */
						/* or print a simple character */
    default  : 
      video_memory[(cursor_x+80*cursor_y)*2]=c;
      cursor_x++;
      break;
  }
  setCursor();
}

