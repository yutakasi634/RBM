#include <iostream>
#include <fstream>
#include <random>

template<typename T>
vector<vector<T>>& getDataVector(std::string filename){
  std::ifstream reading_file;
  reading_file.open(filename,std::ios::in);
  std::string reading_line_buffer;
  std::cout << "reading" << filename << "…" << std::endl;
  T num;
  char comma;

  while(!reading_file.eof()){
    std::vector<T> temp_data;
    getline(reading_file,reading_line_buffer);
    std::istringstream is(reading_line_buffer);
    while(!is.eof()){
      is >> num >> comma;
      temp_data.push_back(num);
    }
    std::vector<vector<T>> data;
    data.push_back(temp_data);
  }
  return data;
}

template<typename T>
class RBM{
protected:
  std::vector<std::vector<T>> potential;
  std::vector<std::vector<double>> connectionMatrix;
public:
  RBM(std::vector<T> initialValues);
  void setVisiblePotentials(std::vector<T>& values);
  void setConnectionMatrix(std::vector<std::vector<double>>& weightMatrix);
};

template<typename T>
RBM<T>::RBM(std::vector<T> initialValues){
  potential.push_back(initialValue);
  std::vector<T> hiddenLayer(initialValue.size());
  potential.push_back(hiddenLayer);
  connectionMatrix.resize(initialValue.size());
  for(std::size_t i = 0; i < initialValue.size(); ++i){
    connectionMatrix.at(i).resize(initialValue.size());
    for(std::size_t j = 0; j < initialValue.size(); ++j){
      connectionMatrix.at(j) = 1;
    }
  }
}

template<typename T>
void RBM<T>::setVisiblePotentials(std::vector<T>& values){
  potential.at(0) = values;
}

template<typename T>
void RBM<T>::setConnectionMatrix(std::vector<std::vector<double>>& weightMatrix){
  connectionMatrix = weightMatrix;
}

int main(){
  std::vector<vector<int>> sample = getDataVector(data.csv); //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  return 0;
}
