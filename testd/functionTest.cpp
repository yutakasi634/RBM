#include "../RBM.hpp"
#include "../vectorWrapper.hpp"
#include "../defaultTraits.hpp"
#include <iostream>

int main(){
  RBM<BBRBMTypeTraits>::RBMstaticGenerate(3,0);
  vector<int> pot{1,2,3};
  matrix<double> con{{1,0,1},{1,1,0},{0,0,1}};
  RBM<BBRBMTypeTraits>::connectionMatrix = con;
  std::cout << "connectionMatrix" << std::endl;
  std::cout << RBM<BBRBMTypeTraits>::connectionMatrix << std::endl;
  std::cout << "calculateH" << std::endl;
  std::cout << RBM<BBRBMTypeTraits>::calculateH(pot) << std::endl;
  std::cout << "calculateVH" << std::endl;
  std::cout << RBM<BBRBMTypeTraits>::calculateVH(pot) << std::endl;

  return 0;
}
