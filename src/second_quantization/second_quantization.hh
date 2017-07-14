#ifndef __SECOND_QUANTIZATION__
#define __SECOND_QUANTIZATION__

#include <cstdio>
#include <cstdlib>
#include "../debug/debug.h"
#include "../slater_determinants/slater_determinants.hh"

class Many_Body_Hamiltonian{
private:
  Many_Particle_Basis* mp_basis;
  double* matrix;
  double* current_row;
  int n;
  void compute_contribution(slatdet state,
			   slatdet annih,
			   slatdet crea,
			   char phase);
  
  void create_particles(slatdet state,
			slatdet ceiling,
			int p, char phase,
			slatdet annis,
			slatdet creas);
  void annihilate_particles(slatdet state,
			    slatdet ceiling,
			    int p, char phase,
			    slatdet annis);
public:
  Many_Body_Hamiltonian(Many_Particle_Basis* mp_basis);
  ~Many_Body_Hamiltonian();
  void compute_matrix();
  void display_matrix();
};

void unit_test_second_quantization();

#endif
