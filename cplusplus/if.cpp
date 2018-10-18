#include<iostream>
using namespace std;
template<bool c, typename Then, typename Else> struct IF_{};
template<typename Then, typename Else>
struct IF_<true, Then, Else> {  using reType=Then;};

template<typename Then, typename Else>
struct IF_<false, Then, Else>{  using reType=Else;};

template<typename Statement> 
 struct STOP{ using reType=Statement;};
 
template<template<typename> class Condition, typename Statement>
struct WHILE_{
 using reType = typename IF_<Condition<Statement>::ret,
                         WHILE_<Condition, typename Statement::Next>,
                         STOP<Statement>>::reType::reType;
};

 template<int i, int x> struct pow_e{  enum { ret = i*pow_e<i,x-1>::ret};};
 template<int i> struct pow_e<i,0>{ enum { ret = 1};};
 template<int i,int x > struct pow{  enum{ ret = pow_e<i,x>::ret};};
template<int n,int x>
struct sum_pow{
 template<typename stat>
 struct cond{  enum{ ret = (stat::ri<=n)};};
 template<int i, int sum> 
 struct stat{  using Next=stat<i+1,sum+pow<i,x>::ret>;
             enum {ri = i, ret = sum};};
    
 enum{ ret = WHILE_<cond, stat<1,0>>::reType::ret};
 
};

int main()
{
    //using int_my = IF_<sizeof(short)==len,short,
          //IF_<sizeof(int)==len,int,
          //IF_<sizeof(long)==len,long,
          //IF_<sizeof(long)==len,long,
          //char>::retType>::retType>::retType>::retType;
    //typedef IF_<sizeof(short)==len,short,
          //IF_<sizeof(int)==len,int,
          //IF_<sizeof(long)==len,long,
          //IF_<sizxof(long)==lxn,long,
         //char>::retType>::retType>::retType>::retType int_my;
   cout << sum_pow<10,2>::ret<<endl;
    return 0;
}
