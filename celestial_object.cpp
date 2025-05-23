#include<vector>
#include<valarray>
#include<numbers>
#include<math.h>
#include<string>
#include<iostream>
#include<sstream>
#include<cmath>
#include "global.h"
#include "celestial_object.h"

// This creates the object and sets the input values to the corresponding variables.
Celestial_object::Celestial_object(std::vector<double> coord, std::vector<double> velocity, double radius, double mass): coordinate{coord}, velocity{velocity}
{
  set_mass(mass);
  set_radius(radius);
}

// This calculates the locations of the centre of mass between two objects
std::vector<double> Celestial_object::centre_of_mass(Celestial_object& object)
{
  std::vector<double> coord{std::vector<double>(3)};
  for(int i{0}; i<3; ++i)
  {
    coord[i] = ((m_mass * coordinate[i] + object.get_mass() * object.coordinate[i]) / (m_mass + object.get_mass()));
  }
  return coord;
}

// This combines the velocity of two objects
std::vector<double> Celestial_object::combined_velocity(Celestial_object& object)
{
  std::vector<double> velocity{std::vector<double>(3)};
  for(int i{0}; i<3; ++i)
  {
    velocity.push_back((m_mass * velocity[i] + object.get_mass() * object.velocity[i]) / (m_mass + object.get_mass()));
  }
  return velocity;
}

// This gets the distance between two objects
double Celestial_object::get_distance(const Celestial_object& object)
{
  return std::sqrt(pow(coordinate[0] - object.coordinate[0], 2) + pow(coordinate[1] - object.coordinate[1], 2) + pow(coordinate[2] - object.coordinate[2], 2));
}

// This finds the acceleration between two objects
void Celestial_object::acceleration_between(Celestial_object& object)
{
  double distance{get_distance(object)};
  double force{gravitational_constant * m_mass * object.get_mass() / (distance * distance * distance)};

  for(int i{0}; i<3; ++i)
  {
    m_new_acceleration[i] += (force) * (object.coordinate[i] - coordinate[i]) / m_mass;
    object.m_new_acceleration[i] += force * (coordinate[i] - object.coordinate[i]) / object.get_mass();
  }

  if((m_instant_force && time == 0 && first_calculation) || (m_instant_force == false && first_calculation))
  {
    for(int i{0}; i<3; ++i){m_new_acceleration[i] += m_acc[i]/m_mass;}
  }

  if((object.m_instant_force && time == 0 && object.first_calculation) || (object.m_instant_force == false && object.first_calculation))
  {
    for(int i{0}; i<3; ++i){object.m_new_acceleration[i] += object.m_acc[i]/object.m_mass;}
  }

  first_calculation = false;
  object.first_calculation = false;
}

// Uses leapfrog method to calculate and update the position of the object
void Celestial_object::update_position_leapfrog()
{
  for(int i{0}; i < 3; ++i)
  {
    coordinate[i] += velocity[i] * delta_time + 0.5 * m_new_acceleration[i] * delta_time * delta_time;
  }
}

// Uses leapfrog method to calculate and update the velocity of the object
void Celestial_object::update_velocity_leapfrog()
{
  for(int i{0}; i < 3; ++i)
  {
    velocity[i] += 0.5 * (m_old_acceleration[i] + m_new_acceleration[i]) * delta_time;
  }
}

double Celestial_object::total_acceleration()
{
  return sqrt(m_old_acceleration[0]*m_old_acceleration[0] + m_old_acceleration[1]*m_old_acceleration[1] + m_old_acceleration[2] * m_old_acceleration[2]);
}

double Celestial_object::get_energy(std::vector<Celestial_object>& objects)
{
  double kenetic_energy{0.5 * m_mass * (velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2])};
  double potential_energy{0};
  for(int i{0}; i<objects.size(); ++i)
  {
    if(this != &objects[i])
    {
      potential_energy -= gravitational_constant * m_mass * objects[i].get_mass() / this->get_distance(objects[i]);
    }
  }
  return kenetic_energy + potential_energy;
}

double Celestial_object::get_momentum()
{
  return m_mass * sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
}

// This defines what happens when Celestial_object1 + Celestial_object2. This is not currently used but
// it will be useful for combining objects when they collide.
// Celestial_object Celestial_object::operator+(Celestial_object& object)
// {
//   return Celestial_object(centre_of_mass(object), combined_velocity(object), m_mass + object.get_mass(), m_radius + object.get_radius());
// }


