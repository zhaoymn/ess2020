# ess2020

How to use printf for debugging

in main.c
#include "stdio.h"

add these lines:
// printf() support
int itm_debug(int ch)
{
	return ITM_SendChar(ch);
}
int fputc(int ch, FILE *f) 
{
    return itm_debug(ch);
}

and in project settings
->Debug->Settings(on the right side)->Trace
in this page, change core clock to 168.000 MHz, tick "Trace Enable"

Now you can add printf in your code

in debugging mode, open Debug(printf) Viewer, you can see the message you print with the code here.

If you are printing too fast it may stuck so don't forget to add break points.

