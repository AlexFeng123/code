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

#endif
