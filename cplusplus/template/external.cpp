#include<iostream>
#include<tuple>
using namespace std;
template<typename head, typename... tail>
struct tuple<head,tail...>: private tuple<tail...>
{
    using inherited = tuple<tail...>;
    tuple{}
    tuple(typename add_const_reference<head>::type v, typename add_const_reference<tail>::type... vtail)
        :m_head(v),inherited(vtail...){}
    template<typename ... vvalues>
    tuple(const tuple<vvalues...>& other):m_head(other.head()),inherited(other.tail())
    {}
    template<typename ... vvalues>
    tuple& operator==(const tuple <vvalues...>& other)
    {
        m_head = other.head();
        tail() =  other.tail();
        return *this;
    }

    typename add_reference<head>::type head(){return m_head;}
    typename add_reference<const head>::type head() const {return m_head;}

    inherited & tail(){return *this;}
    const inherited & tail() const { return *this;}

    protected:
    head m_head;

};

int main()
{
  tuple<string, vector,double> tt("hello",{1,2,3},1.2);
  string h = tt.head();
  tuple<vector,double> t2 = tt.tail();
  tuple<double> t3 = t2.tail();
  tuple<> t4  = t3.tail();
  cout <<"tt"<< get<0>(tt)<< "t2="<< get<0>(t2) <<" t3 " << get<0>(t3) << " t4 "<<t4<<endl;
  return 0;
}
