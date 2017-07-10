#include "maths.hh"
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

size_t noverk(size_t n, size_t k){
  double acc = 0;
  for (size_t i = n-k+1; i<=n; i++){
    acc += log((double)i);
  }
  for (size_t i = 1; i<=k; i++){
    acc -= log((double)i);
  }
  return (size_t)(exp(acc)+0.5);
}


void unit_tests_maths(){
  size_t res = noverk(8,4);
  if (res != 70){
    fprintf(stderr,"maths unit test failed\n"
	    "noverk(8,4) = %ld but should be 70\n",
	    res);
    exit(1);
  }

}
