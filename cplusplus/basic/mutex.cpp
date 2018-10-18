#include<iostream>
#include<thread>
using namespace std;
struct BigNum
{
    BigNum&  operator++()
    {
        this->a++;
        this->b++;
        return *this;
    }
    BigNum operator++(int)
    {
        BigNum  tmp(*this);
        operator++();
        return tmp;
    }
    BigNum():a(0),b(0){}
    BigNum& operator=(const BigNum &val)
    {
        this->a = val.a;
        this->b = val.b;
        return *this;
    }
    BigNum& operator=(const  int &&val)
    {
        this->a = val;
        this->b = val;
        return *this;
    }
  int a;
  int b;
};
std::ostream& operator<<(std::ostream& os, const  BigNum &val)
{
    os<<"a=" <<val.a <<",b="<<val.b;
    return os; 
}
BigNum  g_i;
void safe_inc()
{
    ++g_i;
    cout << std::this_thread::get_id() <<": "<<g_i<<endl;
}
int main()
{
    cout <<"main :"<<g_i<<endl;
    std::thread t1(safe_inc);
    std::thread t2(safe_inc);
  t1.join();
  t2.join();  
  return 0;
}
