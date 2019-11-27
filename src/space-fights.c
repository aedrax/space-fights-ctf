/*
Space Fights CTF problem
Scrolling on the screen does not scroll far enough
The RE must increase the distance of the scroll
in order to see the flag, looking at the nametables
will not be enough to see the flag in plain text.
*/

#include "neslib.h"
#include <string.h>
#define SECRET 100
// thanks this guy:
// https://yurisk.info/2017/06/25/binary-obfuscation-string-obfuscating-in-C/index.html
#define HIDE_LETTER(a) (a) + SECRET
#define UNHIDE_STRING(str) \
  do                       \
  {                        \
    char *ptr = str;       \
    while (*ptr)           \
      *ptr++ -= SECRET;    \
  } while (0)

// link the pattern table into CHR ROM
//#link "chr_generic.s"

char flag[] = {
    HIDE_LETTER('f'), HIDE_LETTER('l'), HIDE_LETTER('a'), HIDE_LETTER('g'), HIDE_LETTER('{'),
    HIDE_LETTER('g'), HIDE_LETTER('r'), HIDE_LETTER('e'), HIDE_LETTER('e'), HIDE_LETTER('d'),
    HIDE_LETTER('o'), HIDE_LETTER('_'), HIDE_LETTER('s'), HIDE_LETTER('h'), HIDE_LETTER('o'),
    HIDE_LETTER('t'), HIDE_LETTER('_'), HIDE_LETTER('f'), HIDE_LETTER('i'), HIDE_LETTER('r'),
    HIDE_LETTER('s'), HIDE_LETTER('t'), HIDE_LETTER('!'), HIDE_LETTER('}'), '\0'};
int sneaky_variable = SECRET;

// function to write a string into the name table
//   adr = start address in name table
//   str = pointer to string
void put_str(unsigned int adr, const char *str)
{
  vram_adr(adr);                // set PPU read/write address
  vram_write(str, strlen(str)); // write bytes to PPU
}

// function to scroll window up and down until end
void scroll_demo()
{

  int x = 0;  // x scroll position
  int y = 0;  // y scroll position
  int dy = 1; // y scroll direction
  // infinite loop
  while (1)
  {
    // wait for next frame
    ppu_wait_frame();
    // update y variable
    y += dy;
    // stop scrolling after hitting edge limit
    if (y >= sneaky_variable)
      dy = 0;
    // set scroll register
    scroll(x, y);
  }
}

// main function, run after console reset
void main(void)
{
  // set palette colors
  pal_col(0, 0x0D); // black background
  pal_col(1, 0x00); // white boarder
  pal_col(2, 0x00); // white nothing
  pal_col(3, 0x39); // yellow text

  // write text to name table
  put_str(NTADR_A(2, 15), "A long time ago,");
  put_str(NTADR_A(2, 16), "In a NES far, far away...");
  put_str(NTADR_A(2, 24), "It is a period of civil ");
  put_str(NTADR_A(2, 25), "war. A seige of 100 Rebel");
  put_str(NTADR_A(2, 26), "spaceships, striking from a");
  put_str(NTADR_A(2, 27), "hidden base, have won their");
  put_str(NTADR_A(2, 28), "first victory against the");
  put_str(NTADR_A(2, 29), "evil Galactic Empire. But,");
  put_str(NTADR_C(2, 0), "that was only half of the");
  put_str(NTADR_C(2, 1), "original Rebel fighters...");
  put_str(NTADR_C(2, 6), "It would be really awkward");
  put_str(NTADR_C(2, 7), "if the movie didn't scroll");
  put_str(NTADR_C(2, 8), "far enough for you to read");
  put_str(NTADR_C(2, 9), "the entire story...");
  if (sneaky_variable > SECRET)
  {
    UNHIDE_STRING(flag);
  }
  
  // print the flag on the nametable
  put_str(NTADR_C(2, 19), flag);

  // enable PPU rendering (turn on screen)
  ppu_on_all();

  // scroll window back and forth
  scroll_demo();
}
