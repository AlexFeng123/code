#include<iostream>
#include<atomic>
#include<thread>
#include<vector>
#include<cassert>
#include<chrono>
std::atomic<int>  cnt{0};
//volatile int  cnt{0};
using namespace std;
using namespace std::chrono;
void inc()
{
    for(int i = 0; i < 1000; ++i)
    {
        cnt++;
        //cnt.fetch_add(1,memory_order_relaxed);
    }
}
int main()
{
  auto start = system_clock::now();
  std::vector<std::thread> v;
  for(int i = 0; i < 10; ++i)
  {
      v.emplace_back(inc);
  }  
  for(auto &it:v)
  {
      it.join();
  }
  auto end =  system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end-start).count();
  assert(cnt == 10000);
  return 0;
}

