#include<string>
#include<vector>
#include "global.h"

// This file is the user side where you can change all the important variables.
// Will later add the making of the celestial objects here

// The file to save the data to
const std::string data_file("celestial_object_simulation.txt");
const double gravitational_constant{6.67430e-11}; // Units m^3 kg^-1 s^-2

// //Units in s
const double delta_time{10}; // Time between calculations
const double total_time{60*1000}; // Total time of the simulation
const int output_time{60};// time between each output to the file
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

