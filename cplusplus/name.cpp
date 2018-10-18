#include<iostream>
#include<tuple>
#include<typeinfo>
#include"index.hpp"
#include <cxxabi.h>
using namespace std;
namespace x
{
    int var = 42;
}
static void foo() __attribute__ ((weakref, alias("foo"))); 
template<typename T>
string cat()
{
    int status;
    return  abi::__cxa_demangle(typeid(T).name(),0,0,&status) ; 
}
template<typename ... Args>
string func(Args...args)
{
    string str;
    std::initializer_list<int>{(str+=" "+cat<Args>(),0)...}; 
    return str;
}

template<typename T,typename = typename T::xxx>
static std::true_type has_xxx_impl(int);

template<typename T>
static std::false_type has_xxx_impl(...);

template<typename T>
struct has_xxx: decltype(has_xxx_impl<T>(0))
{
    
};

template<typename... Ts> struct make_void { typedef void type; };
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

template<typename T,typename = void>
struct has_yyy: std::false_type{};

template<typename T>
struct has_yyy<T,void_t<T>>: std::true_type{};


struct X
{
    using xxx = int;
};

int main()
{
    static_assert(has_xxx<X>::value,"no xxx");
    static_assert(has_yyy<X>::value,"no xxx");
    return 0;
}
