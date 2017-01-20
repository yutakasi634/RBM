#ifndef operatorForVector
#define operatorForVector
#include <iostream>
#include <vector>
#include <stdexcept>

//matrix,tensorの定義
template<typename T>
using vector = std::vector<T>;

template<typename T>
using matrix = vector<vector<T> >;

template<typename T>
using tensor = vector<matrix<T> >;

//vectorに対する四則演算のオーバーロード
//+
template<typename T>
vector<T> operator+(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");  
  vector<T> result(self.size());
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
template<typename T>
vector<T> operator-(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");
  vector<T> result;
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
template<typename T>
vector<T> operator*(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");  
  vector<T> result;
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
template<typename T>
vector<T> operator*(const vector<T>& self,const T& other){
  vector<T> result;
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
template<typename T>
vector<T> operator*(const T& self,const vector<T>& other){
  return other*self;
}

//matrix * scholar
template<typename T>
matrix<T> operator*(const matrix<T>& self,const T& other){
  matrix<T> result;
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

//scholar * matrix
template<typename T>
matrix<T> operator*(const T& self,const matrix<T>& other){
  return other*self;
}

// vector / vector
template<typename T>
vector<T> operator/(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");    
  vector<T> result;
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
template<typename T>
vector<T> operator/(const vector<T>& self,const T& other){ 
  vector<T> result;
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

// matrix / scholar
template<typename T>
matrix<T> operator/(const matrix<T>& self,const T& other){ 
  matrix<T> result;
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
template<typename T>
vector<T> operator/(const T& self,const vector<T>& other){
  vector<T> result;
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

// scholar / matrix
template<typename T>
matrix<T> operator/(const T& self,const matrix<T>& other){
  matrix<T> result;
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

#endif
