#include <cstdio>
#include "../utils/maths.hh"
#include "../debug/debug.h"
#include "slater_determinants.hh"
#include <string.h>


slatdet ground(int A){
  slatdet a = 0;
  for (int i = 0; i<A; i++){
    a<<=1;
    a|=1;
  }
  return a;
}
#define HASHCONST 0x0123456789ABCDEF
uint64_t hash_func(slatdet s){
  return s^(s<<16)^(s<<32)^HASHCONST;
}

void Many_Particle_Basis::add_to_hash(slatdet s, size_t i){
  uint64_t hash = hash_func(s);
  while (hash_array[hash%this->hash_size].i>-1){
    hash++;
    DEBUG_MSG("hash = 0x%lx\n",hash);
  }
  hash_array[hash%this->hash_size].key = s;
  hash_array[hash%this->hash_size].i = i;
}

void Many_Particle_Basis::create_particles(slatdet s,int p,int F){
  if (p == 0){
    if (this->current_state == dimension){
      fprintf(stderr,"This should never happen, we are creating more many particle states than possible\n");
      exit(1);
    }
    DEBUG_MSG("Created state:%0.64lx\n",s);
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
  this->dimension = noverk(sp_basis->get_dimension(),A);
  DEBUG_MSG("dim: %ld, (%d %d)\n",dimension,sp_basis->get_dimension(),A);
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

void Many_Particle_Basis::set_up_hash(){
  this->hash_size = this->dimension*2;
  this->hash_array = new hash_element[this->hash_size];
  for (size_t i=0; i<this->hash_size; i++){
    this->hash_array[i].i = -1;
    this->hash_array[i].key = 0;
  }
  for (size_t i = 0; i<dimension; i++){
    add_to_hash(states[i],i);
  }
  
}

int Many_Particle_Basis::get_M(slatdet s){
  int M = 0;
  for (int i = 0; i<sp_basis->get_dimension(); i++){
    if (s&1){
      M+=sp_basis->get_sp_state(i).m;
    }
    s>>=1;
  }
  return M;
}

Many_Particle_Basis::Many_Particle_Basis(Single_Particle_Basis* sp_basis,
					 int A,int M) :
  Many_Particle_Basis(sp_basis,A){
  slatdet* wanted_states = new slatdet[dimension];
  size_t wanted_dimension = 0;
  for (size_t i = 0; i<dimension; i++){
    if (get_M(states[i])==M){
      wanted_states[wanted_dimension++]=states[i];
    }
  }

  delete[] states;
  states = new slatdet[wanted_dimension];
  memcpy(states,wanted_states,sizeof(slatdet)*wanted_dimension);
  delete[] wanted_states;
  dimension = wanted_dimension;
}

Many_Particle_Basis::~Many_Particle_Basis(){
  delete[] states;
}


inline
bool is_pair_exciation(slatdet s){
  slatdet mask = 0xAAAAAAAAAAAAAAAA;
  return (((s&mask)>>1)^(s&(mask>>1))) == 0;
}

void Many_Particle_Basis::keep_pair_excitations_only(){
  slatdet* wanted_states = new slatdet[dimension];
  size_t wanted_dimension = 0;
  for (size_t i = 0; i<dimension; i++){
    if (is_pair_exciation(states[i])){
      wanted_states[wanted_dimension++]=states[i];
    }
  }

  delete[] states;
  states = new slatdet[wanted_dimension];
  memcpy(states,wanted_states,sizeof(slatdet)*wanted_dimension);
  delete[] wanted_states;
  dimension = wanted_dimension;
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

size_t Many_Particle_Basis::get_dimension(){
  return this->dimension;
}

size_t Many_Particle_Basis::get_sp_dimension(){
  return this->sp_basis->get_dimension();
}

slatdet Many_Particle_Basis::get_state(size_t i){
  return states[i];
}

ssize_t Many_Particle_Basis::get_index(slatdet s){
  uint64_t hash = hash_func(s);
  while (this->hash_array[hash%this->hash_size].key != s &&
	 this->hash_array[hash%this->hash_size].i != -1){
    hash++;
  }
  return this->hash_array[hash%this->hash_size].i;
}

void unit_test_slater_determinants(){
  { // easy test
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

    // comparing to what I expect
    slatdet expected[6] = {0x0000000000000003, // 0011
			   0x0000000000000005, // 0101
			   0x0000000000000009, // 1001
			   0x0000000000000006, // 0110
			   0x000000000000000a, // 1010
			   0x000000000000000c};
    if (mpb->get_dimension() != 6){
      fprintf(stderr,"slater determinant unit test 1 faild,\n"
	      "wrong dimension %ld, should be 6\n", mpb->get_dimension());
      exit(1);
    }

    for (int i = 0; i<6; i++){
      if (mpb->get_state(i) != expected[i]){
	fprintf(stderr,"slater determinant unit test 1 faild,\n"
		"state %d is %0.64lx,\n"
		"but should be %0.64lx\n",i, mpb->get_state(i),expected[i]);
	exit(1);
      }
    }
    printf("slater determinant unit test 1 succeded\n");

    
    delete mpb;
    delete spb;
  }
  { // Hard test
    Single_Particle_Basis* spb = new Single_Particle_Basis();
    spb->set_num_shells(4);
    spb->set_dimension(8);
    shell sh;
    sh.l = 0;
    sh.j = 1;
    sp_state st;
    printf("Setting up pair shells and states\n");
    for (int p = 0; p<4; p++){
      sh.n = p+1;
      spb->add_shell(sh);
      st.s = p;
      st.m = -1;
      spb->add_sp_state(st);
      st.m = 1;
      spb->add_sp_state(st);
    }
    Many_Particle_Basis *mpb_all = new Many_Particle_Basis(spb,4);
    Many_Particle_Basis *mpb_pairs = new Many_Particle_Basis(spb,4,0);
    mpb_pairs->keep_pair_excitations_only();
    printf("all 4 particle slater determinants in 4st s shells\n");
    mpb_all->list_states();
    printf("pair excitations only\n");
    mpb_pairs->list_states();
    delete spb;
    delete mpb_all;
    delete mpb_pairs;
  }
}
