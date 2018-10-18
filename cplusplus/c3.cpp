#include<iostream>
#include<tuple>
#include<functional>
#include<utility>
#include"boost/any.hpp"

using namespace std;

template<typename T>
struct Func
{
    auto operator()(T & t)
    {
        return t+1;
    }
};
#if 0
template<typename T>
struct ComposFunc
{
private:
    tuple<> FuncList;
    size_t  count{0};
public:
    inline size_t getCount() const { return count;}
    inline auto getFuncList() const {return FuncList;}
    ComposFunc(ComposFunc<T> &f)
    {
       FuncList = tuple_cat(FuncList,f.getFuncList());
       count += f.getCount();
    }

    template<typename F>
    ComposFunc(F &f)
    {
       FuncList.swap(tuple_cat(FuncList,make_tuple<decltype(f)>(f)));
       ++count;
    }

    template<typename F>
    ComposFunc& operator=(F &f)
    {
       FuncList = tuple_cat(FuncList,make_tuple<decltype(f)>(f));
       ++count;
       return *this;
    }

    ComposFunc& operator=(ComposFunc<T> &f)
    {
       if(&f == this)
           return *this;
       FuncList = tuple_cat(FuncList,f.getFuncList());
       count += f.getCount();
       return *this;
    }
    template<typename tp, size_t...Idx>
    auto callFunc(size_t i, tp & t, T &ret, T &args,  std::index_sequence<Idx...>)
    {
        (((i==Idx) && (ret=get<Idx>(tp)(args))),false)...);
    }
    auto operator()(T &&t)
    {
       auto args = t;
       for(size_t i = 0; i < count; ++i)
       {
           callFunc(i,FuncList,args,args,std::make_index_sequence<tuple_size<decltype(FuncList)>::value>;
       }
       return args;
    }
};
#endif

template<typename T>
struct ComposFunc
{
private:
    vector<boost::any> FuncList;
public:
    inline auto getFuncList() const {return FuncList;}
    ComposFunc(ComposFunc<T> &f)
    {
       FuncList = f.getFuncList();
    }

    template<typename F>
    ComposFunc(F &f)
    {
       FuncList.emplace_back(f);
    }

    template<typename F>
    ComposFunc& operator+=(F &f)
    {
        FuncList.emplace_back(f);
        return *this;
    }

    ComposFunc& operator=(ComposFunc<T> &f)
    {
       if(&f == this)
           return *this;
       for(auto &it: f.getFuncList())
       {
           FuncList.emplace_back(it);
       }
       return *this;
    }
    auto operator()(T &&t)
    {
       auto args = t;
       auto count = FuncList.size();
       cout << "count =" << count <<endl;
       for(size_t i = 0; i < count; ++i)
       {
           auto f = boost::any_cast<Func<int>>(FuncList[i]);
           args = f(args);
       }
       return args;
    }
};

int main()
{
    Func<int> f1;
    Func<int> f4;
    ComposFunc<int> f2(f1);
    ComposFunc<int> f3(f2);
    f3 += f4;
    cout << f3(1) <<endl;
    return 0;
}