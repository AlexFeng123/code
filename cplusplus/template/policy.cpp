#include<iostream>
#include <typeinfo>
#include<vector>
#include <cxxabi.h>
using namespace std;


struct true_poly_type{};
struct false_poly_type{};

template<typename T,bool isPoly>
struct isPolymoirphic {
using type =  false_poly_type;
};

template<typename T>
struct isPolymoirphic<T,true>
{
    using type = true_poly_type;
    using valueType = vector<T*>;
};

template<typename T>
struct isPolymoirphic<T,false>
{
    using type = false_poly_type;
    using valueType = vector<T>;
};

struct X
{
    using type = true_poly_type;
};

struct Y
{
    using type = false_poly_type;
};

template<typename T>
class fix_vector
{
public:
   using dataType = typename isPolymoirphic<T,is_same<typename T::type,true_poly_type>::value>::valueType;
private:

    dataType mData;
};

int main()
{
   int status;
  cout << abi::__cxa_demangle(typeid(fix_vector<X>::dataType).name(),0,0,&status) <<endl;  
  cout << abi::__cxa_demangle(typeid(fix_vector<Y>::dataType).name(),0,0,&status) <<endl;  
  return 0;
}
