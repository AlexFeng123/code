#include<iostream>
#include<algorithm>
#include<vector>
#include<functional>
#include<chrono>
//select sort
template<typename T>
void select_sort(std::vector<T> &array)
{
    size_t array_size(array.size());
    for(size_t i  = 0; i < array_size; ++i)
    {
        size_t min = i; 
        for(size_t j = i+1; j < array_size; ++j)
        {
            if(array[min] > array[j])
            {
                min  = j;
            }
        }
        if( min != i)
            std::swap(array[i],array[min]);
    }
}

//stl select sort
template<typename T>
void selection_sort_stl(std::vector<T> &array)
{
    for(auto it =  array.begin(); it != array.end(); ++it)
    {
         std::iter_swap(it,std::min_element(it,array.end()));
    }
}

template<typename T>
void quick_sort(size_t begin, size_t end,std::vector<T> &array)
{
    if(begin >= end)
        return;
    auto pivot = begin;
    T  pivot_data = array[pivot];
    auto new_pivot = begin;
    for(auto i = begin+1; i <= end; ++i)
    {
        if(array[i] < pivot_data)
        {
            std::swap(array[i],array[new_pivot]);
            ++new_pivot;
        }
    }
    quick_sort(begin,new_pivot,array);
    quick_sort(new_pivot+1,end,array);
}
template<typename T>
void insertion_sort(std::vector<T> &v)
{

}

template<typename T>
void quick_sort_v3(int begin, int end,std::vector<T> &arr)
{
    auto pivot = begin + ((end - begin)>>1);
    T  pivot_data = arr[pivot];
    auto i = begin;
    auto j = end;
    while(i <= j)
    {
        while(arr[j] >  pivot_data) --j;
        while(arr[i] <  pivot_data) ++i;
        if(i <= j)
        {
            std::swap(arr[i],arr[j]);
            ++i;
            --j;
        }
    }
    ////std::for_each(arr.begin(),arr.end(),[](const int &elem){std::cout <<" "<<elem;});
    //std::cout<<std::endl;
    if(begin < j) 
        quick_sort_v3(begin,j,arr);
    if(i < end)
        quick_sort_v3(i,end,arr);
}


template<typename it>
void quick_sort_stl(it begin, it end)
{
    if(begin == end) return ;
    auto pivot = *std::next(begin,std::distance(begin,end)/2);
    auto middle = std::partition(begin,end,[pivot](const auto&em){ return em < pivot;});
    auto middle2 = std::partition(middle,end,[pivot](const auto&em){ return !(em > pivot);});
    quick_sort_stl(begin,middle);
    quick_sort_stl(middle2,end);
}
void test_1(std::vector<int>  v)
{
    auto begin = std::chrono::system_clock::now();
    select_sort(v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_2(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    selection_sort_stl(v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_3(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    quick_sort(0,v.size()-1,v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_4(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    quick_sort_stl(v.begin(),v.end());
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_5(std::vector<int> &v)
{
    auto begin = std::chrono::system_clock::now();
    quick_sort_v3(0,v.size()-1,v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}
void test_7(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    std::sort(v.begin(),v.end());
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-10000,10000);
    //std::vector<int> v{-54, -84, -85, 28, 56, 19, -97, 98, 90, -81};
    //std::vector<int> v{10,15,14,13,12};
    //std::vector<int> v(1000000,1);
    std::vector<int> v;
    int i = 10000000;
    auto  f =  [&]()->int { return i--;};
    generate_n(std::back_inserter(v),10000000, std::bind(dist,gen));
    std::for_each(v.begin()+9999990,v.end(),[](const int &elem){std::cout <<" "<<elem;});
    std::cout<<std::endl;   
    //test_1(v);
    //test_2(v); 
    //test_3(v); 
    test_4(v); 
    test_7(v); 
    test_5(v); 
    std::for_each(v.begin()+9999990,v.end(),[](const int &elem){std::cout <<" "<<elem;});
    std::cout<<std::endl;   
    return 0;
}
