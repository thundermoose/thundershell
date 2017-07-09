#include "maths.hh"
#include <cmath>

using namespace std;

size_t noverk(size_t n, size_t k){
  double acc = 0;
  for (size_t i = n-k+1; i<=n; i++){
    acc += log((double)i);
  }
  for (size_t i = 1; i<=k; i++){
    acc -= log((double)i);
  }
  return (size_t)exp(acc);
}
