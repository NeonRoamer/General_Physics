#include<vector>
#include<random>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<string>
#include<sstream>
#include<memory>
#include "global.h"
#include "celestial_object.h"

using std::vector;
double total_mass(vector<Celestial_object>& objects);


void centre_of_mass(vector<Celestial_object>& objects)
{
  vector<double> centre_mass_coord{vector<double>(3)};
  double mass{total_mass(objects)};

  for(int i{0}; i< 3; ++i)
  {
    for(int j{0}; j < objects.size(); ++j){centre_mass_coord[i] += objects[j].get_mass() * objects[j].coordinate[i];}
    centre_mass_coord[i] = centre_mass_coord[i] / mass;
  }

  for(int i{0}; i< 3; ++i)
  {
    for(int j{0}; j < objects.size(); ++j){objects[j].coordinate[i] -= centre_mass_coord[i];}
  }
}

double total_mass(vector<Celestial_object>& objects)
{
  double mass{0};
  for(int i{0}; i<objects.size(); ++i){mass += objects[i].get_mass();}
  return mass;
}

// void random_objects()
// {
//   std::mt19937 mt{std::random_device{}()};
//   std::uniform_int_distribution mass{min_mass, max_mass}; // Units in kg
//   std::uniform_int_distribution radius{min_radius, max_radius}; // Units in m
//   std::uniform_int_distribution coord{min_coord, max_coord}; // Units in m

//   vector<Celestial_object> object;
//   object.reserve(number_of_objects);
//   for(int i{0}; i < number_of_objects; ++i)
//   {

//   }
// }



int main()
{
  double distance_earth_moon{3.844e8};
  double circular_velocity = 1e6* sqrt(gravitational_constant * 5.972e24 / 5.972e22);
  std::ofstream file(data_file, std::ofstream::trunc);

  
  vector<Celestial_object> objects;
  objects.reserve(2);
  Celestial_object earth(vector<double>(3,0), vector<double>(3,0), 6.378e6, 5.972e24);
  Celestial_object moon(vector<double>{distance_earth_moon, 0, 0}, std::vector<double>{0, circular_velocity, 0}, 1.17374e6, 7.34767309e22);
  objects.push_back(earth);
  objects.push_back(moon);
  centre_of_mass(objects);

  std::ostringstream oss;
  for(int i{0}; i < total_time / delta_time; ++i)
  {
    for(int j{0}; j < objects.size()-1; ++j){for(int k{j+1}; k < objects.size(); ++k){objects[j].acceleration_between(objects[k]);}}
    for(int j{0}; j < objects.size(); ++j)
    {
      if(remainder(i * delta_time, output_time) == 0)
      {
        oss << "  ";
        for (int k = 0; k < 3; ++k) 
        {
          oss << objects[j].coordinate[k];
          oss << ", ";
        }
        oss << objects[j].get_radius();
        if (j < objects.size()-1){oss << ", ";}
      }
      objects[j].update_position_leapfrog();
      objects[j].update_velocity_leapfrog();
      objects[j].update_acceleration();
      objects[j].reset_acceleration();
    }
    if(remainder(i * delta_time, output_time) == 0){oss << '\n';}
    if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5 * 1024 * 1024))
    {
      file << oss.str();
      oss.str("");
      oss.clear();
    }
  }
  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();
  return 0;
}
