#include <iostream>
#include <fstream>
#include <random>

template<typename potentialType>
vector<vector<potentialType>>& getDataVector(std::string filename){
  std::ifstream reading_file;
  reading_file.open(filename,std::ios::in);
  std::string reading_line_buffer;
  std::cout << "reading" << filename << "…" << std::endl;
  T num;
  char comma;

  while(!reading_file.eof()){
    std::vector<potentialType> temp_data;
    getline(reading_file,reading_line_buffer);
    std::istringstream is(reading_line_buffer);
    while(!is.eof()){
      is >> num >> comma;
      temp_data.push_back(num);
    }
    std::vector<vector<potentialType>> data;
    data.push_back(temp_data);
  }
  return data;
}

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

int main(){
  std::vector<vector<int>> sample = getDataVector(data.csv); //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  return 0;
}
