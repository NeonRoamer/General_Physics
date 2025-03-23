#include<string>
#include "global.h"

const std::string data_file("celestial_object_simulation.txt");
const double gravitational_constant{6.67430e-11}; // Units m^3 kg^-1 s^-2

// //Units in s
const double delta_time{0.001};
const int total_time{14*24*60*60};
const int output_time{60*60};// time between each output to the file


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

