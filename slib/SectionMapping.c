#include<stdlib.h>
#include<stdio.h>

unsigned maxinum = 0;

int main()
{
     unsigned blocksize[] = { 1024*1204, 1024, 1};
     int i ,count ;
     for(i = 0; i < 3; i++)  
     {
         for(count = 1 ;; count++)
         {
             void *block = malloc(maxinum + blocksize[i] * count);
             if(block)
             {
                 maxinum += blocksize[i] * count; 
                 free(block);
             }
             else
             {
                 break;
             }
         }
     }    
     printf("maxinum malloc size = %u bytes \n", maxinum);
    return 0;
}

