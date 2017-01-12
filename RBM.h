#ifndef RBM
#define RBM
#include <random>

template<typename potentialType,typename connectionType>
class RBM{
protected:
  std::vector<std::vector<potentialType>> potential;
  std::vector<std::vector<connectionType>> connectionMatrix;
public:
  RBM(std::vector<potentialType> initialValues);
  void setVisiblePotentials(std::vector<potentialType>& values);
  void setConnectionMatrix(std::vector<std::vector<connectionType>>& weightMatrix);
};

template<typename potentialType, typename connectionType>
RBM<potentialType,connectionType>::RBM(std::vector<potentialType>& initialValues){
  potential.push_back(initialValue);
  std::vector<potentialType> hiddenLayer(initialValue.size());
  potential.push_back(hiddenLayer);
  connectionMatrix.resize(initialValue.size());
  for(std::size_t i = 0; i < initialValue.size(); ++i){
    connectionMatrix.at(i).resize(initialValue.size());
    for(std::size_t j = 0; j < initialValue.size(); ++j){
      connectionMatrix.at(j) = 1;
    }
  }
}

template<typename potentialType, typename connectionType>
void RBM<potentialType,connectionType>::setVisiblePotentials(std::vector<potentialType>& values){
  potential.at(0) = values;
}

template<typename potentialType, typename connectionType>
void RBM<potentialType>::setConnectionMatrix(std::vector<std::vector<connectionType>>& weightMatrix){
  connectionMatrix = weightMatrix;
}

#endif
