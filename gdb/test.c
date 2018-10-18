#include<stdio.h>

long long  add(long long a , long long b)
{
   long long result = 0;
   result = a + b;
   return result;
}

int  main(int  argc, char *argv[])
{
   long long a = 0x11111111111f1, b = 2;
   long long c = 0;
   c = add(a , b);
   printf("result = %lld\n", c);
   return 0; 
}
