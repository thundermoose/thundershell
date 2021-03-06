#include <cstdlib>
#include <cstdio>
#include "../shell_reader/shell_reader.hh"
#include "../slater_determinants/slater_determinants.hh"
#include "../second_quantization/second_quantization.hh"
#include "../utils/maths.hh"
using namespace std;

void usage(char* prg){
  printf("This program is under construction please wait\n");
}

int main(int argc, char** argv){
#ifdef DEBUG
  printf("This is a unit test\n");
  unit_tests_maths();
  unit_tests_shell_reader();
  unit_test_slater_determinants();
  unit_test_second_quantization();
#endif
  usage(*argv);
  return 0;
}
