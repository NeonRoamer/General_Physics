#include<string>
#include<vector>
#include "global.h"

// This file is the user side where you can change all the important variables.
// Will later add the making of the celestial objects here

// The file to save the data to
const std::string data_file("celestial_object_simulation.txt");
const std::string energy_file("celestial_object_energy.txt");
const std::string momentum_file("celestial_object_momentum.txt");
const double gravitational_constant{6.67430e-11}; // Units m^3 kg^-1 s^-2
const bool leapfrog_method{true};

//keep this false it doesn't work
const bool rk5_method{false};

// //Units in s
const double delta_time{100}; // Time between calculations
const double total_time{365*24*60*60}; // Total time of the simulation
const int output_time{12*60*60}; // time between each output to the file
bool start_of_simulation{true};


// None of this is used right now it will be used to make random objects
//Random Objects
const bool random_obj{false};
const int number_of_objects{7};

//Units in kg
const double min_mass{5e24};
const double max_mass{1e28};

//Units in m
const double min_radius{4e6};
const double max_radius{10e9};
const double min_coord{10};
const double max_coord{15};

