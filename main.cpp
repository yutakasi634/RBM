#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"

int main(){
  const std::size_t dataNum = 1000;
  const std::size_t miniBatchSampleNum = 100;
  
  //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  std::vector<std::vector<BBRBMTypeTraits::potentialType>> sample = getDataVector(data.csv);

  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randSampleKind(0,sample.size() - 1);
  std::uniform_int_distribution<std::size_t> randMiniBatchNum(0,dataNum/miniBatchSampleNum - 1);

  //教師データ集合を用意する
  std::vector<std::vector<std::vector<BBRBMTypeTraits::potentialType>>> teacher;
  teacher.reserve(dataNum/miniBatchSampleNum);
  std::vector<std::vector<BBRBMTypeTraits::potentialType>> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < dataNum/miniBatchSampleNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      minibatch.at(i) = sample.at(randSampleKind(mt));
    }
    teacher.emplace_back(minibatch);
  }

  //ミニバッチごとのpotentialの平均値を計算し、保持しておく
  std::vector<std::vector<std::vector<double> > > VHmeans;
  std::vector<std::vector<double> > Vmeans,Hmeans;
  for(std::size_t batchNum = 0; batchNum < dataNum/miniBatchSampleNum; ++batchNum){
    VHmeans = RBM::batchDataMeanCalculateVH(teacher.at(batchNum));
    Vmeans = RBM::batchDataMeanCalculateV(teacher.at(batchNum));
    Hmeans = RBM::batchDataMeanCalculateH(teacher.at(batchNum));
  }
  
  //ミニバッチのサンプル数だけRBMを用意する
  RBM::RBMstaticGenerate(0);
  std::vector<std::shared_ptr<RBM<BBRBMTypeTraits>>> RBMptrs;
  for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
    std::shared_ptr<RBM<BBRBMTypeTraits>> RBMptr(new RBM<BBRBMTypeTraits>(teacher.at(randMiniBatchNum(mt)).at(i),mt));
    RBMptrs.emplace_back(RBMptr);
  }

  //connectionMatrixを更新する
  for(std::size_t rbmNum = 0; rbmNum < miniBatchSampleNum; ++rbmNum){
    RBMptrs.at(rbmNum)->timeEvolution();
    RBM::batchDataMeanCalculateVH((RBMptrs.at(rbmNum)->getPotential()).at(0));
    RBM::batchDataMeanCalculateV((RBMptrs.at(rbmNum)->getPotential()).at(0));
    RBM::batchDataMeanCalculateH((RBMptrs.at(rbmNum)->getPotential()).at(0));
  }
  
  
  return 0;
}
