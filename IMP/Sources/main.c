/* David Mikus, xmikus15
original
17.12.2015
*/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define size 64
#define init 1
#define horiz 2
#define vertical 4

const unsigned char login[size] = {
  0,66,36,24,24,36,66,0,    // X
  0,126,12,24,24,12,126,0,  // M
 	0,0,0,126,126,0,0,0,      // I
 	0,126,126,24,52,102,66,0, // K
 	0,30,62,96,96,62,30,0,    // U
 	0,0,76,82,82,34,0,0,      // S
 	0,0,8,4,126,0,0,0,        // 1
 	0,0,94,82,82,114,0,0      // 5
};
unsigned char *buttons = (unsigned char*)0xB0;
unsigned char *display = (unsigned char*)0xB1;
unsigned char *slider = (unsigned char*)0xD1;
unsigned char rotate = 0;

void checkButtons() {
  if(*buttons&init)
    rotate = 0;
  else if(*buttons&horiz)
    rotate = horiz;
  else if (*buttons&vertical)
    rotate = vertical;
}

void delay() {
 unsigned int i;
 unsigned int speed = 4300 - (*slider * 16);
 for(i=0;i<speed;i++){
  checkButtons();
 }
}



void redraw(int x, int y){
  int i;
  int index;
  unsigned char ch;
  y = y%8;
   for(i=0; i<32;i++) {
      // Horizontalny posun
      index = (i-x)%size;
      if(index<0)
        index += size;
      ch = login[index];
      // Rotatovanie bitov pre vertikalny posun
      ch = (ch >> y) | (ch << (8-y));
      display[i] = ch;
    }
  
}
void main(void) {
  int x = 0;
  int y = 0;
  *buttons = 0;
  *slider = 128;
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  for(;;) {
    __RESET_WATCHDOG(); /* feeds the dog */
    checkButtons();
    if(rotate == 0) {
     x = 0;
     y = 0;
    } else if(rotate == horiz)
      ++x;
    else if(rotate == vertical)
      ++y;
    redraw(x,y);
    delay();
  } /* loop forever */
  /* please make sure that you never leave main */
}
