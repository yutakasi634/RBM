#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"
#include "vectorWrapper.hpp"
#include <memory>

int main(){
  const std::size_t dataNum = 1000;
  const std::size_t miniBatchSampleNum = 100;
  
  //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  vector<vector<BBRBMTypeTraits::potentialType>>
    sample = getDataVector<BBRBMTypeTraits::potentialType>("data.csv");

  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randSampleKind(0,sample.size() - 1);
  std::uniform_int_distribution<std::size_t> randMiniBatchNum(0,dataNum/miniBatchSampleNum - 1);

  //教師データ集合を用意する
  tensor<BBRBMTypeTraits::potentialType> teacher;
  teacher.reserve(dataNum/miniBatchSampleNum);
  matrix<BBRBMTypeTraits::potentialType> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < dataNum/miniBatchSampleNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      miniBatch.at(i) = sample.at(randSampleKind(mt));
    }
    teacher.emplace_back(miniBatch);
  }

  //ミニバッチごとのpotentialの平均値を計算し、保持しておく
  tensor<double> dataVHmeans;
  matrix<double> dataVmeans,dataHmeans;
  for(std::size_t batchNum = 0; batchNum < dataNum/miniBatchSampleNum; ++batchNum){
    dataVHmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateVH(teacher.at(batchNum));
    dataVmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateV(teacher.at(batchNum));
    dataHmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateH(teacher.at(batchNum));
  }
  
  //ミニバッチのサンプル数だけRBMを用意する
  RBM<BBRBMTypeTraits>::RBMstaticGenerate(0);
  vector<std::shared_ptr<RBM<BBRBMTypeTraits>>> RBMptrs;
  for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
    std::shared_ptr<RBM<BBRBMTypeTraits>> RBMptr(new RBM<BBRBMTypeTraits>(teacher.at(randMiniBatchNum(mt)).at(i)));
    RBMptrs.emplace_back(RBMptr);
  }

  //connectionMatrixを更新する
  double epsilon;
  
  matrix<double> rbmVHmeans;
  matrix<double> deltaConnection,deltaBias;
  vector<int> rbmVsums;
  vector<double> rbmHmeans;
  RBMptrs.at(0)->timeEvolution();
  rbmVHmeans = RBM<BBRBMTypeTraits>::calculateVH((RBMptrs.at(0)->getPotential()).at(0));
  rbmVsums = RBMptrs.at(0)->getPotential().at(0);
  rbmHmeans = RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(0)->getPotential()).at(0));
  for(std::size_t rbmNum = 1; rbmNum < miniBatchSampleNum; ++rbmNum){
    RBMptrs.at(rbmNum)->timeEvolution();
    rbmVHmeans = rbmVHmeans + RBM<BBRBMTypeTraits>::calculateVH((RBMptrs.at(rbmNum)->getPotential()).at(0));
    rbmVsums = rbmVsums + RBMptrs.at(rbmNum)->getPotential().at(0);
    rbmHmeans = rbmHmeans + RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(rbmNum)->getPotential()).at(0));
  }
  vector<double> rbmVmeans(rbmVsums.begin(),rbmVsums.end());
  rbmVHmeans = rbmVHmeans / (double)miniBatchSampleNum;
  rbmVmeans = rbmVmeans / (double)miniBatchSampleNum;
  rbmHmeans = rbmHmeans / (double)miniBatchSampleNum;

  std::size_t miniBatchNum = randMiniBatchNum(mt);
  deltaConnection = epsilon*(dataVHmeans.at(miniBatchNum) - rbmVHmeans);
  deltaBias = epsilon*(dataVmeans(miniBatchNum) - rbm);

  //  RBM<BBRBMTypeTraits>::setConnectionMatrix();
  //  RBM<BBRBMTypeTraits>::setBias();
  
  return 0;
}
