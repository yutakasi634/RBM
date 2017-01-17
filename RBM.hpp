//オブジェクトを生成する前にrandomNumbeGenerate()を使用しておく
#ifndef RBM
#define RBM
#include <random>
#include <cmath>

template<typename T_traits>
class RBM{
protected:
  std::vector<std::vector<T_traits::potentialType>> potential;
public:
  static std::size_t totalNodeNum;
  static std::vector<std::vector<T_traits::connectionType>> connectionMatrix;
  //connectionMatrix[i][j]はvisible layerのi番目nodeとhidden layerのj番目の接続の重み
  static std::vector<std::vector<T_traits::biasType>> bias;
  static std::mt19937 randomNumberGenerator;
  RBM(std::vector<T_traits::potentialType> initialValues,int randomSeed = 0);
  void setVisiblePotentials(std::vector<T_traits::potentialType>& values);
  void setBias(std::vector<std::vector<T_traits::biasType>>& values);
  void setConnectionMatrix(std::vector<std::vector<T_traits::connectionType>>& weightMatrix);
  double activationFunction(std::size_t layerNum, std::size_t nodeNum);
  static double activationFunction(std::size_t layerNum, std::size_t nodeNum, std::vector<T_traits::potentialType> nodePotentials);
  T_traits::potentialType activate(std::size_t layerNum, std::size_t nodeNum);
  void timeEvolution();
  static std::vector<std::vector<double>>& batchDataMeanCalculateVH(std::vector<BBRBMTypeTraits::potentialType>& miniBatch);
  static std::vector<double>& batchDataMeanCalculateV(std::vector<BBRBMTypeTraits::potentialType>& minibatch);
  static std::vector<double>& batchDataMeanCalculateH(std::vector<BBRBMTypeTraits::potentialType>& minibatch);
  static void RBMstaticGenerate(int randomSeed);
};

template<typename T_traits>
RBM<T_traits>::RBM(std::vector<T_traits::potentialType>& initialValues){
  totalNodeNum = initialValue.size();
  potential.push_back(initialValue);
  std::vector<T_traits::potentialType> hiddenLayer(totalNodeNum);
  potential.emplace_back(hiddenLayer);
}

template<typename T_traits>
void RBM<T_traits>::setVisiblePotentials(std::vector<T_traits::potentialType>& values){
  potential.at(0) = values;
}

template<typename T_traits>
void RBM<T_traits>::setConnectionMatrix(std::vector<std::vector<T_traits::connectionType>>& weightMatrix){
  connectionMatrix = weightMatrix;
}

template<typename T_traits>
double activationFunction(std::size_t layerNum, std::size_t nodeNum){
  std::size_t inputLayer = ++layerNum%2;
  double sum = 0.0;
  if(inputLayer == 0){  
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(i).at(nodeNum)*potential.at(layerNum).at(nodeNum);
  }
  else{
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(nodeNum).at(i)*potential.at(layerNum).at(nodeNum);
  }
  sum += bias.at(layerNum).at(nodeNum);
  double possibility = 1.0/(1 + std::exp(-sum));
  return possibility;
}

template<typename T_traits>
double activationFunction(std::size_t layerNum, std::size_t nodeNum, std::vector<T_traits::potentialType> nodePotentials){
  std::size_t inputLayer = ++layerNum%2;
  double sum = 0.0;
  if(inputLayer == 0){  
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(i).at(nodeNum)*nodePotentials.at(i);
  }
  else{
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(nodeNum).at(i)*nodePotentials.at(i);
  }
  sum += bias.at(layerNum).at(nodeNum);
  double possibility = 1.0/(1 + std::exp(-sum));
  return possibility;
}

template<typename T_traits>
T_traits::potentialType activate(std::size_t layerNum, std::size_t nodeNum){
  std::uniform_real_distribution rand(0.0,1.0);
  double possibility = activationFunction(layerNum,nodeNum);
  T_traits::potentialType potential;
  if(rand(randomNumberGenerator) <= possibility)
    potential = 1;
  else
    potential = 0;

  return potentialType;
}

template<typename T_traits>
void RBM<T_traits>::timeEvolution(){
  for(std::size_t i = 0; i < potential.at(1).size(); ++i)
    potential.at(1).at(i) = activationFunction(1,i);
  for(std::size_t i = 0; i < potential.at(0).size(); ++i)
    potential.at(0).at(i) = activationFunction(0,i);
}

template<typename T_traits>
std::vector<std::vector<double>>& RBM<T_traits>::batchDataMeanCalculateVH(std::vector<std::vector<T_traits::potentialType>>& miniBatch){
  std::vector<std::vector<double>> meanPotentialVH(miniBatch.at(0).size());
  for(std::size_t visibleNodeNum = 0; visibleNodeNum < meanPotentialVH.size(); ++visibleNodeNum){
    meanPotentialVH.at(visibleNodeNum).reserve(meanPotentialVH.size());
    for(std::size_t hiddenNodeNum = 0; hiddenNodeNum < meanPotentialVH.size(); ++hiddenNodeNum){
      double sum = 0;
      for(std::size_t sampleNum = 0; sampleNum < miniBatch.size(); ++sampleNum)
	sum += miniBatch.at(sampleNum).at(visibleNodeNum)*activationFunction(1,hiddenNodeNum,miniBatch).at(sampleNum);
      meanPotentialVH.at(visibleNodeNum).at(hiddenNodeNum) = sum/miniBatch.size();
    }
  }
  return meanPotentialVH;
}

template<typename T_traits>
std::vector<double>& RBM<T_traits>::batchDataMeanCalculateV(std::vector<std::vector<T_traits>>& miniBatch){
  std::vector<double> meanPotentialV(miniBatch.at(0).size());
  for(std::size_t nodeNum; nodeNum < miniBatch.at(0).size(); ++nodeNum){
    double sum;
    for(std::size_t sampleNum; sampleNum < miniBatch.size(); ++sampleNum)
      sum += miniBatch.at(sampleNum).at(nodeNum);
    meanPotentialV.at(nodeNum) = sum/miniBatch.size();
  }
  return meanPotentialV;
}

template<typename T_traits>
std::vector<double>& RBM<T_traits>::batchDataMeanCalculateH(std::vector<std::vector<T_traits>> miniBatch){
  std::vector<double> meanPotentialH(miniBatch.at(0).size());
  for(std::size_t nodeNum; nodeNum < miniBatch.at(0).size(); ++nodeNum){
    double sum;
    for(std::size_t sampleNum; sampleNum < miniBatch.size(); ++sampleNum)
      sum += activationFunction(1,nodeNum,miniBatch.at(sampleNum));
    meanPotentialH.at(nodeNum) = sum/miniBatch.size();
  }
  return meanPotentialH;
}

template<typename T_traits>
void RBM<T_traits>::RBMstaticGenerate(int randomSeed){
  int totalLayerNum = 2;
  randomNumberGenerator.seed(randomSeed);
  for(std::size_t i = 0; i < totalLayerNum; ++i)
    bias.at(i).resize(totalNodeNum,0);
  connectionMatrix.resize(totalNodeNum);
  for(std::size_t i = 0; i < totalNodeNum; ++i)
    connectionMatrix.at(i).resize(totalNodeNum,1);
}

#endif
