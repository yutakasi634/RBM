#include <iostream>
#include "vectorWrapper.hpp"

int main(){
  tensor<int> testv3;
  for(std::size_t j = 0; j < 10; ++j){
    matrix<int> testv;
    for(std::size_t i = 0; i < 10; ++i){
      vector<int> testv2(10,i);
      testv.emplace_back(testv2);
    }
    testv3.emplace_back(testv);
  }
  std::cout << testv3 << std::endl;
  
  return 0;
}
