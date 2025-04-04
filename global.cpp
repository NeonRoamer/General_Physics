#include<string>
#include<vector>
#include "global.h"

// This file is the user side where you can change all the important variables.
// Will later add the making of the celestial objects here

// The file to save the data to
const std::string data_file("celestial_object_simulation.txt");
const std::string energy_file("celestial_object_energy.txt");
const std::string momentum_file("celestial_object_momentum.txt");
const std::string collision_file("force.txt");
const double gravitational_constant{6.67430e-11}; // Units m^3 kg^-1 s^-2
const bool leapfrog_method{true};
const int n_test{100};

//keep this false it doesn't work

// //Units in s
const double delta_time{10}; // Time between calculations
const double total_time{15*24*60*60}; // Total time of the simulation
const int output_time{60*60}; // time between each output to the file
bool start_of_simulation{true};


// None of this is used right now it will be used to make random objects
//Random Objects
const bool continuous_force{false};

const double min_acc{1e15};// N
const double max_acc{1.5e16};// N

const double min_impulse{1e21};// Ns
const double max_impulse{1e22};// Ns
