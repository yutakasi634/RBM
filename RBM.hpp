#ifndef RBM
#define RBM
#include <random>
#include <cmath>

template<typename T_traits>
class RBM{
protected:
  std::size_t totalNodeNum;
  std::vector<std::vector<T_traits::potentialType>> potential;
  std::vector<std::vector<T_traits::connectionType>> connectionMatrix;
  //connectionMatrix[i][j]はvisible layerのi番目nodeとhidden layerのj番目の接続の重み
  std::vector<std::vector<T_traits::biasType>> bias;
public:
  RBM(std::vector<T_traits::potentialType> initialValues);
  void setVisiblePotentials(std::vector<T_traits::potentialType>& values);
  void setBias(std::vector<std::vector<T_traits::biasType>>& values);
  void setConnectionMatrix(std::vector<std::vector<T_traits::connectionType>>& weightMatrix);
  T_traits::potentialType activationFunction(std::size_t nodeNum);
  void timeEvolution();
};

template<typename T_traits>
RBM<T_traits>::RBM(std::vector<T_traits::potentialType>& initialValues){
  totalNodeNum = initialValue.size();
  potential.emplace_back(initialValue);
  std::vector<T_traits::potentialType> hiddenLayer(totalNodeNum);
  potential.emplace_back(hiddenLayer);
  for(std::size_t i = 0; i < potential.size(); ++i)
    bias.at(i).resize(totalNodeNum,0);
  connectionMatrix.resize(totalNodeNum);
  for(std::size_t i = 0; i < totalNodeNum; ++i){
    connectionMatrix.at(i).resize(totalNodeNum);
    for(std::size_t j = 0; j < totalNodeNum; ++j){
      connectionMatrix.at(j) = 1;
    }
  }
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
T_traits::potentialType activationFunction(std::size_t layerNum, std::size_t nodeNum){
  std::size_t inputLayer = ++layerNum%2;
  double sum = 0.0;
  if(inputLayer == 0){  
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(i).at(nodeNum);
  }
  else{
    for(std::size_t i = 0; i < totalNodeNum; ++i)
      sum += connectionMatrix.at(nodeNum).at(i);
  }
  sum += bias.at(layerNum).at(nodeNum);
  double possibility = 1.0/(1 + std::exp(-sum));
  
}

template<typename T_traits>
void RBM<T_traits>::timeEvolution(){
  
}

#endif
