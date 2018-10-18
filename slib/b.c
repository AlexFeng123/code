int shared = 1;
long long g __attribute__((nocommon));
void swap(int *a, int *b)
{
    *a ^= *b ^= *a ^= *b;
}
