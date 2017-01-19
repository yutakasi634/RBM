#ifndef GETDATAVECTOR
#define GETDATAVECTOR
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

template<typename potentialType>
std::vector<std::vector<potentialType>> getDataVector(std::string filename){
  std::ifstream reading_file;
  reading_file.open(filename,std::ios::in);
  std::string reading_line_buffer;
  std::cout << "reading" << filename << "…" << std::endl;
  potentialType num;
  char comma;

  std::vector<std::vector<potentialType>> data;
  while(!reading_file.eof()){
    std::vector<potentialType> temp_data;
    getline(reading_file,reading_line_buffer);
    std::istringstream is(reading_line_buffer);
    while(!is.eof()){
      is >> num >> comma;
      temp_data.push_back(num);
    }
    data.push_back(temp_data);
    reading_file.peek();
  }
  return data;
}

#endif
