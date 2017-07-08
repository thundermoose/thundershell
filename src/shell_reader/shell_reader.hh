#ifndef __SHELL_READER__
#define __SHELL_READER__
#include <string>
#include <cstdio>

using namespace std;

/* Reads a .sp file and use it to 
 * set up the single particle basis
 */

class Shell_Reader{
private:
  enum ipn {UK,ISO,PN};
  ipn formalism;
  int a,z;
  int num_shells;
  
  bool parse_row(char* row);
public:
  Shell_Reader(string filename);
  ~Shell_Reader();
};

void unit_tests_shell_reader();

#endif
