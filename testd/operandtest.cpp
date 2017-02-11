#include <iostream>
#include "../vectorWrapper.hpp"

int main(){
  matrix<double> a{{0,1},{2,3}};
  matrix<double> b{{2,3},{4,5}};
  vector<double> d{3,4};
  auto c = a * d;
  std::cout << c << std::endl;
}
