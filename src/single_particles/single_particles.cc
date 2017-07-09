#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "single_particles.hh"
#include "../debug/debug.h"

Single_Particle_Basis::Single_Particle_Basis(){
  this->shells = NULL;
  this->num_shells = 0;
  this->current_shell = 0;
  this->states = NULL;
  this->dimension = 0;
  this->current_state = 0;
}

Single_Particle_Basis::~Single_Particle_Basis(){
  if (this->shells != NULL)
    delete[] this->shells;
  if (this->states != NULL)
    delete[] this->states;
}

void Single_Particle_Basis::set_num_shells(int num_shells){
  if (this->shells != NULL){
    shell *tmp = new shell[num_shells];
    memcpy(tmp,this->shells,this->num_shells*sizeof(shell));
    delete[] this->shells;
    this->shells = tmp;
  }else{
    this->shells = new shell[num_shells];
  }
  this->num_shells = num_shells;
}

void Single_Particle_Basis::set_dimension(int dimension){
  if (this->states != NULL){
    DEBUG_MSG("this->states = %p\n",this->states);
    sp_state *tmp = new sp_state[dimension];
    DEBUG_MSG("tmp = %p\n",tmp);
    memcpy(tmp,this->states,this->dimension*sizeof(sp_state));
    DEBUG_MSG("This has been done\n");
    delete[] this->states;
    this->states = tmp;
  }else{
    this->states = new sp_state[dimension];
    DEBUG_MSG("this->states = %p\n",this->states);
  }
  this->dimension = dimension;
}

void Single_Particle_Basis::add_shell(shell s){
#ifdef DEBUG
  static int counter = 0;
  counter++;
#endif
  if (this->current_shell == this->num_shells){
    fprintf(stderr,"Can not add more shells\n");
    fprintf(stderr,"The number of shells are %d, current_shell is %d\n",
	    this->num_shells,this->current_shell);
    DEBUG_MSG("This function has been called %d times\n",counter);
    exit(1);
  }
  this->shells[this->current_shell++] = s;
}

void Single_Particle_Basis::add_sp_state(sp_state s){
  if (this->current_state == this->dimension){
    fprintf(stderr,"Can not add more states\n");
    exit(1);
  }
  this->states[this->current_state++] = s;
}  

int Single_Particle_Basis::get_dimension(){
  return dimension;
}

int Single_Particle_Basis::get_num_shells(){
  return num_shells;
}

shell Single_Particle_Basis::get_shell(int i){
  return shells[i];
}

sp_state Single_Particle_Basis::get_sp_state(int i){
  return states[i];
}
