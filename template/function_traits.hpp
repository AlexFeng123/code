#include<iostream>
#include<functional>
#include <cxxabi.h>
using namespace std;

//function traits
template<typename T>
struct func_traits;

//normal function
template<typename Ret, typename ...Args>
struct func_traits<Ret(Args...)>
{
    constexpr static int  num_args = sizeof...(Args);
    using ret_type = Ret;
    using stl_func_type = std::function<Ret(Args...)>;
    using func_pointer = Ret(*)(Args...);
    using func_type = Ret(Args...);
    template<int I>
    struct args
    {
        static_assert(I < num_args, "index is out of range, index must less than sizeof Args");
        using type = typename std::tuple_element<I,std::tuple<Args...>>::type;
    };
};

//function pointer
template<typename Ret, typename ...Args>
struct func_traits<Ret(*)(Args...)> :  func_traits<Ret(Args...)>{};

//std::function
template<typename Ret, typename ...Args>
struct func_traits<std::function<Ret(Args...)>> : func_traits<Ret(Args...)>{};

//member function
#define FUNCTION_TRAITS(...) \
        template<typename Ret, class ClassType, typename ...Args> \
        struct func_traits<Ret(ClassType::*)(Args...) __VA_ARGS__>: func_traits<Ret(Args...)>{};

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

//function obj
template<typename Callable>
struct func_traits: func_traits<decltype(&Callable::operator())>{};

template<typename T>
void printType()
{
    int status;
    cout << abi::__cxa_demangle(typeid(T).name(),0,0,&status) <<endl;  
}

struct Funtor
{
    int operator()(int a)
    {
        return a;
    }
};

struct Class
{
    int mem_func(int a, int b) const 
    {
        return a+b;
    }
#if 0
    int mem_func(int a, int b) const 
    {
        return a+b;
    }

    int mem_func(int a, int b) volatile 
    {
        return a+b;
    }

    int mem_func(int a, int b) const volatile 
    {
        return a+b;
    }
#endif
};

template<typename T, typename OpFunc=std::less<T>>
bool cmp(const T &lval, const T& rval, OpFunc f)
{
    return f(lval,rval);
}

#if 0
template<typename FuncType>
void test()
{
    cout << func_traits<FuncType>::num_args <<endl;
    printType<typename func_traits<FuncType>::func_type>();
    printType<typename func_traits<FuncType>::func_pointer>();
    printType<typename func_traits<FuncType>::stl_func_type>();
    printType<typename func_traits<FuncType>::ret_type>();
    printType<typename func_traits<FuncType>::template args<0>::type>();
}

int main()
{
    auto f = [](int a, long b){return a+b;};
    Funtor  funObj;
    Class  obj;
    test<decltype(f)>();
    test<decltype(funObj)>();
    test<decltype(cmp<int>)>();
    test<decltype(&Class::mem_func)>();
    
    return 0;
}
#endif
