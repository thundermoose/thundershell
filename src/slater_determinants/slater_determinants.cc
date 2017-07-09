#include <cstdio>
#include "../utils/maths.hh"
#include "../debug/debug.h"
#include "slater_determinants.hh"


slatdet ground(int A){
  slatdet a = 0;
  for (int i = 0; i<A; i++){
    a<<=1;
    a|=1;
  }
  return a;
}

void Many_Particle_Basis::create_particles(slatdet s,int p,int F){
  if (p == 0){
    if (this->current_state == dimension){
      fprintf(stderr,"This should never happen, we are creating more many particle states than possible\n");
      exit(1);
    }
    DEBUG_MSG("Created state:%0.64lX\n",s);
    this->states[this->current_state++]=s;
    return;
  }
  DEBUG_MSG("fermilevel: %d\n",F);
  for (int cand = F; cand<sp_basis->get_dimension(); cand++){
    s^=((slatdet)1)<<cand;
    printf("created particle(%d): %d\n",p,cand);
    create_particles(s,p-1,cand+1);
    s^=((slatdet)1)<<cand;
  }
  
}

void Many_Particle_Basis::annihilate_particles(slatdet s,int h,int p,int F){
  
  if (h == 0){
    create_particles(s,p,this->A);
    return;
  }
  if (F == 0)
    return;
  DEBUG_MSG("fermilevel: %d\n");
  for (int cand = F-1; cand>=0; cand--){
    s^=((slatdet)1)<<cand;
    DEBUG_MSG("annihilated particle(%d): %d\n",h,cand);
    annihilate_particles(s,h-1,p,cand);
    s^=((slatdet)1)<<cand;
  }
}

  

Many_Particle_Basis::Many_Particle_Basis(Single_Particle_Basis* sp_basis,
					 int A){
  this->sp_basis = sp_basis;
  this->A = A;
  this->current_state = 1;
  dimension = noverk(sp_basis->get_dimension(),A);
  states = new slatdet[dimension];
  states[0]= ground(A);
  DEBUG_MSG("ground: %0.64lx\n",states[0]);
  for (int phex=1; phex<=A; phex++){
    slatdet next_state = states[0];
    printf("%dp-%dh:\n",phex,phex);
    annihilate_particles(next_state,phex,phex,A); 
  }
  DEBUG_MSG("dimension: %ld, current_state: %ld\n",dimension,current_state);
}
Many_Particle_Basis::~Many_Particle_Basis(){
  delete[] states;
}


void Many_Particle_Basis::list_states(){
  for (size_t i = 0; i<this->dimension; i++){
    char buffer[65];
    buffer[64]=0;
    slatdet ts =states[i];
    for (int j = 0; j<64; j++){
      buffer[63-j]=(ts&1)+'0';
      ts>>=1;
    }
    printf("(%ld) %s\n",i,buffer);
  }
}


void unit_test_slater_determinants(){
  {
    Single_Particle_Basis* spb = new Single_Particle_Basis();
    spb->set_num_shells(2);
    spb->set_dimension(4);
    shell s;
    s.n = 1;
    s.l = 0;
    s.j = 1;
    spb->add_shell(s);
    sp_state ss;
    ss.s = 0;
    ss.m = -1;
    spb->add_sp_state(ss);
    ss.s = 0;
    ss.m = 1;
    spb->add_sp_state(ss);
    s.n = 2;
    s.l = 0;
    s.j = 1;
    spb->add_shell(s);
    ss.s = 1;
    ss.m = -1;
    spb->add_sp_state(ss);
    ss.s = 1;
    ss.m = 1;
    spb->add_sp_state(ss);

    Many_Particle_Basis *mpb = new Many_Particle_Basis(spb,2);
    printf("Lists all possible slater determinants for a 4 sp state 2 particle system\n");
    mpb->list_states();
    delete mpb;
    delete spb;
  }
}
