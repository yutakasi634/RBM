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
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] + other[i];
  return result;
}

//-
template<typename T>
vector<T> operator-(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");
  vector<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] - other[i];
  return result;
}


//vector * vector
template<typename T>
vector<T> operator*(const vector<T>& self,const vector<T>& other){
  if(self.size() != other.size())
    throw std::invalid_argument("You put different size vector.");  
  vector<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] * other[i];
  return result;
}

//vector * scholar
template<typename T>
vector<T> operator*(const vector<T>& self,const T& other){
  vector<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] * other;
  return result;
}

//matrix * scholar
template<typename T>
matrix<T> operator*(const matrix<T>& self,const T& other){
  matrix<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] * other;
  return result;
}

//scholar * vector
template<typename T>
vector<T> operator*(const T& self,const vector<T>& other){
  return other*self;
}

//scholar * vector
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
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] / other[i];
  return result;
}

// vector / scholar
template<typename T>
vector<T> operator/(const vector<T>& self,const T& other){ 
  vector<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] / other;
  return result;
}

// matrix / scholar
template<typename T>
matrix<T> operator/(const matrix<T>& self,const T& other){ 
  matrix<T> result;
  result.resize(self.size());
  for(std::size_t i = 0; i < self.size(); ++i)
    result[i] = self[i] / other;
  return result;
}

// scholar / vector
template<typename T>
vector<T> operator/(const T& self,const vector<T>& other){
  vector<T> result;
  result.resize(other.size());
  for(std::size_t i = 0; i < other.size(); ++i)
    result[i] = self / other[i];
  return result;
}

// scholar / matrix
template<typename T>
matrix<T> operator/(const T& self,const matrix<T>& other){
  matrix<T> result;
  result.resize(other.size());
  for(std::size_t i = 0; i < other.size(); ++i)
    result[i] = self / other[i];
  return result;
}

#endif
