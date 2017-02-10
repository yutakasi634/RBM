//オブジェクトを生成する前にrandomNumbeGenerate()を使用しておく
#ifndef SOTUKENRBM
#define SOTUKENRBM
#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <fstream>
#include "vectorWrapper.hpp"

template<typename T_traits>
class RBM{
  typedef typename T_traits::potentialType potentialType;
  typedef typename T_traits::connectionType connectionType;
  typedef typename T_traits::biasType biasType;
protected:
  matrix<potentialType> potential;
  std::normal_distribution<> dist;
  std::uniform_real_distribution<double> rand;
public:
  static std::size_t totalNodeNum;
  static matrix<connectionType> connectionMatrix;
  //connectionMatrix[i][j]はvisible layerのi番目nodeとhidden layerのj番目の接続の重み
  static matrix<biasType> bias;
  static std::mt19937 randomNumberGenerator;

  RBM(vector<potentialType>& initialValues);
  static void RBMstaticGenerate(int nodeNum, int randomSeed);
  void timeEvolution();
  const matrix<potentialType> getPotential();
  void setVisiblePotentials(const vector<potentialType>& values){potential.at(0) = values;};
  static void setBias(const matrix<biasType>& values){bias = values;};
  static void setConnectionMatrix(const matrix<connectionType>& weightMatrix){connectionMatrix = weightMatrix;};
  
  double
  calculateInput(std::size_t layerNum, std::size_t nodeNum);
  
  static
  double
  calculateInput(std::size_t layerNum, std::size_t nodeNum,
		 const vector<potentialType>& nodePotentials);

  potentialType activate(std::size_t layerNum, std::size_t nodeNum);
  
  static
  vector<double>
  calculateH(const vector<potentialType>& visibleLayer);

  static
  matrix<double>  
  calculateVH(const vector<potentialType>& visibleLayer);
  
  static
  matrix<double>
  batchDataMeanCalculateVH(const matrix<potentialType>& miniBatch);
  
  static vector<double> batchDataMeanCalculateV(const matrix<potentialType>& minibatch);
  
  static
  vector<double>
  batchDataMeanCalculateH(const matrix<potentialType>& minibatch);
};

template<typename T_traits>
RBM<T_traits>::RBM(vector<potentialType>& initialValues){
  assert(initialValues.size() == totalNodeNum);
  std::uniform_real_distribution<>::param_type uniformDistParam(0.0, 1.0);
  rand.param(uniformDistParam);
  std::normal_distribution<>::param_type normalDistParam(0.0, 1.0);
  dist.param(normalDistParam);
  potential.push_back(initialValues);
  vector<potentialType> hiddenLayer(totalNodeNum);
  potential.emplace_back(hiddenLayer);
}

template<typename T_traits>
const matrix<typename RBM<T_traits>::potentialType>
RBM<T_traits>::getPotential(){
  return potential;
}

template<typename T_traits>
double RBM<T_traits>::calculateInput(std::size_t layerNum, std::size_t nodeNum){
  if(layerNum != 0 and layerNum != 1)
    throw std::invalid_argument("You put invalid layerNum.");
  double sum = calculateInput(layerNum, nodeNum,potential.at((layerNum + 1) % 2));
  return sum;
}

template<typename T_traits>
double RBM<T_traits>::calculateInput(std::size_t layerNum, std::size_t nodeNum,
				     const vector<potentialType>& nodePotentials){
  if(layerNum != 0 and layerNum != 1)
    throw std::invalid_argument("You put invalid layerNum.");
  double sum = 0.0;
  if(layerNum == 0){//可視層の場合
    const auto& connectionToHiddeni = connectionMatrix.at(nodeNum);
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionToHiddeni.at(i) * nodePotentials.at(i);
  }
  else{//隠れ層の場合
    for(std::size_t i = 0; i < totalNodeNum; ++i){
      sum += connectionMatrix.at(i).at(nodeNum) * nodePotentials.at(i);
    }
  }
  sum += bias.at(layerNum).at(nodeNum);
  return sum;
}

template<typename T_traits>
typename RBM<T_traits>::potentialType
RBM<T_traits>::activate(std::size_t layerNum, std::size_t nodeNum){
  potentialType newpotential;
  if(layerNum == 0){
    newpotential = dist(randomNumberGenerator) + calculateInput(layerNum,nodeNum);
  }
  else if(layerNum == 1){
    double possibility = 1 / (1 + std::exp(-calculateInput(layerNum,nodeNum)));
    if(rand(randomNumberGenerator) <= possibility)
      newpotential = 1;
    else
      newpotential = 0;
  }
  return newpotential;
}

