#ifndef __SINGLE_PARTICLE__
#define __SINGLE_PARTICLE__

struct shell{
  int n,l,j; // the radial qn, the orbital angular momentum and the total angular momentum
};

struct sp_state{
  int s; // refer to what shell the state is a member of
  int m;
};

class Single_Particle_Basis{
private:
  shell *shells;
  int num_shells;
  int current_shell;
  sp_state *states;
  int dimension;
  int current_state;
public:
  Single_Particle_Basis();
  ~Single_Particle_Basis();
  /* The following functions are intended to be used while setting up
   * a single particle basis
   */
  void set_num_shells(int num_shells);
  void set_dimension(int dimension);
  void add_shell(shell s);
  void add_sp_state(sp_state s);
  /* The following functions are intended to be used when ever some
   * info about the single particle basis is needed
   */
  int get_dimension();
  int get_num_shells();
  shell get_shell(int i);
  sp_state get_sp_state(int i);
};


#endif
