#include <features.h>
#include <stdio.h>
#include <graphics.h>
#include <input.h>
#include <sound.h>
#include <games.h>
#include <time.h>

int main()
{
   char buf[10];
   int  c;

   printf("x\n");
#ifdef __HAVE_KEYBOARD
   c = fgetc_cons();
//   c = getk();
#endif
#ifdef __HAVE_INKEY
   c = in_LookupKey('2');
   c = in_KeyPressed(2);
   c = in_Inkey();
#endif
#ifdef __HAVE_GFX
   plot(0,0);
   unplot(0,0);
   //xorplot(0,0);
   //point(0,0);
#endif
#ifdef __HAVE_ONEBITSOUND
   bit_click(1);
#endif
#ifdef __HAVE_TIME
   c = time(NULL);
#endif
#ifdef __HAVE_FILEIO
   c = read(0, buf, 0);
#endif
}