template<typename T_traits>
void RBM<T_traits>::timeEvolution(){
  for(int layerNum = 1; layerNum >= 0; --layerNum){
    auto& oneLayerPotential = potential.at(layerNum);
    for(std::size_t i = 0; i < totalNodeNum; ++i){
      oneLayerPotential.at(i) = activate(layerNum,i);
    }
  }
}

template<typename T_traits>//TODO calculateInputを使ったものに書き換える
vector<double>
RBM<T_traits>::calculateH(const vector<potentialType>& visibleLayer){
  vector<double> potentialH;
  potentialH.reserve(totalNodeNum);
  for(std::size_t nodeNum = 0; nodeNum < totalNodeNum; ++nodeNum)
    potentialH.push_back(1.0 / (1.0 + std::exp(calculateInput(1,nodeNum,visibleLayer))));
  return potentialH;
}

template<typename T_traits>
matrix<double>
RBM<T_traits>::calculateVH(const vector<potentialType>& visibleLayer){
  vector<double> hiddenLayer = calculateH(visibleLayer);
  matrix<double> potentialVH(totalNodeNum);
  auto itrPotVH = potentialVH.begin();
  auto itrPotVi = visibleLayer.begin();
  while(itrPotVH != potentialVH.end() && itrPotVi != visibleLayer.end()){
    *itrPotVH = *itrPotVi * hiddenLayer;
    ++itrPotVH;
    ++itrPotVi;
  }
  return potentialVH;
}

template<typename T_traits>
matrix<double>
RBM<T_traits>::batchDataMeanCalculateVH(const matrix<potentialType>& miniBatch){
  matrix<double> meanPotentialVH(totalNodeNum);
  for(std::size_t visibleNodeNum = 0; visibleNodeNum < totalNodeNum; ++visibleNodeNum)
    meanPotentialVH.at(visibleNodeNum).resize(totalNodeNum,0);
  for(std::size_t sampleNum = 0; sampleNum < miniBatch.size(); ++sampleNum)
    meanPotentialVH = meanPotentialVH + calculateVH(miniBatch.at(sampleNum));
  meanPotentialVH = meanPotentialVH / (double)miniBatch.size();
  return meanPotentialVH;
}

template<typename T_traits>
vector<double>
RBM<T_traits>::batchDataMeanCalculateV(const matrix<potentialType>& miniBatch){
  vector<double> meanPotentialV(totalNodeNum,0);
  for(auto itr = miniBatch.begin(); itr != miniBatch.end(); ++itr){
    meanPotentialV = meanPotentialV + *itr;
  }
  meanPotentialV = meanPotentialV / (double)miniBatch.size();
  return meanPotentialV;
}

template<typename T_traits>
vector<double>
RBM<T_traits>::batchDataMeanCalculateH(const matrix<potentialType>& miniBatch){
  vector<double> meanPotentialH(totalNodeNum,0);
  for(std::size_t sampleNum = 0; sampleNum < miniBatch.size(); ++sampleNum)
    meanPotentialH = meanPotentialH + calculateH(miniBatch.at(sampleNum));
  meanPotentialH = meanPotentialH / (double)miniBatch.size();
  return meanPotentialH;
}

template<typename T_traits>
void RBM<T_traits>::RBMstaticGenerate(int nodeNum, int randomSeed){
  totalNodeNum = nodeNum;
  int totalLayerNum = 2;
  randomNumberGenerator.seed(randomSeed);
  std::uniform_real_distribution<double> rand(0.0,1.0);
  bias.reserve(totalLayerNum);
  for(std::size_t i = 0; i < totalLayerNum; ++i){
    vector<biasType> tempBias;
    tempBias.reserve(totalNodeNum);
    for(std::size_t j = 0; j < totalNodeNum; ++j){
      tempBias.emplace_back(rand(randomNumberGenerator));
    }
    bias.emplace_back(tempBias);
  }
  connectionMatrix.reserve(totalNodeNum);
  for(std::size_t i = 0; i < totalNodeNum; ++i){
    vector<connectionType> tempConn;
    tempConn.reserve(totalNodeNum);
    for(std::size_t j = 0; j < totalNodeNum; ++j){
      tempConn.emplace_back(rand(randomNumberGenerator));
    }  
    connectionMatrix.emplace_back(tempConn);
  }
  std::cout << "RMBstaticGenerate compleated" << std::endl;
}

template<typename T_traits>
std::size_t RBM<T_traits>::totalNodeNum = 0;

template<typename T_traits>
vector<vector<typename T_traits::connectionType> > RBM<T_traits>::connectionMatrix;

template<typename T_traits>
vector<vector<typename T_traits::biasType>> RBM<T_traits>::bias;

template<typename T_traits>
std::mt19937 RBM<T_traits>::randomNumberGenerator;

#endif
