#include<tuple>
//-std=C++11
#if __cplusplus >= 201103L && __cplusplus <201300L
//gen int seq
template<int...>
struct seq{};

template<int n, int...Is>
struct make_seq: make_seq<n-1,n-1,Is...> {};

template<int...Is>
struct make_seq<0,Is...>:seq<Is...>{};

template<typename ...Ts>
using get_seq = make_seq<sizeof...(Ts)>;

template<typename T,typename F, int...Is>
void for_each(T&& tp, F f, seq<Is...>)
{
    auto l = {(f(std::get<Is>(tp)),0)...};
}

template<typename ...Ts,typename F>
void for_each_tuple(const tuple<Ts...> &tp, F f)
{
    for_each(tp,f,get_seq<Ts...>());
}

#elif __cplusplus >= 201300L
#include<utility>
template<typename T,typename F, size_t...Is>
void for_each(T&& tp, F f, std::index_sequence<Is...>)
{
    auto l = {(f(std::get<Is>(tp)),0)...};
}

template<typename ...Ts,typename F>
void for_each_tuple(const std::tuple<Ts...> &tp, F f)
{
    for_each(tp,f,std::index_sequence_for<Ts...>());
}
#endif











