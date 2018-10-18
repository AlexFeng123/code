#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<chrono>
#include<thread>
#include<iostream>
#include<memory>
//#include <gperftools/profiler.h>
using namespace std;
const int thread_num = 10;
const int num = 100000;


int main(void) {
    //ProfilerStart("test.prof");
    for(int i = 0;  i < 10; i++)
    {
    int* a[thread_num][num];
    auto start = chrono::high_resolution_clock::now();
    auto alloc_func = [&](int index )
    {
        cout << " index = " << index<<endl;
        for(int i = 0; i < num; ++i)
        {
            a[index][i] = new int(i);
        }

        for(int i = 0; i < num; ++i)
        {
            delete a[index][i];
        }
    };
    std::unique_ptr<std::thread> t[thread_num];
    for(int i = 0; i < thread_num; ++i)
    {
       t[i] = std::make_unique<std::thread>(alloc_func,i);
    }
    for(int i = 0; i < thread_num; ++i)
    {
       t[i]->join(); 
    }
    auto end = chrono::high_resolution_clock::now();
    auto lTotalInitDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "lTotalInitDuration:" << lTotalInitDuration<<endl;
    }
    sleep(5);
    //ProfilerStop();
    return 0;
}
