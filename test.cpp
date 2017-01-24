#include <fstream>
#include <iostream>
#include "vectorWrapper.hpp"
#include "getDataVector.hpp"
#include "defaultTraits.hpp"

int main(){

  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randbool(0,1);
  
  RBMstaticGenerate<BBRBMTypeTraits>(100,0);
  RBM<BBRBMTypeTraits>::connectionMatrix = getDataVector("connection.dat");
  vector<BBRBMTypeTraits::potentialType> initialValue;
  for(std::size_t i = 0; i < RBM<BBRBMTypeTraits>::totalNodeNum; ++i){
    initialValue.emplace_back(randbool(mt));
  }  
  RBM<BBRBMTypeTraits> rbm(initialValue);
  
  return 0;
}
