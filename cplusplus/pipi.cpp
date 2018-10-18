#include<iostream>
#include<algorithm>
#include<functional>
#include<vector>
#include<memory>
#include<iterator>


using namespace std;
using namespace std::placeholders;
template<typename T>
struct pipe
{
    pipe(const vector<T> &v):mData(v){}
    pipe(vector<T> &&v):mData(forward<vector<T>>(v)){}
    pipe(const pipe &p):mData(p.mData){}
    template<typename U>
    pipe& operator|(U func)
    {
        func(mData.begin(),mData.end());
        return *this;   
    }
    pipe& operator=(const pipe &p)
    {
        mData=p.mData;
        return *this;
    }
    void print()
    {
        copy(mData.begin(),mData.end(),ostream_iterator<int>(cout," "));
    }
    vector<T> mData; 
};

int main()
{
    pipe<int> p1= vector<int>{1,11,1,1,2,3};
    auto sort_f  = [](auto begin,auto end){ sort(begin,end);};
    auto uniq_f  = [](auto begin,auto end){ unique(begin,end);};
    p1 = p1| sort_f |uniq_f;
    p1.print();    

}
