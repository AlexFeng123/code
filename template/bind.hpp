#include<iostream>
#include<tuple>
#include"function_traits.hpp"
#include"tuple_for.hpp"
using namespace std;

template<int T>
struct placehold{};
placehold<1> _1;
placehold<2> _2;
placehold<3> _3;
placehold<4> _4;

template<typename T,typename Tuple>
inline auto select(T&& val, Tuple&)->T&&
{
    return forward<T>(val);
}

template<int I,typename Tuple>
inline auto select(placehold<I>&, Tuple&tp)->decltype(get<I-1>(tp))
{
    return get<I-1>(tp);
}

template<typename F>
struct is_mf_noref:is_member_function_pointer<typename remove_reference<F>::type>{};

template<typename F>
struct is_ptr_noref:is_pointer<typename remove_reference<F>::type>{};

template<typename R,typename F, typename...Args>
inline typename  enable_if<!is_mf_noref<F>::value &&!is_ptr_noref<F>::value,R>::type apply(F &&f, Args&&...args)
{
    return forward<F>(f)(forward<Args>(args)...);
} 

template<typename R,typename F, typename...Args>
inline typename  enable_if<!is_mf_noref<F>::value && is_ptr_noref<F>::value,R>::type apply(F &&f, Args&&...args)
{
    return (*forward<F>(f))(forward<Args>(args)...);
} 
template<typename R,typename F, typename C, typename...Args>
inline typename enable_if<is_mf_noref<F>::value && !is_ptr_noref<C>::value,R>::type apply(F &&f, C &&this_obj, Args&&...args)
{
    return (forward<C>(this_obj).*forward<F>(f))(forward<Args>(args)...);
} 

template<typename R,typename F, typename C, typename...Args>
inline typename enable_if<is_mf_noref<F>::value && is_ptr_noref<C>::value,R>::type apply(F &&f, C &&this_obj, Args&&...args)
{
    return (forward<C>(this_obj)->*forward<F>(f))(forward<Args>(args)...);
} 
template<typename F,typename ...Args>
struct Bind_t
{
    using args_type = tuple<typename decay<Args>::type...>;
    using func_type = typename decay<F>::type;
    using ret_type =  typename func_traits<func_type>::ret_type;
   
    template<typename Fun, typename ...P> 
    Bind_t(Fun &f, P&...args):m_f(f),m_args(args...){}

    template<typename Fun, typename ...P> 
    Bind_t(Fun &&f, P&&...args):m_f(std::move(f)),m_args(std::move(args)...){}
 
    template<typename...Para>
    ret_type operator()(Para&&...args)
    {
        return forward_call(forward_as_tuple(forward<Para>(args)...),get_seq<Args...>());          
    }
    

    template<typename Tp, int ... Is>
    ret_type forward_call(const Tp &args, seq<Is...>)    
    {
       return  apply<ret_type>(m_f, select(get<Is>(m_args),args)...);
    }
    private:
    func_type m_f;
    args_type m_args;
};

template<typename F, typename ...Args>
inline Bind_t<F,Args...> Bind(F&& f, Args&&...args)
{
    return Bind_t<F,Args...>(forward<F>(f),forward<Args>(args)...);
}

template<typename F, typename ...Args>
inline Bind_t<F,Args...> Bind(F& f, Args&...args)
{
    return Bind_t<F,Args...>(f,args...);
}

int sum(int a, int b)
{
    return a+b;
}

struct Func
{
    int operator()(int a,int b)
    {
        return a+b+1;
    }

    int sum(int a, int b)
    {
        return a+b;
    }
};

int main()
{
    auto x = [](int a, int b){ return a+b;};
    Func fp;
    using fun_ptr = int(*)(int,int);
    fun_ptr fPtr = sum;
    auto f = Bind(fPtr,_1,_2);
    cout << f(2,3) <<endl;
    return 0; 
}
