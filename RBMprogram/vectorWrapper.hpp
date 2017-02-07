#ifndef OPERATORFORVECTOR
#define OPERATORFORVECTOR
#include <iostream>
#include <vector>
#include <stdexcept>
#include <ostream>
#include <utility>
#include <type_traits>


//matrix,tensorの定義
template<typename T>
using vector = std::vector<T>;

template<typename T>
using matrix = vector<vector<T> >;

template<typename T>
using tensor = vector<matrix<T> >;

template<bool condition>
using enable_if_type = typename std::enable_if<condition,std::nullptr_t>::type;

//vectorに対する四則演算と<<のオーバーロード(前方宣言)
//+
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() + std::declval<T2>())>
operator+(const vector<T1>& self,const vector<T2>& other);

//-
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() - std::declval<T2>())>
operator-(const vector<T1>& self,const vector<T2>& other);

//vector * scholar
template<typename T1 ,typename T2, enable_if_type<std::is_arithmetic<T2>::value> = nullptr>
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const vector<T1>& self,const T2& other);

//scholar * vector
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T1>::value> = nullptr>
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const T1& self,const vector<T2>& other);


//vector * vector
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const vector<T1>& self,const vector<T2>& other);

// vector / vector
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const vector<T1>& self,const vector<T2>& other);

// vector / scholar
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T2>::value> = nullptr>
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const vector<T1>& self,const T2& other);

// scholar / vector
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T1>::value> = nullptr>
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const T1& self,const vector<T2>& other);

// << vector<scholar>
  template<typename T, enable_if_type<std::is_arithmetic<T>::value> = nullptr>
  std::ostream& operator<<(std::ostream& os, const vector<T>& self);

// << matrix,tensor
template<typename T, enable_if_type<not(std::is_arithmetic<T>::value)> = nullptr>
std::ostream& operator<<(std::ostream& os, const vector<T>& self);

//定義
//+
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() + std::declval<T2>())>
operator+(const vector<T1>& self,const vector<T2>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");  
  vector<decltype(std::declval<T1>() + std::declval<T2>())> result(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  auto otherItr = other.cbegin();
  while(resultItr != result.end() && selfItr != self.cend() &&
	otherItr != other.cend()){
    *resultItr = *selfItr + *otherItr;
    ++resultItr;
    ++selfItr;
    ++otherItr;
  }
  return result;
}

//-
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() - std::declval<T2>())>
operator-(const vector<T1>& self,const vector<T2>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");
  vector<decltype(std::declval<T1>() - std::declval<T2>())> result;
  result.resize(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  auto otherItr = other.cbegin();
  while(resultItr != result.end() && selfItr != self.cend() &&
	otherItr != other.cend()){
    *resultItr = *selfItr - *otherItr;
    ++resultItr;
    ++selfItr;
    ++otherItr;
  }  
  return result;
}


//vector * vector
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const vector<T1>& self,const vector<T2>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");  
  vector<decltype(std::declval<T1>() * std::declval<T2>())> result;
  result.resize(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  auto otherItr = other.cbegin();
  while(resultItr != result.end() && selfItr != self.cend() &&
	otherItr != other.cend()){
    *resultItr = *selfItr * *otherItr;
    ++resultItr;
    ++selfItr;
    ++otherItr;
  }  
  return result;
}

//vector * scholar
template<typename T1 ,typename T2, enable_if_type<std::is_arithmetic<T2>::value> >
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const vector<T1>& self,const T2& other){
  vector<decltype(std::declval<T1>() * std::declval<T2>())> result;
  result.resize(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  while(resultItr != result.end() && selfItr != self.cend()){
    *resultItr = *selfItr * other;
    ++resultItr;
    ++selfItr;
  }  
  return result;
}

//scholar * vector
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T1>::value> >
vector<decltype(std::declval<T1>() * std::declval<T2>())>
operator*(const T1& self,const vector<T2>& other){
  return other*self;
}

// vector / vector
template<typename T1, typename T2>
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const vector<T1>& self,const vector<T2>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");    
  vector<decltype(std::declval<T1>() / std::declval<T2>())> result;
  result.resize(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  auto otherItr = other.cbegin();
  while(resultItr != result.end() && selfItr != self.cend() &&
	otherItr != other.cend()){
    *resultItr = *selfItr / *otherItr;
    ++resultItr;
    ++selfItr;
    ++otherItr;
  }  
  return result;
}

// vector / scholar
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T2>::value> >
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const vector<T1>& self,const T2& other){ 
  vector<decltype(std::declval<T1>() / std::declval<T2>())> result;
  result.resize(self.size());
  auto resultItr = result.begin();
  auto selfItr = self.cbegin();
  while(resultItr != result.end() && selfItr != self.cend()){
    *resultItr = *selfItr / other;
    ++resultItr;
    ++selfItr;
  }  
  return result;
}

// scholar / vector
template<typename T1, typename T2, enable_if_type<std::is_arithmetic<T1>::value> >
vector<decltype(std::declval<T1>() / std::declval<T2>())>
operator/(const T1& self,const vector<T2>& other){
  vector<decltype(std::declval<T1>() / std::declval<T2>())> result;
  result.resize(other.size());
  auto resultItr = result.begin();
  auto otherItr = other.cbegin();
  while(resultItr != result.end() && otherItr != other.cend()){
    *resultItr = self / *otherItr;
    ++resultItr;
    ++otherItr;
  }    
  return result;
}

// << vector<scholar>
  template<typename T, enable_if_type<std::is_arithmetic<T>::value> >
std::ostream& operator<<(std::ostream& os, const vector<T>& self){
  auto itr = self.begin();
  while(itr != self.end() - 1){
    os << *itr << ",";
    ++itr;
  }
  os << *itr << std::endl;
  return os;
}

// << matrix,tensor
template<typename T, enable_if_type<not(std::is_arithmetic<T>::value)> >
std::ostream& operator<<(std::ostream& os, const vector<T>& self){
  auto itr = self.begin();
  while(itr != self.end() - 1){
    os << *itr;
    ++itr;
  }
  os << *itr << std::endl;
  return os;
}

#endif
