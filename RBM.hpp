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
  activationFunction(std::size_t layerNum, std::size_t nodeNum);
  
  static
  double
  activationFunction(std::size_t layerNum, std::size_t nodeNum,
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
double RBM<T_traits>::activationFunction(std::size_t layerNum, std::size_t nodeNum){
  std::size_t inputLayerNum = (layerNum + 1)%2;
  double sum = 0.0;
  const auto& connectionNodes = connectionMatrix.at(nodeNum);
  const auto& potentialNodes = potential.at(inputLayerNum);
  if(layerNum == 0){  
    for(std::size_t i = 0; i < totalNodeNum; ++i){
      sum += connectionNodes.at(i) * potentialNodes.at(i);
    }
  }
  else{
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(i).at(nodeNum) * potentialNodes.at(i);
  }
  sum += bias.at(layerNum).at(nodeNum);
  double possibility = 1.0/(1 + std::exp(-sum));
  return possibility;
}

template<typename T_traits>
double RBM<T_traits>::activationFunction(std::size_t layerNum, std::size_t nodeNum,
					 const vector<potentialType>& nodePotentials){
  double sum = 0.0;
  const auto& connectionNodes = connectionMatrix.at(nodeNum);
  if(layerNum == 0){  
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionNodes.at(i) * nodePotentials.at(i);
  }
  else{
    for(std::size_t i = 0; i < totalNodeNum; ++i){
      sum += connectionMatrix.at(i).at(nodeNum) * nodePotentials.at(i);
    }
  }
  sum += bias.at(layerNum).at(nodeNum);
  double possibility = 1.0/(1 + std::exp(-sum));
  return possibility;
}

template<typename T_traits>
typename RBM<T_traits>::potentialType
RBM<T_traits>::activate(std::size_t layerNum, std::size_t nodeNum){
  std::uniform_real_distribution<double> rand(0.0,1.0);
  double possibility = activationFunction(layerNum,nodeNum);
  potentialType potential;
  if(rand(randomNumberGenerator) <= possibility)
    potential = 1;
  else
    potential = 0;
  
  return potential;
}

//TODO activationFunction ->activate
template<typename T_traits>
void RBM<T_traits>::timeEvolution(){
  for(int layerNum = 1; layerNum >= 0; --layerNum){
    auto& oneLayerPotential = potential.at(layerNum);
    for(std::size_t i = 0; i < totalNodeNum; ++i){
      oneLayerPotential.at(i) = activationFunction(layerNum,i);
    }
  }
}

template<typename T_traits>
vector<double>
RBM<T_traits>::calculateH(const vector<potentialType>& visibleLayer){
  vector<double> potentialH;
  potentialH.reserve(totalNodeNum);
  for(std::size_t nodeNum = 0; nodeNum < totalNodeNum; ++nodeNum)
    potentialH.push_back(activationFunction(1,nodeNum,visibleLayer));
  return potentialH;
}

template<typename T_traits>
matrix<double>
RBM<T_traits>::calculateVH(const vector<potentialType>& visibleLayer){
  vector<double> hiddenLayer = calculateH(visibleLayer);
  matrix<double> potentialVH(totalNodeNum);
  for(std::size_t visibleNodeNum = 0; visibleNodeNum < totalNodeNum; ++visibleNodeNum){
    potentialVH.at(visibleNodeNum).reserve(totalNodeNum);
    auto& visibleNodePotentialVH = potentialVH.at(visibleNodeNum);
    const auto& visibleNode = visibleLayer.at(visibleNodeNum);
    for(std::size_t hiddenNodeNum = 0; hiddenNodeNum < totalNodeNum; ++hiddenNodeNum){
      visibleNodePotentialVH.push_back(visibleNode * hiddenLayer.at(hiddenNodeNum));
    }
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
  vector<double> meanPotentialV(totalNodeNum);
  for(std::size_t nodeNum = 0; nodeNum < totalNodeNum; ++nodeNum){
    double sum;
    for(std::size_t sampleNum = 0; sampleNum < miniBatch.size(); ++sampleNum)
      sum += miniBatch.at(sampleNum).at(nodeNum);
    meanPotentialV.at(nodeNum) = sum/miniBatch.size();
  }
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
  bias.resize(totalLayerNum);
  for(std::size_t i = 0; i < totalLayerNum; ++i)
    bias.at(i).resize(totalNodeNum,0);
  connectionMatrix.resize(totalNodeNum);
  for(std::size_t i = 0; i < totalNodeNum; ++i)
    connectionMatrix.at(i).resize(totalNodeNum,1);
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
