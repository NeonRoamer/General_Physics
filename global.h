#ifndef global
#define global

#include<string>

// extern means that other files can use the variables here. i.e. the variables are global
extern const std::string data_file;
extern const std::string energy_file;
extern const std::string momentum_file;
extern const std::string collision_file;
extern const double gravitational_constant;
extern const bool leapfrog_method;
extern const int n_test;

//Units in s
extern const double delta_time;
extern const double total_time;
extern const int output_time;
extern bool start_of_simulation;


//Random Objects
extern const bool continuous_force;
extern const double min_acc;// N
extern const double max_acc;// N

extern const double min_impulse;// Ns
extern const double max_impulse;// Ns

#endif
