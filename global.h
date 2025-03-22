#ifndef global
#define global

#include<string>

extern const std::string data_file;
extern const double gravitational_constant; 

//Units in s
extern const double delta_time;
extern const int total_time;
extern const int output_time;


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
