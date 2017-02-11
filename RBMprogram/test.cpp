#include "vectorWrapper.hpp"
#include <iostream>

using std::cout;
using std::endl;


int main(){
  vector<double> a = {1.0,2.5,3.5};
  vector<double> b = {2.0,4.3,2.4};
  double c(5.0);
  cout << "a " << a << endl << "b " << b << endl << "c " << c << endl;
  cout << "a + b = " << a + b << endl
       << "a - b = " << a - b << endl
       << "a / b = " << a / b << endl
       << "a * b = " << a * b << endl
    //<< "a + c = " << a + c << endl
    //<< "a - c = " << a - c << endl
       << "a / c = " << a / c << endl
       << "a * c = " << a * c << endl
    //<< "c + a = " << c + a << endl
    //<< "c - a = " << c - a << endl
       << "c / a = " << c / a << endl
       << "c * a = " << c * a << endl;
  
    }
