#ifndef global
#define global

#include<string>

// extern means that other files can use the variables here. i.e. the variables are global
extern const std::string data_file;
extern const std::string energy;
extern const double gravitational_constant; 

//Units in s
extern const double delta_time;
extern const double total_time;
extern const int output_time;
extern bool start_of_simulation;


//Random Objects
extern const bool random_obj;
extern const int number_of_objects;
//Units in kg
extern const double min_mass;
extern const double max_mass;

//Units in m
extern const double min_radius;
extern const double max_radius;
extern const double min_coord;
extern const double max_coord;

#endif
