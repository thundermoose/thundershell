#ifndef __SLATER_DETERMINANTS__
#define __SLATER_DETERMINANTS__
#include <cstdlib>
#include <stdint.h>
#include "../single_particles/single_particles.hh" 


// a slater determinant will in this code be represented
// with a 64bit integer, where each bit is an ocupation number
typedef uint64_t slatdet;

struct hash_element{
  slatdet key;
  ssize_t i;
};


class Many_Particle_Basis{
private:
  Single_Particle_Basis *sp_basis;
  slatdet* states;
  
  size_t dimension;
  hash_element* hash_array;
  size_t hash_size;
  size_t current_state;
  int A;
  void add_to_hash(slatdet s, size_t i);
  void create_particles(slatdet s, int p, int F);
  void annihilate_particles(slatdet s, int h,int p, int F);
  int get_M(slatdet s);
public:
  Many_Particle_Basis(Single_Particle_Basis* sp_basis,int A);
  Many_Particle_Basis(Single_Particle_Basis* sp_basis,int A,int M);
  ~Many_Particle_Basis();
  void set_up_hash();
  void keep_pair_excitations_only();
  void list_states();
  size_t get_dimension();
  size_t get_sp_dimension();
  slatdet get_state(size_t i);
  ssize_t get_index(slatdet s);
};

void unit_test_slater_determinants();

#endif
