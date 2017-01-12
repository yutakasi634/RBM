#ifndef GETDATAVECTOR
#define GETDATAVECTOR
#include <iostream>
#include <fstream>

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

#endif
