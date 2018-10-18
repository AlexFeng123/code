#include<iostream>
#include<list>
#include<chrono>
#include<random>
#include<functional>
#include<algorithm>
#include<future>
using namespace std;
template<typename T>
std::list<T>  sequential_quick_sort(std::list<T> input)
{
   if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot = *result.begin();
    auto divide_point= std::partition(input.begin(), input.end(),[&](T const& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),divide_point);
    
    auto new_lower(sequential_quick_sort(std::move(lower_part)));
    auto new_higher(sequential_quick_sort(std::move(input)));


    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower);

    return result;
}
template<typename T>
std::list<T>  parallel_quick_sort(std::list<T> input)
{
   if(input.empty())
    {
        return input;

    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot = *result.begin();
    auto divide_point= std::partition(input.begin(), input.end(),[&](T const& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),divide_point);
    
    std::future<std::list<T>>  new_lower(std::async(&parallel_quick_sort<T>,std::move(lower_part)));
    auto new_higher(parallel_quick_sort(std::move(input)));

    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower.get());

    return result;
}
int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-10,10);
    std::list<int> List;
    std::list<int> List1;
    generate_n(back_inserter(List),10000, bind(dist,gen));
    List1 = List;
    auto start = std::chrono::high_resolution_clock::now();
    auto result = sequential_quick_sort(List); 
    auto end = std::chrono::high_resolution_clock::now();
    cout <<endl;
    cout <<" time: "<< std::chrono::duration_cast<chrono::microseconds>(end - start).count()<<" ms"<<endl;
    cout <<endl;
    start = std::chrono::high_resolution_clock::now();
    auto result1 = parallel_quick_sort(List1); 
    end = std::chrono::high_resolution_clock::now();
    cout <<" time: "<< std::chrono::duration_cast<chrono::microseconds>(end - start).count()<<" ms"<<endl;
    
    cout <<endl;

    return 0;

}
