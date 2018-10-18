#define barrier() __asm__ __volatile__("": : :"memory")

int main(int argc, char *argv[])
{
    int a = 0, b , c, d[4096], e;

    e = d[4095];
    /*barrier();*/
    b = a;
    c = a;
    /

    printf("a:%d b:%d c:%d e:%d\n", a, b, c,e);
    O
  2  
    return 0;
}
