#include<iostream>
#include<tuple>
using namespace std;
// template<typename ...Args>
// void print(Args&&...args)
// {
//     (cout<< ... <<args)<<endl;
// }
template<typename T>
void print1(T t)
{
    cout << t << endl;
}

template<typename first,typename...Args>
void print1(first a, Args...args)
{
    print1(a);
    print1(args...);
}

template<typename ...Args>
void print2(Args...args)
{
    std::initializer_list<int>{(cout<< args << endl,0)...};
}


template<typename ...Args>
void print3(Args ...args)
{
    std::initializer_list<int>{(print1(args),0)...};
}
template<int I = 0,typename Tuple>
typename enable_if<I == tuple_size<Tuple>::value>::type printtp(Tuple tp)
{
}

template<int I = 0,typename Tuple>
typename enable_if< I < tuple_size<Tuple>::value>::type printtp(Tuple tp)
{
    cout << std::get<I>(tp) << endl;
    printtp<I+1>(tp);
}

template<typename ...Args>
void print4(Args... args)
{
    printtp(make_tuple(args...));
}

template<int N>
struct printX
{
    template<typename Tuple>
    static void print(const Tuple &tp)
    {
        printX<N-1>::print(tp);
        cout << get<N-1>(tp).first <<" " << get<N-1>(tp).second <<endl;
    }
};

template<>
struct printX<0>
{
    template<typename Tuple>
    static void print(const Tuple &tp)
    {

    }
};

template<typename ...Args>
void print5(Args &&...args)
{
    auto tp = make_tuple(args...);
    printX<sizeof...(args)>::print(tp);
}

template<int...>
struct int_seq{};

template<int N, int ...Ts>
struct make_seq:make_seq<N-1,N-1,Ts...>{};

template<int ...Ts>
struct make_seq<0,Ts...>
{
    using type = int_seq<Ts...>;
};

template<typename Tuple,int ...Ts>
void print_help(const Tuple & tp, int_seq<Ts...>)
{
   std::initializer_list<int>{(cout << get<Ts>(tp)<<endl,0)...};
}
template<typename ...Args>
void print6(Args...args)
{
    print_help(make_tuple(args...),typename make_seq<sizeof...(args)>::type());
}

#define MAKE_PAIR(T, args) std::make_pair<std::string,decltype(args)>(#T,args)

template<typename T>
 constexpr static inline auto apply( T const & t)
{
    return t;
}

template<typename T, typename T1, typename ...Args>
 constexpr static inline auto apply(T const  &t, const T1& first,const Args&...args) 
{
    return apply(std::tuple_cat(t, std::make_tuple(MAKE_PAIR(T1,first))), args...);
}



template<typename...Args>
auto meta(const Args&...args) 
{ 
     return apply(tuple<>(),args...);
}

template<typename ...Args>
void print7(const Args &...args)
{
    auto tp = meta(args...);
    printX<sizeof...(args)>::print(tp);
}

int main()
{
    print7(1,2,3,"abc");
    return 0;
}


