#ifndef celestial_object
#define celestial_object

#include<vector>
#include<numbers>

using std::vector;

class Celestial_object
{
private:
  double m_radius{1}; // Units in m
  double m_mass{1}; // Units in kg
  vector<double> m_old_acceleration = std::vector<double>(3); // Units in m / s^2
  vector<double> m_new_acceleration = std::vector<double>(3); // Units in m / s^2


public:
  vector<double> coordinate{vector<double>(3)}; // Units in m
  vector<double> velocity{vector<double>(3)}; // Units in m/s

  Celestial_object() = default;
  ~Celestial_object(){};
  Celestial_object(vector<double> coord, vector<double> velocity, double radius, double mass);

  vector<double> centre_of_mass(Celestial_object& object);
  vector<double> combined_velocity(Celestial_object& object);
  double get_distance(const Celestial_object& object);
  void acceleration_between(Celestial_object& object);
  void update_position_leapfrog();
  void update_velocity_leapfrog();
  void update_acceleration(){m_old_acceleration = m_new_acceleration;}
  Celestial_object operator+(Celestial_object& object);

  void set_mass(double mass){if(mass > 0){m_mass = mass;}}
  void set_radius(double radius){if(radius > 0){m_radius = radius;}}
  void reset_acceleration(){m_new_acceleration = std::vector<double>(3,0);}
  double get_mass(){return m_mass;}
  double get_radius(){return m_radius;}
};


#endif
