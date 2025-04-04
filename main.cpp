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

void yax_rotation(std::vector<double>& vec, const double& inc_angle)  //Rotates the frame of asteroid headed to earth at random inclination angle from the z-axis
{
  std::vector<double> temp_vec(3);
  temp_vec[0] = vec[0] * cos(inc_angle) - vec[2] * sin(inc_angle);
  temp_vec[1] = vec[1];
  temp_vec[2] = vec[0] * sin(inc_angle) + vec[2] * cos(inc_angle);
  vec = temp_vec;
}

void remove_momentum(std::vector<Celestial_object>& objects)
{
  for(int j{0}; j<3; ++j)
  {
    for(int i{1}; i<objects.size(); ++i)
    {
      objects[0].velocity[j] -= (objects[i].get_mass() * objects[i].velocity[j]) / objects[0].get_mass();
    }
  }
}


// This is where the code begins.
int main()
{
  auto start = std::chrono::high_resolution_clock::now();
  double distance_earth_moon{3.844e8};
  double circular_velocity = 1020;

  // Creates/opens a file (name saved in data_file) and clears the file
  std::ofstream file(data_file, std::ofstream::trunc);
  std::ofstream file_energy(energy_file, std::ofstream::trunc);
  std::ofstream file_momentum(momentum_file, std::ofstream::trunc);
  std::ofstream file_collision(collision_file, std::ofstream::trunc);

  // oss is a buffer for strings and values
  std::ostringstream oss;
  std::ostringstream oss_energy;
  std::ostringstream oss_momentum;
  std::ostringstream oss_collision;

  // Creates a vector of celestial objects.
  vector<Celestial_object> objects;

  // The vector has a length of 2
  objects.reserve(3);

  // Creates both Celestial objects
  // The inputs is position, velocity, radius, and then mass
  Celestial_object earth(std::vector<double>{0, 0, 0}, std::vector<double>{0, 0, 0}, 6.378e6, 5.972e24);
  Celestial_object moon(std::vector<double>{3.8e8, 0, 0}, std::vector<double>{0, 1022, 0}, 1.17374e6, 7.34767309e22);
  Celestial_object asteriod(std::vector<double>{3.88e9, 0, 0}, std::vector<double>{-3.88e9/(60*60*24*15), 0, 0}, 4.73e5, 9.39e20);

  double pi{3.1415926535};
  std::mt19937 mt{ std::random_device{}() };
  int collided;
  double acceleration;
  for(int l{0}; l<n_test; ++l)
  {
    collided = 0;
    objects.clear();

    std::uniform_real_distribution theta(0.0, pi / 2);
    std::uniform_real_distribution phi(0.0, 2 * pi);
    std::uniform_real_distribution elevation(0.0, pi/2);
    std::uniform_real_distribution acc(min_acc, max_acc);
    std::uniform_real_distribution impulse(min_impulse, max_impulse);

    if(continuous_force)
    {
      acceleration = acc(mt);
      asteriod.m_instant_force = false;
    }
    //else{acceleration = 1e300*impulse(mt) / delta_time;}
    
    double theta_value{theta(mt)};
    double phi_value{phi(mt)};
    asteriod.m_acc[0] = acceleration * cos(theta_value);
    asteriod.m_acc[1] = acceleration * sin(theta_value) * cos(phi_value);
    asteriod.m_acc[2] = acceleration * sin(theta_value) * sin(phi_value);

    double elev{elevation(mt)};
    yax_rotation(asteriod.coordinate, elev);
    yax_rotation(asteriod.velocity, elev);
    yax_rotation(asteriod.m_acc, elev);

    // Adds the celestial objects to the vector objects
    objects.push_back(earth);
    objects.push_back(moon);
    objects.push_back(asteriod);

    // Makes the centre of mass the point (0, 0, 0)
    centre_of_mass(objects);
    remove_momentum(objects);

    double initial_energy{0};
    double initial_momentum{0};
    for(int i{0}; i< objects.size(); ++i)
    {
      initial_energy += objects[i].get_energy(objects);
      initial_momentum += objects[i].get_momentum();
    }

    // Loops for time time required.
    for(int i{0}; i < total_time / delta_time; ++i)
    {
      double energy{0};
      double momentum{0};

      if(leapfrog_method)
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
          energy += objects[j].get_energy(objects);
          momentum += objects[j].get_momentum();
          objects[j].update_position_leapfrog();
          objects[j].update_velocity_leapfrog();
          objects[j].update_acceleration();
          objects[j].reset_acceleration();
          objects[j].first_calculation = true;
        }
      }


      // Goes to the next line in the file
      if(remainder(i * delta_time, output_time) == 0)
      {
        oss_energy << std::log10(energy / initial_energy);
        oss_momentum << std::log10(momentum / initial_momentum);
        oss_energy << ", ";
        oss_momentum << ", ";
        oss << '\n';
      }

      // This codes makes the oss write to the file if it is holding more than 5MB worth of data
      if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5 * 1024 * 1024))
      {
        file << oss.str();
        oss.str("");
        oss.clear();
      }
      if(objects[0].get_distance(objects[2]) < objects[0].get_radius() + objects[2].get_radius())
      {
        collided = true;
        break;
      }
      start_of_simulation = false;
    }
    oss_collision << collided << ", " << acceleration << ", " << theta_value << '\n';
  }

  // writes everything left in oss to the file and closes the 
  file_collision << oss_collision.str();

  std::string energy_str = oss_energy.str();
  std::string momentum_str = oss_momentum.str();
  energy_str.pop_back();
  energy_str.pop_back();
  momentum_str.pop_back();
  momentum_str.pop_back();
  file_energy << energy_str;
  file_momentum << momentum_str;

  file << oss.str();
  oss.clear();
  oss_energy.clear();
  oss_momentum.clear();
  oss_collision.clear();
  file.close();
  file_energy.close();
  file_momentum.close();
  file_collision.close();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time: " << duration.count()/1000 << " seconds" << std::endl;
  return 0;
}
