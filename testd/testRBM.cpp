#include <fstream>
#include <iostream>
#include "../vectorWrapper.hpp"
#include "../defaultTraits.hpp"
#include "../RBM.hpp"
#include "../getDataVector.hpp"

int main(int argc, char *argv[]){//step
  if(argc < 2){
    std::cout << "引数が足りません" << std::endl;
    return 1;
  }

  std::size_t nodeNum = 4;
  int randomSeed = 0;
  std::size_t totalStep = std::atoi(argv[1]);
  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randbool(0,1);

  std::ofstream fout("testRBMPotentials.dat");
  RBM<BBRBMTypeTraits>::RBMstaticGenerate(nodeNum, randomSeed);
  RBM<BBRBMTypeTraits>::connectionMatrix = getDataVector<BBRBMTypeTraits::connectionType>("testRBMConnection.dat");
  vector<BBRBMTypeTraits::potentialType> initialValue;
  for(std::size_t i = 0; i < RBM<BBRBMTypeTraits>::totalNodeNum; ++i){
    initialValue.emplace_back(randbool(mt));
  }

  std::cout << "initialValue" << std::endl;
  std::cout << initialValue;
  
  std::size_t outputculum = 2;
  std::size_t outputraw = 2;
  assert(nodeNum == outputculum * outputraw);
  
  RBM<BBRBMTypeTraits> rbm(initialValue);
  vector<BBRBMTypeTraits::potentialType> outputVecor;
  for(std::size_t i = 0; i < totalStep; ++i){
    rbm.timeEvolution();
    //    std::cout << i << std::endl;
    if((i % 100) == 0){
      outputVecor = rbm.getPotential().at(0);
      auto itr = outputVecor.begin();
      for(std::size_t i = 0; i  < outputculum; ++i){
	for(std::size_t j = 0; j < outputraw; ++j){
	  fout << *itr;
	  ++itr;
	}
	fout << std::endl;
      }
      fout << std::endl;
    }
  }
  fout.close();
  
  return 0;
}
