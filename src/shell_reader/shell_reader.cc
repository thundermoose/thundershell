#include <stdio.h>
#include <stdlib.h>
#include "shell_reader.hh"
#include "../debug/debug.h"


Shell_Reader::Shell_Reader(string filename){
  FILE* infile = fopen(filename.c_str(),"r");
  if (infile == NULL){
    fprintf(stderr,"Could not open file %s\n",filename.c_str());
    exit(1);
  }
  // Seting up all initial values
  this->formalism = UK;
  this->a = -1;
  this->z = -1;
  this->num_shells=-1;
  char* current_row = NULL; // a good initial value for getline
  size_t n = 0;
  
  // Reading the file row by row and parsing them
  DEBUG_MSG("Starting to read file %s\n",filename.c_str());
  while (!feof(infile)){
    DEBUG_MSG("The program should come this far\n");
    ssize_t res;
    if ((res = getline(&current_row,&n,infile))<0){
      DEBUG_MSG("Reached end of file (%ld)\n",res);
      break; // This will happen if the file ends without \n
    }
    DEBUG_MSG("read row %s",current_row);
    // the file can contain comments, these should be ignored
    if (current_row[0] == '!'){
      continue;
    }
    if (!parse_row(current_row)){
      break;
    }
  }
  DEBUG_MSG("finished reading\n");
  fclose(infile);
}

Shell_Reader::~Shell_Reader(){
  DEBUG_MSG("In the destructor\n");
}

bool Shell_Reader::parse_row(char* row){
  DEBUG_MSG("parsing: %s\n",row);
  if (formalism == UK){
    if (row[0] == 't')
      formalism = ISO;
    else if(row[0]=='p' && row[0]=='n')
      formalism = PN;
    else{
      fprintf(stderr,"Unknown formalism in .sp file: %s\n",row);
      exit(1);
    }
    DEBUG_MSG("Formalism determined\n");
  }else if (a<0 && z<0)
    sscanf(row,"%d %d",&a,&z);
  else if (num_shells<0){
    sscanf(row,"%d",&num_shells);

  }

  return true;
}



void unit_tests_shell_reader(){
  string sd_shells_filename = "/home/djarv/Dokument/ECTstar_Courses/Course_5/Programs/nushellx/sps/sd.sp";
  Shell_Reader* shell_reader = new Shell_Reader(sd_shells_filename);

  delete shell_reader;
}
