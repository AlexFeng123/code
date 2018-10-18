#include<iostream>
#include<functional>
#include<map>
#include<string>
#include<memory>
using namespace std;
class ClassFactory
{
public:
   using callBack  = std::function<void*(void)>;
public:
    static ClassFactory& getInstance()
    {
        static ClassFactory mClassFactory;
        return mClassFactory;
    }

     void registerClass(const string &pClassName, callBack && pCallBack)
     {
         mMap.emplace(pClassName, std::forward<callBack>(pCallBack));
     }
     void* getClassByName(const string &pClassName)
     {
         auto it = mMap.find(pClassName);
         if(it != mMap.end())
         {
             return it->second();
         }
         return nullptr;
     }
private:
   ClassFactory() = default;
   std::map<string,callBack> mMap;
};
struct Register
{
    Register(const string& pClassName, ClassFactory::callBack &&pCallBack)
    {
         ClassFactory::getInstance().registerClass(pClassName,std::forward<ClassFactory::callBack>(pCallBack));
    }
};
#define REGISTER(className)\
  className* createClass##className() {     \
      return new className;              \
  }                                      \
  Register  register##className(#className,ClassFactory::callBack(createClass##className))
//test class
class test
{
public:
    void show(){cout << "test" <<endl;}
    ~test(){cout <<"~test"<<endl;}
};
REGISTER(test);
int main()
{
   unique_ptr<test> testObj;
   testObj.reset(static_cast<test*>(ClassFactory::getInstance().getClassByName("test")));
   if(testObj)
   {
      testObj->show();
   }
   return 0;
}
