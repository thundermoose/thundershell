#ifndef __SLATER_DETERMINANTS__
#define __SLATER_DETERMINANTS__
#include <cstdlib>
#include <stdint.h>
#include "../single_particles/single_particles.hh" 


// a slater determinant will in this code be represented
// with a 64bit integer, where each bit is an ocupation number
typedef uint64_t slatdet;



class Many_Particle_Basis{
private:
  Single_Particle_Basis *sp_basis;
  slatdet* states;
  size_t dimension;
  size_t current_state;
  int A;
  void create_particles(slatdet s, int p, int F);
  void annihilate_particles(slatdet s, int h,int p, int F);
  int get_M(slatdet s);
public:
  Many_Particle_Basis(Single_Particle_Basis* sp_basis,int A);
  Many_Particle_Basis(Single_Particle_Basis* sp_basis,int A,int M);
  ~Many_Particle_Basis();
  void keep_pair_excitations_only();
  void list_states();
  size_t get_dimension();
  slatdet get_state(size_t i);
};

void unit_test_slater_determinants();

#endif
