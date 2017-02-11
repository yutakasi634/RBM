template<typename T1, int T2 = 0>
int example(T1 x);

template<typename T1, int T2>
int example(T1 x){
  return x;
}


int main(){
  example(2);
}
