#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"

int main(){
  std::vector<std::vector<BBRBMTypeTraits::potentialType>> sample = getDataVector(data.csv); //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  const std::size_t dataNum = 1000;
  const std::size_t miniBatchSampleNum = 100;

  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randSampleKind(0,sample.size() - 1);
  std::uniform_int_distribution<std::size_t> randMiniBatchNum(0,dataNum/miniBatchSampleNum - 1);
  
  std::vector<std::vector<std::vector<BBRBMTypeTraits::potentialType>>> teacher;
  teacher.reserve(dataNum/miniBatchSampleNum);
  std::vector<std::vector<BBRBMTypeTraits::potentialType>> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < dataNum/miniBatchSampleNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      minibatch.at(i) = sample.at(randSampleKind(mt));
    }
    teacher.emplace_back(minibatch);
  }

  std::vector<std::shared_ptr<RBM<BBRBMTypeTraits>>> RBMptrs;
  for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
    std::shared_ptr<RBM<BBRBMTypeTraits>> RBMptr(new RBM<BBRBMTypeTraits>(teacher.at(randMiniBatchNum(mt)).at(i)));
    RBMptrs.emplace_back(RBMptr);
  }
  
  
  
  return 0;
}
