#include<vector>
#include<valarray>
#include<numbers>
#include<math.h>
#include<string>
#include<iostream>
#include<sstream>
#include "global.h"
#include "celestial_object.h"

Celestial_object::Celestial_object(std::vector<double> coord, std::vector<double> velocity, double radius, double mass): coordinate{coord}, velocity{velocity}
{
  set_mass(mass);
  set_radius(radius);
}

std::vector<double> Celestial_object::centre_of_mass(Celestial_object& object)
{
  std::vector<double> coord{std::vector<double>(3)};
  for(int i{0}; i<3; ++i){coord.push_back((m_mass * coordinate[i] + object.get_mass() * object.coordinate[i]) / (m_mass + object.get_mass()));}
  return coord;
}

std::vector<double> Celestial_object::combined_velocity(Celestial_object& object)
{
  std::vector<double> velocity{std::vector<double>(3)};
  for(int i{0}; i<3; ++i){velocity.push_back((m_mass * velocity[i] + object.get_mass() * object.velocity[i]) / (m_mass + object.get_mass()));}
  return velocity;
}

double Celestial_object::get_distance(Celestial_object& object)
{
  return std::sqrt(pow(coordinate[0] - object.coordinate[0], 2) + pow(coordinate[1] - object.coordinate[1], 2) + pow(coordinate[2] - object.coordinate[2], 2));
}

void Celestial_object::acceleration_between(Celestial_object& object)
{
  double distance{get_distance(object)};
  double force{gravitational_constant * m_mass * object.get_mass() / pow(distance, 3)};
  for(int i{0}; i<3; ++i)
  {
    m_new_acceleration[i] += force * (coordinate[i] - object.coordinate[i]) / m_mass;
    object.m_new_acceleration[i] += force * (object.coordinate[i] - coordinate[i]) / object.get_mass();
  }
}

void Celestial_object::update_position_leapfrog()
{
  for(int i{0}; i < 3; ++i)
  {
    coordinate[i] = coordinate[i] + velocity[i] * delta_time + 0.5 * m_new_acceleration[i] * pow(delta_time, 2);
  }
}

void Celestial_object::update_velocity_leapfrog()
{
  for(int i{0}; i < 3; ++i)
  {
    velocity[i] = velocity[i] + 0.5 * (m_old_acceleration[i] + m_new_acceleration[i]) * delta_time;
  }
}

Celestial_object Celestial_object::operator+(Celestial_object& object)
{
  coordinate = centre_of_mass(object);
  velocity = combined_velocity(object);
  set_mass(m_mass + object.get_mass());
  set_radius(m_radius + object.get_radius());
  return *this;
}
