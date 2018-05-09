#include<iostream>
#include<string>
#include<tuple>
using namespace std;
class rational
{
public:
      rational();
      rational(int num, int den);
      rational(int val);
      rational(const rational& val);
      rational(const rational&& val);
      ~rational() = default;

      rational& operator=(const rational &val);
      rational& operator+=(const rational &val);
      rational& operator-=(const rational &val);
      rational& operator*=(const rational &val);
      rational& operator/=(const rational &val);

      rational& operator+(int val);
      rational& operator*(int val);
      rational& operator-(int val);
      rational& operator/(int val);
      
      rational& operator++();
      rational operator++(int);

      template<typename ...Args>
      static inline  void print(Args...args)
      {
#ifdef DEBUG
	  size_t args_num = sizeof...(Args);
	  tuple<Args...> tp = std::make_tuple(args...);
          for_each_tuple(tp,[](auto & elem){ cout << " " <<elem;});
          cout <<endl;
#endif
      }

      friend ostream& operator<<(ostream & os, const rational &val);
      friend istream& operator>>(istream & is, rational &val);
      friend rational operator+(const  rational &lval, const rational &rval);
      friend rational operator-(const  rational &lval, const rational &rval);
      friend rational operator*(const  rational &lval, const rational &rval);
      friend rational operator/(const  rational &lval, const rational &rval);
      friend inline bool operator<(const  rational &lval, const rational &rval)
      {
	      return rational::cmp(lval,rval) < 0;
      }

      friend inline bool operator==(const  rational &lval, const rational &rval)
      {
	      return rational::cmp(lval, rval) == 0;
      }
      
      friend inline bool operator>(const  rational &lval, const rational &rval)
      {
	      return rational::cmp(lval,rval) > 0;
      }

private:
      int mNum;
      int mDen;
      static int gcd(int a, int b);
      static int lcm(int a, int b);
      static int cmp(const rational &lval, const rational &rval);
      inline void normalize()
      {
          if(mDen < 0)
          {
             mDen = -mDen;
             mNum = -mNum;
          }
      }
};


rational::rational():mNum(0),mDen(1){}

rational::rational(int num, int den):mNum(num),mDen(den){    normalize();}

rational::rational(const rational& val):mNum(val.mNum),mDen(val.mDen){}

rational::rational(const rational &&val):mNum(std::move(val.mNum)),mDen(move(val.mDen)){}

rational& rational::operator=(const rational &val)
{ 
    if(&val == this)
        return *this;
    
    mNum = val.mNum;
    mDen = val.mDen;
    return *this;
}

rational& rational::operator+=(const rational &val)
{
    print(__LINE__,*this,val);
    int lLcm = lcm(mDen, val.mDen);
    mNum = (lLcm/mDen)*mNum + (lLcm/val.mDen)*val.mNum;
    mDen = lLcm;
    print(__LINE__,lLcm,mNum);
    return *this;
}

rational& rational::operator+(int val)
{
    mNum += val*mDen;
    return *this;
}

rational& rational::operator-(int val)
{
    mNum -= val*mDen;
    return *this;
}

rational& rational::operator*(int val)
{
    mNum *= val;
    return *this;
}

rational& rational::operator/(int val)
{
    mDen *= val;
    return *this;
}


rational& rational::operator-=(const rational &val)
{
    int lLcm = lcm(mDen, val.mDen);
    mNum = (lLcm/mDen)*mNum - (lLcm/val.mDen)*val.mNum;
    mDen = lLcm;
    return *this;
}

rational& rational::operator*=(const rational &val)
{
    mNum *= val.mNum;
    mDen *= val.mDen;
    return *this;
}

rational& rational::operator/=(const rational &val)
{
    mNum *= val.mDen;
    mDen *= val.mNum;
    normalize();
    return *this;
}

rational rational::operator++(int)
{
    rational tmp(mNum,mDen);
    mNum += mDen;
    return tmp;
}

rational& rational::operator++()
{
   mNum += mDen;
   return *this;
}

rational operator+(const  rational &lval, const rational &rval)
{
    rational tmp(lval);
    tmp += rval;
    return tmp;
}

rational operator-(const  rational &lval, const rational &rval)
{
    rational tmp(lval);
    tmp -= rval;
    return tmp;
}

rational operator*(const  rational &lval, const rational &rval)
{
    rational tmp(lval);
    tmp *= rval;
    return tmp;
}

rational operator/(const  rational &lval, const rational &rval)
{
    rational tmp(lval);
    tmp /= rval;
    return tmp;
}

ostream& operator<<(ostream &os, const rational &val)
{
   int lGcd =rational::gcd(val.mNum,val.mDen);
   os << val.mNum/lGcd<<"/"<<val.mDen/lGcd;
   return os;
}

istream& operator>>(istream &is,  rational &val)
{
   char slash;
   int num = 0;
   int den = 1;
   is >> num >> slash >>den;
   val = rational(num,den);
   return is;
}

int rational::gcd(int a, int b)
{
   a = abs(a);
   b = abs(b);
   if(a < b) swap(a,b);
   print(__LINE__,a,b);
   while(b != 0)
   {
      int tmp = a;
      a = b;
      b = tmp%b;
   }
   print(__LINE__,a);
   return a;
}

int rational::lcm(int a, int b)
{
    int x = (a*b)/gcd(a,b);
   print(__LINE__,a,b,x);
   return x;
}

int rational::cmp(const rational &lval, const rational &rval)
{
    rational tmp(lval-rval);
    return tmp.mNum - 0;
}


