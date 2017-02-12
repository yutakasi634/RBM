#ifndef GETDATAVECTOR
#define GETDATAVECTOR
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

template<typename potentialType>
std::vector<std::vector<potentialType>> getDataVector(std::string filename){
  std::cout << filename << std::endl;
  std::ifstream reading_file(filename);
  if(!reading_file){
    std::cout << filename << "が開けません" << std::endl;    
  }
  std::string reading_line_buffer;
  std::cout << "reading " << filename << "…" << std::endl;
  potentialType num;
  char comma;

  std::vector<std::vector<potentialType>> data;
  while(getline(reading_file,reading_line_buffer)){
    std::istringstream is(reading_line_buffer);
    std::vector<potentialType> temp_data;
    std::string token;
    while(getline(is,token,',')){
      std::istringstream numtoken(token);
      numtoken >> num;
      temp_data.push_back(num);
    }
    data.push_back(temp_data);
    reading_file.peek();
  }
  reading_file.close();
  return data;
}

#endif
