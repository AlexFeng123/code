extern int shared;
int g __attribute__((nocommon));
int main()
{
    int a = 100;
    swap(&a, &shared);
}

