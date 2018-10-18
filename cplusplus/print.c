#include<stdio.h>
#include"stdarg.h"
void alex_print(const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);
	char fmtmsg[256];
	vsnprintf(fmtmsg,sizeof(fmtmsg),msg,ap);
	va_end(ap);
        printf("%s\n", fmtmsg);
}

int  main()
{	
   int a = 0;
   char b = 'a';
   char *c = "1234";
   alex_print("%d %c %s\n",a, b, c);
   return 0;
}

