#include <cstdlib>
#include <cstdio>
#include "single_particles.hh"

Single_Particles_Basis::Single_Particles_Basis(){
  this->shells = NULL;
  this->num_shells = 0;
  this->current_shell = 0;
  this->states = NULL;
  this->dimension = 0;
}


void Single_Particles_Basis::set_num_shells(int num_shells){
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
    shell *tmp = new shell[dimension];
    memcpy(tmp,this->states,this->dimension*sizeof(shell));
    delete[] this->states;
    this->states = tmp;
  }else{
    this->states = new shell[dimension];
  }
  this->dimension = dimension;
}

void Single_Particle_Basis::add_shell(shell s){
  if (this->current_shell == this->num_shells){
    fprintf(stderr,"Can not add more shells\n");
    exit(1);
  }
  this->shells[this->current_shell++] = s;
}

void Single_Particle_Basis::add_sp_state(sp_state s){
  if (this->current_state == this->num_states){
    fprintf(stderr,"Can not add more states\n");
    exit(1);
  }
  this->states[this->current_state++] = s;
}  

shell Single_Particle_Basis::get_shell(int i){
  return shells[i];
}

sp_state Single_Particle_Basis::get_sp_state(int i){
  return states[i];
}
