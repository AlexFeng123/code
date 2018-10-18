#include<iostream>
#include<future>
#include<chrono>
using namespace std;
class task{};
template<typename T>
T sum( const T & a,const T & b)
{
    this_thread::sleep_for(chrono::seconds(2));
    return a+b;
}

template<typename T>
void print(std::future<T> &fut)
{
    T x = fut.get();
    cout << " value: "<< x << endl;
}

void test1()
{
  future<int> my_future = async(sum<int>,10,10);
  cout << "xxxxx"<<endl;
  cout << my_future.get();
  cout << "yyyy" <<endl;  
}

void test2()
{
  std::promise<int> pom;
  auto fut = pom.get_future();
  thread  t1(print<int>,std::ref(fut));
  pom.set_value(10);
  t1.join();
}
void test3()
{
  std::chrono::system_clock::time_point two_seconds_passed 
      =  chrono::system_clock::now() +  chrono::seconds(2);
  std::promise<int> pom1;
  auto fut_completes = pom1.get_future();
  thread([](std::promise<int> p1)
          {
            this_thread::sleep_for(chrono::seconds(1));
            p1.set_value(9);
          },
          std::move(pom1)
        ).detach();

  std::promise<int> pom2;
  auto fut_time_out = pom2.get_future();
  thread([](std::promise<int> p2)
          {
            this_thread::sleep_for(chrono::seconds(5));
            p2.set_value(8);
          },
          std::move(pom2)
        ).detach();
  std::cout << "Waiting for 2 seconds..." << std::endl;
  if(std::future_status::ready == fut_completes.wait_until(two_seconds_passed))
  { std::cout << "f_completes: " << fut_completes.get() << "\n"; }
  else
  { std::cout << "f_completes did not complete!\n"; }

  if(std::future_status::ready == fut_time_out.wait_until(two_seconds_passed))
  { std::cout << "f_completes: " << fut_time_out.get() << "\n"; }
  else
  { std::cout << "f_completes did not complete!\n"; }
}
int main()
{
  test3();  
  return 0;
}
