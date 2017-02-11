#include <iostream>
#include <fstream>

int main(){
  std::ofstream fout("input.dat");
  for(std::size_t j = 0; j < 10; ++j){
    for(std::size_t i = 0; i < 10; ++i){
      fout << 1.0 * i / 9.0 << ",";
    }
    fout << std::endl;
  }
  fout << std::endl;
  for(std::size_t j = 0; j < 10; ++j){
    for(std::size_t i = 0; i < 10; ++i){
      fout << 1.0 * j / 9.0 << ",";
    }
    fout << std::endl;
  }
  fout << std::endl;
  for(std::size_t j = 0; j < 10; ++j){
    for(std::size_t i = 0; i < 10; ++i){
      fout << 1.0 - (1.0 * i / 9.0) << ",";
    }
    fout << std::endl;
  }
  fout << std::endl;
  for(std::size_t j = 0; j < 10; ++j){
    for(std::size_t i = 0; i < 10; ++i){
      fout << 1.0 - (1.0 * j / 9.0) << ",";
    }
    fout << std::endl;
  }
  return 0;
}
