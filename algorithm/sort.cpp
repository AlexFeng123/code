#include<iostream>
#include<algorithm>
#include<vector>
#include<functional>
#include<chrono>
//debug
template<typename it>
void print_stl(const it &begin, const it &end)
{
   std::for_each(begin, end,[](const auto& item){ std::cout<<" "<< item;});
   std::cout<<std::endl;
}
//select sort
template<typename T>
void select_sort(std::vector<T> &v)
{
    size_t v_size(v.size());
    for(size_t i  = 0; i < v_size; ++i)
    {
        size_t min = i; 
        for(size_t j = i+1; j < v_size; ++j)
        {
            if(v[min] > v[j])
            {
                min  = j;
            }
        }
        if( min != i)
            std::swap(v[i],v[min]);
    }
}

//stl select sort
template<typename T>
void selection_sort_stl(std::vector<T> &v)
{
    for(auto it = v.begin(); it != v.end(); ++it)
    {
         std::iter_swap(it,std::min_element(it,v.end()));
    }
}

template<typename T>
void quick_sort_unidir(int begin, int end,std::vector<T> &v)
{
    if(begin >= end)
        return;
    T  pivot_data = v[end];
    auto new_pivot = begin;
    for(auto i = begin; i <= end; ++i)
    {
        if(v[i] <= pivot_data)
        {
            std::swap(v[i],v[new_pivot]);
            ++new_pivot;
        }
    }
    quick_sort_unidir(begin,new_pivot-2,v);
    quick_sort_unidir(new_pivot,end,v);
}

template<typename T>
void insertion_sort(std::vector<T> &v)
{
    size_t v_size(v.size());
    size_t i,j;
    T tmp;
    for(i = 1; i < v_size; ++i)
    {
       tmp = v[i];
       j = i;
       while(j > 0 && tmp < v[j-1])
       {
         v[j] = v[j-1];
         --j;
       }
       v[j] = tmp;
    }
}

template<typename T, typename cmp = std::less<T>>
void insertion_sort_stl(std::vector<T> &v)
{
    size_t v_size(v.size());
    size_t i;
    T tmp;
    for(i = 1; i < v_size; ++i)
    {
       tmp = v[i];
       auto it = std::lower_bound(v.begin(),v.begin()+i,tmp,cmp());
       std::copy(it,v.begin()+i,it+1);
       *it = tmp;
    }
}

template<typename T>
void quick_sort_bidir(int begin, int end,std::vector<T> &v)
{
    auto pivot = begin + ((end - begin)>>1);
    T  pivot_data = v[pivot];
    auto i = begin;
    auto j = end;
    while(i <= j)
    {
        while(v[j] >  pivot_data) --j;
        while(v[i] <  pivot_data) ++i;
        if(i <= j)
        {
            std::swap(v[i],v[j]);
            ++i;
            --j;
        }
    }
    if(begin < j) 
        quick_sort_bidir(begin,j,v);
    if(i < end)
        quick_sort_bidir(i,end,v);
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

template<typename T, typename cmp = std::less<T>>
void insertion_sort_stl(int begin, int end,std::vector<T> &v)
{
    int i;
    T tmp;
    for(i = begin+1; i <= end; ++i)
    {
       tmp = v[i];
       auto it = std::lower_bound(v.begin()+begin,v.begin()+i,tmp,cmp());
       std::copy(it,v.begin()+i,it+1);
       *it = tmp;
    }
}

template<typename T, int threshold = 16,  typename cmp = std::less<T>>
void interact_sort(int begin, int end,std::vector<T> &v)
{
    if(end - begin <= threshold)
    {
        insertion_sort_stl<T,cmp>(begin, end,v);
        return;
    }
    auto pivot = begin + ((end - begin)>>1);
    T  pivot_data = v[pivot];
    auto i = begin;
    auto j = end;
    while(i <= j)
    {
        while(v[j] >  pivot_data) --j;
        while(v[i] <  pivot_data) ++i;
        if(i <= j)
        {
            std::swap(v[i],v[j]);
            ++i;
            --j;
        }
    }
    if(begin < j) 
        interact_sort(begin,j,v);
    if(i < end)
        interact_sort(i,end,v);
}

void test_1(std::vector<int>  v)
{
    auto begin = std::chrono::system_clock::now();
    insertion_sort(v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_2(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    insertion_sort_stl(v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_3(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    quick_sort_unidir(0,v.size()-1,v);
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

void test_5(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    quick_sort_bidir(0,v.size()-1,v);
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}
void test_6(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    std::sort(v.begin(),v.end());
    auto end = std::chrono::system_clock::now();
    std::cout << "ms : " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<std::endl;
}

void test_7(std::vector<int> v)
{
    auto begin = std::chrono::system_clock::now();
    interact_sort(0,v.size()-1,v);
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
    //int i = 10000000;
    //auto des_f  =  [&]()->int { return i--;};
    //int i = 0;
    //auto inc_f  =  [&]()->int { return i++;};
    generate_n(std::back_inserter(v),1000000, std::bind(dist,gen));
    //generate_n(std::back_inserter(v),20000, std::bind(dist,gen));
    //generate_n(std::back_inserter(v),10, std::bind(dist,gen));
    //std::for_each(v.begin()+9999990,v.end(),[](const int &elem){std::cout <<" "<<elem;});
    //std::cout<<std::endl;   
    //print_stl(v.begin(), v.end());
    //test_1(v);
    //test_2(v); 
    //test_3(v); 
    //test_4(v); 
    test_5(v); 
    test_6(v); 
    test_7(v); 
    //std::for_each(v.begin()+9999990,v.end(),[](const int &elem){std::cout <<" "<<elem;});
    //std::cout<<std::endl;   
    //print_stl(v.begin(), v.end());
    return 0;
}
