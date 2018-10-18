#include<iostream>
#include<memory>
using namespace std;
int main()
{
    
  weak_ptr<int> wp;
  shared_ptr<int> sp1(new int(10)),sp2;
  wp  = sp1;
  wp.lock(); 
      cout << "x= "<< sp1.use_count()<<endl;
  sp2 = wp.lock();
  if(sp1 == sp2)
      cout << "equal"<< sp2.use_count()<<endl;
  sp1.reset();
      cout << "use count"<< sp2.use_count()<<wp.use_count()<<endl;
  if(wp.expired())
     cout << "xxx"<<endl; 
  if(wp.lock() == nullptr)
      cout << "null" <<endl;
  sp2.reset();  
      cout << "use count"<< sp2.use_count()<<wp.use_count()<<endl;
  return 0;
}
