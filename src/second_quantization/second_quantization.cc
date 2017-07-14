#include "second_quantization.hh"


Many_Body_Hamiltonian::Many_Body_Hamiltonian(Many_Particle_Basis* mp_basis){
  this->mp_basis = mp_basis;
  this->n = mp_basis->get_dimension();
  this->matrix = new double[this->n*this->n];
}

Many_Body_Hamiltonian::~Many_Body_Hamiltonian(){
  delete[] this->matrix;
}

inline
bool is_pair_excitation(slatdet s){
  slatdet mask = 0xAAAAAAAAAAAAAAAA;
  return (((s&mask)>>1)^(s&(mask>>1))) == 0;
}

void Many_Body_Hamiltonian::compute_contribution(slatdet state,
						 slatdet annih,
						 slatdet crea,
						 char phase){

  DEBUG_MSG("compute_contribution:\n");
  //DEBUG_MSG("Creates particle\n");
  // search for the corresponding matrix element
  double e = 0.0;
  //DEBUG_MSG("annis: 0x%0.16lx %d\n",annis,is_pair_excitation(annis));
  //DEBUG_MSG("creas: 0x%0.16lx %d\n",creas,is_pair_excitation(creas));
  if (is_pair_excitation(annih) &&
      is_pair_excitation(crea)){
    //DEBUG_MSG("Pair excitation\n");
    e = -1.0;
  }
  //if (e == 0.0)
  //  return;
  // search for the corresponding Slater determinant
  ssize_t ind_j = mp_basis->get_index(state);
  if (ind_j<0){
    DEBUG_MSG("Could not find 0x%0.16lx\n",state);
    return;
  }
  DEBUG_MSG("state 0x%0.16lx\n",state);
  DEBUG_MSG("Connects to column %ld\n",ind_j);
  // update matrix
  // minus since we are doing the creation annihilation stuf backwards
  this->current_row[ind_j] -= e*(1-2*(phase&1));
}

void Many_Body_Hamiltonian::create_particles(slatdet state,
					     slatdet ceiling,
					     int p, char phase,
					     slatdet annis,
					     slatdet creas){
  DEBUG_MSG("Create_particle(%d):\n",p);
  DEBUG_MSG("state: 0x%0.16x\n",state);
  DEBUG_MSG("phase: %d\n",(int)(1-(phase&1)*2));
  DEBUG_MSG("Crea Ceiling: 0x%0.16lx\n",ceiling);
  if (p == 0){
    compute_contribution(state,
			 annis,
			 creas,
			 phase);
    return;
  }
  for(slatdet c = 1; c<(1<<(p-1)); c<<=1){
    if (state&c)
      phase^=1;
  }
  for (slatdet c = 1<<(p-1); c<ceiling; c<<=1){
    DEBUG_MSG("c = 0x%0.16lx\n",c);
    if ((state&c) == 0){
      DEBUG_MSG("Creates 0x%0.16lx\n",c);
      create_particles(state^c,
		       c,p-1,
		       phase,
		       annis,
		       creas|c);
      DEBUG_MSG("Take next possibility\n");
      
    }else{
      phase^=1;
    }
    
  }
  DEBUG_MSG("end create_particles\n");
}

void Many_Body_Hamiltonian::annihilate_particles(slatdet state,
						 slatdet ceiling,
						 int p, char phase,
						 slatdet annis){
  DEBUG_MSG("annihilate_particles(%d):\n",p);
  DEBUG_MSG("state: 0x%0.16x\n",state);
  DEBUG_MSG("phase: %d\n",(int)(1-(phase&1)*2));
  DEBUG_MSG("Anihi Ceiling: 0x%0.16lx\n",ceiling);
  if (p == 0){
    create_particles(state,
		     1<<mp_basis->get_sp_dimension(),
		     2,phase,annis,0);
    return;
  }
  for(slatdet a = 1; a<(1<<(p-1)); a<<=1){
    if (state&a)
      phase^=1;
  }
  
  for (slatdet a = 1<<(p-1); a<ceiling; a<<=1){
    if(a == 0){
      fprintf(stderr,"This should never happen\n");
    }
    if (state&a){
      DEBUG_MSG("Annihilate 0x%0.16lx\n",a);
      annihilate_particles(state^a,
			   a,p-1,
			   phase,
			   annis|a);
      DEBUG_MSG("Take next possibility\n");
      phase^=1;
    }
  }
  DEBUG_MSG("end annihilate_particles\n");
}

void Many_Body_Hamiltonian::compute_matrix(){
  for (size_t i = 0; i<this->n; i++){
    DEBUG_MSG("Working on row %ld\n",i);
    DEBUG_MSG("ket: 0x%0.16lx\n",mp_basis->get_state(i));
    this->current_row = this->matrix+i*this->n;
    annihilate_particles(mp_basis->get_state(i),
			 1<<mp_basis->get_sp_dimension(),
			 2,0,0);

  }
}

void Many_Body_Hamiltonian::display_matrix(){
  printf("Hamiltonian matrix:\n");
  for (size_t i = 0; i<mp_basis->get_dimension(); i++){
    for (size_t j = 0; j<mp_basis->get_dimension(); j++){
      printf("  %4.3f",this->matrix[i*mp_basis->get_dimension()+j]);
    }
    printf("\n");
  }
}




void unit_test_second_quantization(){
  { // 6x6 pairing-matrix test
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
    Many_Particle_Basis *mpb_pairs = new Many_Particle_Basis(spb,4,0);
    mpb_pairs->keep_pair_excitations_only();
    mpb_pairs->set_up_hash();
    Many_Body_Hamiltonian* ch = new Many_Body_Hamiltonian(mpb_pairs);
    printf("Creating matrix for the %ldx%ld case\n",
	   mpb_pairs->get_dimension(),mpb_pairs->get_dimension());
    ch->compute_matrix();
    ch->display_matrix();
  }
}
