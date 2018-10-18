#include<iostream>
#include<typeinfo>
#include<typeindex>
#include<unordered_map>
#include<map>
#include<string>
using namespace std;
int f( int && a)
{
map<type_index,string> name;

name[type_index(typeid(int&&))] = "int&&";
name[type_index(typeid(int&))] = "int&";
name[type_index(typeid(int))] = "int";
  cout<<" type = " << name[std::type_index(typeid(a))]<<endl;
}

int f( int & a)
{
map<type_index,string> name;

name[type_index(typeid(int&&))] = "int&&";
name[type_index(typeid(int&))] = "int&";
name[type_index(typeid(int))] = "int";
  cout<<" x type = " << name[std::type_index(typeid(a))]<<endl;
}
void x(int && a)
{
    f(forward<int>(a));
    f(a);
}
int main()
{
  int a = 1;  
  x(std::move(a));   
  return 0;
}
