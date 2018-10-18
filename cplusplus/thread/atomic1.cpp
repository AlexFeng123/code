#include<iostream>
#include<atomic>
#include<thread>
#include<vector>
#include<cassert>
#include<chrono>
std::atomic<bool> flag{false};
int  data{0};
using namespace std;
using namespace std::chrono;
void producer()
{
    data = 100;
    flag.store(true,memory_order_release); 
}
void consumer()
{
   while(!flag.load(memory_order_acquire)); 
   assert(data==100);     
}
int main()
{
  thread t1(producer);
  thread t2(consumer);
  t1.join();
  t2.join();
 
  return 0;
}

