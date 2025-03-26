// Equivalent of import
#include<vector> // std::vector
#include<random> // for random numbers not currently used
#include<iostream> // std::cout (print), std::cin (input)
#include<fstream> // to do with strings
#include<cstdio> // allows reading and writing from a file
#include<cmath> // allows more mathematical operations like sqrt
#include<string> // std::string
#include<sstream> // allows the storage of strings in a buffer before being displayed it's for optimisation
#include<memory> // smart pointers (currently not in use)
#include <chrono>
#include "global.h" // This allows the use of the  file global.h and global.cpp
#include "celestial_object.h" // This allows the use of the  file Celestial_objects.h and Celestial_objects.cpp

//allows the use of vector instead of std::vector
using std::vector;

// This line says that there is a function called total_mass in this file.
double total_mass(vector<Celestial_object>& objects);

// Function called centre of mass that takes in a vector of Celestial Objects.
// Should find the centre of mass and change the positions of all celestial objects to make the centre of mass at (0, 0, 0)
// Void means it returns nothing
void centre_of_mass(vector<Celestial_object>& objects)
{
  // This creates a vector holding double (equivalent of python float). vector<double>(3) means it has a length 0f 3.
  vector<double> centre_mass_coord{vector<double>(3)};
  double mass{total_mass(objects)};

  // Loops 3 times
  for(int i{0}; i< 3; ++i)
  {
    // Loops the same number of times as the size of objects
    for(int j{0}; j < objects.size(); ++j)
    {
      centre_mass_coord[i] += objects[j].get_mass() * objects[j].coordinate[i];
    }
    centre_mass_coord[i] = centre_mass_coord[i] / mass;
  }

  for(int i{0}; i< 3; ++i)
  {
    for(int j{0}; j < objects.size(); ++j)
    {
      objects[j].coordinate[i] -= centre_mass_coord[i];
    }
  }
}

// Finds the total mass of the object
double total_mass(vector<Celestial_object>& objects)
{
  double mass{0};
  for(int i{0}; i<objects.size(); ++i)
  {
    mass += objects[i].get_mass();
  }
  return mass;
}

// Not used. Will be fore making lots of random celestial objects
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


// This is where the code begins.
int main()
{
  auto start = std::chrono::high_resolution_clock::now();
  double distance_earth_moon{3.844e8};
  double circular_velocity = 1020;

  // Creates/opens a file (name saved in data_file) and clears the file
  std::ofstream file(data_file, std::ofstream::trunc);

  // Creates a vector of celestial objects.
  vector<Celestial_object> objects;

  // The vector has a length of 2
  objects.reserve(2);

  // Creates both Celestial objects
  // The inputs is position, velocity, radius, and then mass
  Celestial_object sun(std::vector<double>{0, 0, 0}, std::vector<double>{0, -15.768, 0}, 6.96e8, 1.9889e30);
  Celestial_object earth(std::vector<double>{150e9, 0, 0}, std::vector<double>{0, 28900, 0}, 6.378e6, 5.972e24);
  //Celestial_object moon(vector<double>{159e9, 3.844e8, 0}, std::vector<double>{-1022, 29922, 0}, 1.17374e6, 7.34767309e22);
  Celestial_object mercury(std::vector<double>{58e9, 0, 0}, std::vector<double>{0, 47400, 0}, 2.44e6, 3.302e23);
  Celestial_object venus(std::vector<double>{108e9, 0, 0}, std::vector<double>{0, 35000, 0}, 6.052e6, 4.867e24);
  Celestial_object mars(std::vector<double>{228e9, 0, 0}, std::vector<double>{0, 24100, 0}, 3.39e6, 6.417e23);
  Celestial_object jupiter(std::vector<double>{778e9, 0, 0}, std::vector<double>{0, 13100, 0}, 6.9911e7, 1.898e27);
  Celestial_object saturn(std::vector<double>{1434e9, 0, 0}, std::vector<double>{0, 9700, 0}, 5.8232e7, 5.684e26);
  Celestial_object uranus(std::vector<double>{2900e9, 0, 0}, std::vector<double>{0, 6800, 0}, 2.5362e7, 8.861e25);
  Celestial_object neptune(std::vector<double>{4500e9, 0, 0}, std::vector<double>{0, 5400, 0}, 2.4622e7, 1.024e24);

  // Adds the celestial objects to the vector objects
  objects.push_back(earth);
  //objects.push_back(moon);
  objects.push_back(sun);
  objects.push_back(mercury);
  objects.push_back(venus);
  objects.push_back(mars);
  objects.push_back(jupiter);
  objects.push_back(saturn);
  objects.push_back(uranus);
  objects.push_back(neptune);

  // Makes the centre of mass the point (0, 0, 0)
  centre_of_mass(objects);

  // oss is a buffer for strings and values
  std::ostringstream oss;

  // Loops for time time required.
  for(int i{0}; i < total_time / delta_time; ++i)
  {
    for(int j{0}; j < objects.size()-1; ++j)
    {
      for(int k{j+1}; k < objects.size(); ++k)
      {
        // With the loops it should calculate the acceleration between all Celestial objects in the vector objects once
        objects[j].acceleration_between(objects[k]);
      }
    }
    for(int j{0}; j < objects.size(); ++j)
    {
      // The slowest bit of the code by far is writing to the file therefore instead of writing every 
      //calculated coord it only writes it every output_time.
      if(remainder(i * delta_time, output_time) == 0)
      {
        for (int k = 0; k < 3; ++k) 
        {
          // Rounds to the nearest integer so that 1e-96 is saved as 0 instead. 
          // The 1e-96 occurs because double aren't infintely exact.
          oss << std::round(objects[j].coordinate[k]);
          oss << ", ";
        }
        oss << objects[j].get_radius();
        if (j < objects.size()-1){oss << ", ";}
      }
      // This should use the leapfrog also known as verlet method to calculated the updated position and velocity
      objects[j].update_position_leapfrog();
      objects[j].update_velocity_leapfrog();
      objects[j].update_acceleration();
      objects[j].reset_acceleration();
    }
    // Goes to the next line in the file
    if(remainder(i * delta_time, output_time) == 0)
    {
      oss << '\n';
    }

    // This codes makes the oss write to the file if it is holding more than 5MB worth of data
    if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5 * 1024 * 1024))
    {
      file << oss.str();
      oss.str("");
      oss.clear();
    }
  }
  // writes everything left in oss to the file and closes the file
  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time: " << duration.count()/1000 << " seconds" << std::endl;
  return 0;
}
