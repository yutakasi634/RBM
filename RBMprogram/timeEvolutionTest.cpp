#include <iostream>
#include <fstream>
#include "vectorWrapper.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"
#include "getDataVector.hpp"

using RBMTtraits = BBRBMTypeTraits;

int main(int argc, char *argv[]){
  const std::size_t totalOutputStep = 1000;
  
  if(argc != 2){
    std::cout << "引数の数が不適切です" << std::endl;
    return 1;
  }
  matrix<RBMTtraits::connectionType> connection
    = getDataVector<RBMTtraits::connectionType>(argv[1]);
  //std::cout << "connection" << std::endl << connection << std::endl;
  const std::size_t nodeNum(connection.size());
  std::cout << "connection " << nodeNum << "×" << connection.at(0).size() << std::endl;
  std::ofstream fout("outputGenarate.dat");
  std::mt19937 randgen(1);
  std::uniform_int_distribution<> rand(0,1);
  vector<RBMTtraits::potentialType> init;
  for(std::size_t i = 0; i < nodeNum; ++i){
    init.emplace_back(rand(randgen));
  }
  RBM<RBMTtraits>::RBMstaticGenerate(nodeNum,2);
  RBM<RBMTtraits>::setConnectionMatrix(connection);
  std::cout << "set connection Matrix compleated." << std::endl;
  RBM<RBMTtraits> rbm(init);
  std::cout << "potentials " << std::endl << rbm.getPotential();
  std::cout << "set RBM compleated." << std::endl;
  vector<RBMTtraits::potentialType> temp;
  std::size_t culum(static_cast<std::size_t>(std::sqrt(RBM<RBMTtraits>::totalNodeNum)));
  std::size_t row(culum);
  for(std::size_t i = 0; i < totalOutputStep; ++i){
    rbm.timeEvolution();
    //std::cout << "step " << i << std::endl;
    temp = rbm.getPotential().at(0);
    //std::cout << "temp " << temp << std::endl;
    auto itr = temp.begin();
    for(std::size_t j = 0; j < row; ++j){
      for(std::size_t k = 0; k < culum; ++k){
	fout << *itr << ",";
	//std::cout << "j k " << j << k << std::endl;
	++itr;
      }
      fout << std::endl;
    }
    fout << std::endl;
  }
  std::cout << "output compleated." << std::endl;
  fout.close();
  return 0;
}
