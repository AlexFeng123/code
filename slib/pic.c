static int a;
extern int b;
extern void ext();

void bar()
{
    a = 1;
    b = 2;
}

int foo()
{
    bar();
    ext();
}
