#include<iostream>
#include<functional>
#include<vector>
#include<utility>
#include<algorithm>
#include<type_traits>
using namespace std;
template<typename F>
struct pipeable
{
private:
   F m_f;
public:
    pipeable(F&& f):m_f(forward<F>(f)){}
    template<typename...Ts>
    auto operator()(Ts&&...ts)->decltype(bind(m_f,std::placeholders::_1, forward<Ts>(ts)...))
    {
        return bind(m_f,std::placeholders::_1,forward<Ts>(ts)...);
    }
};
template<class F>
pipeable<F> piped(F&& f)
{
    return pipeable<F>{forward<F>(f)};
}

template<class T, class F, bool is_int>
struct pipecall{};

template<class T, class F>
struct pipecall<T,F,true>
{
    static decltype(auto) call(T&&t, const F& f)
    {
        return f(std::forward<T>(t));
    }
};

template<class T, class F>
struct pipecall<T,F,false>
{
    static decltype(auto) call(T&t, const F& f)
    {
        f(t.begin(),t.end());
        return t;
    }
};
template<typename T, typename F>
decltype(auto) operator|(T&& t,  const F& f)
{
   return pipecall<T,F,is_same<remove_reference_t<T>,int>::value>::call(forward<T>(t),f);
}


int main()
{
    vector<int> a{1,2,3,4,2,3,4};
    int num = 1;
    auto add = piped([](int a, int b){return a+b;});
    auto mul = piped([](int a, int b){return a*b;});
    auto mysort = [](auto begin, auto end){sort(begin,end);};
    if(is_integral<decltype(a)>::value) cout<<"true"<<endl;
    int result = num | add(2) | mul(3);
    cout <<"result: "<<result<<endl;
    a = a|mysort;
    for_each(a.begin(),a.end(),[](auto &e){cout <<" "<<e;});
    cout<<endl;
}
