#include<iostream>
using namespace std;

/*change stack*/
int ret_stack(int layer)
{
    unsigned long rbp = 0;
    unsigned long layer_rbp = 0;
    int depth = 0;

    /* 1.得到首层函数的栈基址 */
    __asm__ volatile(
            "movq %%rbp, %0 \n\t"
            :"=r"(rbp)
            :
            :"memory");

    layer_rbp = rbp;
    cout << hex<< rbp <<endl;
    /* 2.逐层回溯栈基址 */
    for(; (depth < layer) && (0 != layer_rbp) && (0 != *(unsigned long *)layer_rbp) && (layer_rbp != *(unsigned long *)layer_rbp); ++depth) {
        cout << hex<< layer_rbp <<endl;
        layer_rbp = *(unsigned long *)layer_rbp;
    }
    cout << hex<< layer_rbp <<endl;
    //change current rbp to target layer rbp
    unsigned long *x = (unsigned long *)rbp;
    *x = layer_rbp;
    cout << hex<< x << " v:" << *x <<endl;
    return depth;
}
int func(int a)
{
    if(a == 20)
    {
        ret_stack(3);
        return 1;
    }
    
    int b = func(a+1);
    cout << "lm in "<< a << " layer" <<endl;
    return b;
}
int main()
{
    int x = 1;
    func(x);

    return 0;
}
