#include<iostream>
#include<typeinfo>
#include<tuple>
#include<functional>
using namespace std;
void tprintf(const char* format)
{
    cout << format;
}

template<typename T, typename... Targs>
void tprintf(const char * format, T value, Targs... Fargs)
{
    for(; *format != '\0'; ++format)
    {
        if(*format == '%')
        {
            cout << value<<" "<<sizeof...(Fargs);
            tprintf(format+1,Fargs...);
            return;
        }
        cout << *format;
    }
}

template<typename T>
T sum(T t)
{
    return t;
}
template<typename T,typename ... Types>
T sum(T first, Types...rest)
{
    return first+sum(rest...);
}

template<class F, typename... Types>
void expand(const F&f, Types&...args)
{
    initializer_list<int>{(f(forward<Types>(args)),0)...};
}


template<typename first, typename... rest>
struct Sum{ enum{ value = Sum<first>::value + Sum<rest...>::value};};

template<typename last>
struct Sum<last> { enum { value = sizeof(last)};};

template<typename... rest>
struct SumCon;

template<typename first, typename... rest>
struct SumCon<first,rest...>:integral_constant<int,  SumCon<first>::value + SumCon<rest...>::value>{};


template<typename last>
struct SumCon<last>:integral_constant<int, sizeof(last)>{};

template<>
struct SumCon<>:integral_constant<int, 0>{};

//int sequence 
template<int...>
struct intSeq{};

template<int first , int...last>
struct makeIntSeq
{
  using type = intSeq<first,makeIntSeq<last...>::value>;
  enum{value = makeIntSeq<last...>::value};  
};

template<int last>
struct makeIntSeq<last>
{
  enum{value = last};
};

template<int N, int ...index>
struct makeIntSeqX
{
    using type = typename makeIntSeqX<N-1,N-1,index...>::type;
};

template<int...index>
struct makeIntSeqX<0,index...>
{
    using type = intSeq<index...>;
};

template<typename T, typename...Args>
T* instance(Args&&...args)
{
    return new T(forward<Args>(args)...);
}
struct A
{
    A(int){}
};

struct B
{
    B(int,double){}
};

#include <cxxabi.h>
template<typename R, typename...Args>
struct Task 
{
    using TaskType = std::function<R(Args...)>;
    Task(TaskType fun, Args...args):
        mFun(fun),mTuple(make_tuple(args...)){}
    template<int...index>
    R funcImpl(TaskType &func,tuple<Args...> &pTuple, intSeq<index...>)
    {
        return func(get<index>(pTuple)...);
    } 
    void execute()
    {
        using index_seq = typename makeIntSeqX<sizeof...(Args)>::type;
        int status;
        cout<<abi::__cxa_demangle(typeid(index_seq).name(),0,0,&status)<<endl;
        mRet = funcImpl(mFun, mTuple, index_seq());
    }
    R  mRet;
    TaskType mFun;
    tuple<Args...> mTuple;
};

int  sumFunc(int a, int b,int c,int d)
{
    return a+b+c+d;
}

#include <cxxabi.h>
int main()
{
    
    Task<int,int,int,int,int> lTask(sumFunc, 1,1,1,1);
    lTask.execute();
    cout << lTask.mRet<<endl;
    int status;
    cout<< abi::__cxa_demangle(typeid(makeIntSeqX<3>::type).name(),0,0,&status)<<endl;
    return 0;
}
