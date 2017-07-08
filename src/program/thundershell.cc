#include <cstdlib>
#include <cstdio>
#include "../shell_reader/shell_reader.hh"

using namespace std;

void usage(char* prg){
  printf("This program is under construction please wait\n");
}

int main(int argc, char** argv){
#ifdef DEBUG
  printf("This is a unit test\n");
  unit_tests_shell_reader();
#endif
  usage(*argv);
  return 0;
}
