#include <stdio.h>
#include <stdlib.h>
#include "shell_reader.hh"
#include "../debug/debug.h"


Shell_Reader::Shell_Reader(string filename){
  this->sp_basis = new Single_Particle_Basis();
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
  this->current_shell = 0;
  this->parse_mode = false;
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
    sp_basis->set_num_shells(num_shells);
  }else if (!this->parse_mode){
    this->parse_mode = true;
  }else if (this->current_shell<sp_basis->get_num_shells()){
    shell s;
    sscanf(row,"%*d %d %d %d",
	   &s.n,&s.l,&s.j);
    
    sp_basis->add_shell(s);
    int dim = sp_basis->get_dimension();
    dim+=s.j+1;
    sp_basis->set_dimension(dim);
    sp_state sps;
    sps.s = current_shell++;
    for (sps.m = -s.j; sps.m<=s.j; sps.m+=2){
      sp_basis->add_sp_state(sps);
    }
  }else{
    return false;
  }

  return true;
}

Single_Particle_Basis* Shell_Reader::get_basis(){
  return sp_basis;
}



void unit_tests_shell_reader(){
  string sd_shells_filename = "/home/djarv/Dokument/ECTstar_Courses/Course_5/Programs/nushellx/sps/sd.sp";
  Shell_Reader* shell_reader = new Shell_Reader(sd_shells_filename);

  Single_Particle_Basis* sp_basis = shell_reader->get_basis();
  printf("Printing all the single particle states that should have been produced\n");
  int i;
  for (i = 0; i<sp_basis->get_dimension(); i++){
    sp_state sps = sp_basis->get_sp_state(i);
    shell sh = sp_basis->get_shell(sps.s);
    printf("(%d): %d %d %d %d\n",
	   i,sh.n,sh.l,
	   sh.j,sps.m);
  }
  
  delete shell_reader;
}
