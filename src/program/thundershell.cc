#include <cstdlib>
#include <cstdio>

using namespace std;

void usage(char* prg){
  printf("This program is under construction please wait\n");
}

int main(int argc, char** argv){
#ifdef DEBUG
  printf("This is a unit test\n");
  // call unit tests from here
#endif
  usage(*argv);
  return 0;
}
