#ifndef celestial_object // you can ignore this (it's a header guard)
#define celestial_object // it stops the code from compiling multiple times

// This is the class celestial object it has a radius, mass, old and new acceleration, coordinates, and velocity.
// This page initialises all the functions in this class

// equivalent of import
#include<vector>
#include<numbers>

// allows me to write vector instead of std::vector everywhere
using std::vector;

// Creates a class called Celestial_object
class Celestial_object
{
// This can only be accessed from within the class functions
private:
  double m_radius{1}; // Units in m
  double m_mass{1}; // Units in kg
  vector<double> m_old_acceleration = std::vector<double>(3); // Units in m / s^2
  vector<double> m_new_acceleration = std::vector<double>(3); // Units in m / s^2

// This can be accessed both within and outside the class functions
public:
  vector<double> coordinate{vector<double>(3)}; // Units in m
  vector<double> velocity{vector<double>(3)}; // Units in m/s

  // This creates the object when no inputs are given
  Celestial_object() = default;
  // This destroys the object
  ~Celestial_object(){};
  // This creates the object with inputs
  Celestial_object(vector<double> coord, vector<double> velocity, double radius, double mass);

  // These are member functions
  vector<double> centre_of_mass(Celestial_object& object);
  vector<double> combined_velocity(Celestial_object& object);
  double get_distance(const Celestial_object& object);
  void acceleration_between(Celestial_object& object);
  void update_position_leapfrog();
  void update_velocity_leapfrog();
  Celestial_object operator+(Celestial_object& object);

  // These set and get the values of variables in the private section
  void set_mass(double mass){if(mass > 0){m_mass = mass;}}
  void set_radius(double radius){if(radius > 0){m_radius = radius;}}
  void update_acceleration(){m_old_acceleration = m_new_acceleration;}
  void reset_acceleration(){m_new_acceleration = std::vector<double>(3,0);}
  double get_mass(){return m_mass;}
  double get_radius(){return m_radius;}
};


#endif
