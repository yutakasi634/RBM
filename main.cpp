#include "getDataVector.h"
#include "RBM.h"

int main(){
  std::mt19937 mt(0);
  std::uniform_int_distribution<> rand8(0,7);
  std::vector<std::vector<int>> sample = getDataVector(data.csv); //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  std::size_t dataNum = 1000;
  std::size_t miniBatchSampleNum = 100;
  std::vector<std::vector<std::vector<int>>> teacher;
  teacher.reserve(dataNum/miniBatchSampleNum);
  std::vector<std::vector<int>> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < dataNum/miniBatchSampleNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      minibatch.at(i) = sample.at(rand8(mt));
    }
    teacher.push_back(minibatch);
  }
  
  return 0;
}
