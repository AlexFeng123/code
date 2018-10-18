#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<atomic>
using namespace std;
std::mutex mut;
std::condition_variable data_cond;
atomic<int> i;
class A
{
public:
    void operator()()
    {
       while(true)
       {
           i = 2;
           if(i == 3)cout<< "add x"<<endl; 
      }
    }
};
void fun()
{ 
    while(true)
    {
        i = 3;
        if(i == 2)cout << "hello world!"<<endl;
    }
}
int main()
{
  thread t2(fun);
  thread t((A()));
  t2.join();
  t.join();  
  return 0;
}
