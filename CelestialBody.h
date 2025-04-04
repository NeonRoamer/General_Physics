#ifndef CELESTIAL_BODY
#define CELESTIAL_BODY
#include<iostream>
#include<vector>

extern const double G;
extern const double pi;
extern const double mass_earth;
extern const double mass_moon;
extern const double radius_earth;
extern const double radius_moon;
extern const double orb_rad_moon;
extern const double orb_vel_moon;

extern const std::vector< std::vector<double>> tableau;

void print_vector(const std::vector<double>& v);

class CelestialBody
{
  friend std::vector<double> acceleration_calc(const int& target_index, const int& rkf45_step, const std::vector<CelestialBody>& bodies);
  friend std::vector<double> acceleration_calc(const int& i, const int& rkf45_step, const std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc);
  friend void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies);
  friend void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc);
  friend std::vector<double> fourth_order_position(const CelestialBody& body);
  friend std::vector<double> fifth_order_position(const CelestialBody& body);
  friend std::vector<double> fourth_order_velocity(const CelestialBody& body);
  friend std::vector<double> fifth_order_velocity(const CelestialBody& body);
private:
  double mass{0};
  double radius{0};
  std::vector<double> position;
  std::vector<double> velocity;
  std::vector<std::vector<double>> position_coeffs{6, std::vector<double>(3)};
  std::vector<std::vector<double>> velocity_coeffs{6, std::vector<double>(3)};
public:
  CelestialBody() : position{std::vector<double>(3)}, velocity{std::vector<double>(3)} {}
  ~CelestialBody() {std::cout<<"Celestial object destroyed"<<std::endl;}
  CelestialBody(const double& body_mass, const double& body_radius, const double& posx, const double& posy, const double& posz, const double& velx, const double& vely, const double& velz);
  CelestialBody(const double& body_mass, const double& body_radius, const std::vector<double>& body_position, const std::vector<double>& body_velocity);
  
  //Setters and getters
  void set_velocity(const std::vector<double>& new_velocity);
  void set_velocity(const double& vx, const double& vy, const double& vz);
  void set_position(const std::vector<double>& new_position);
  void set_position(const double& px, const double& py, const double& pz);
  void set_mass(const double& m);
  void set_radius(const double& rad);

  std::vector<double> get_velocity() const {return velocity;}
  std::vector<double> get_position() const {return position;}
  double get_posx() const {return position[0];}
  double get_posy() const {return position[1];}
  double get_posz() const {return position[2];}
  double get_mass() const {return mass;}
  double get_radius() const {return radius;}
};

//Member functions
std::vector<double> acceleration_calc(const int& i, const int& rkf45_step, const std::vector<CelestialBody>& bodies);
std::vector<double> acceleration_calc(const int& i, const int& rkf45_step, const std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc);
void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies);
void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc);
std::vector<double> fourth_order_position(const CelestialBody& body);
std::vector<double> fifth_order_position(const CelestialBody& body);
std::vector<double> fourth_order_velocity(const CelestialBody& body);
std::vector<double> fifth_order_velocity(const CelestialBody& body);

//Vector maths
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);

void operator+=(std::vector<double>& v1, const std::vector<double>& v2);

std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);

void operator-=(std::vector<double>& v1, const std::vector<double>& v2);

std::vector<double> operator+(const std::vector<double>& v1, const double& s1);

std::vector<double> operator+(const double& s1, const std::vector<double>& v1);

std::vector<double> operator+(const std::vector<double>& v1, const int& s1);

std::vector<double> operator+(const int& s1, const std::vector<double>& v1);

std::vector<double> operator-(const std::vector<double>& v1, const double& s1);

std::vector<double> operator-(const double& s1, const std::vector<double>& v1);

std::vector<double> operator-(const std::vector<double>& v1, const int& s1);

std::vector<double> operator-(const int& s1, const std::vector<double>& v1);

std::vector<double> operator*(const std::vector<double>& v1, const double& s1);

std::vector<double> operator*(const double& s1, const std::vector<double>& v1);

std::vector<double> operator*(const std::vector<double>& v1, const int& s1);

std::vector<double> operator*(const int& s1, const std::vector<double>& v1);

std::vector<double> operator/(const std::vector<double>& v1, const double& s1);

std::vector<double> operator/(const double& s1, const std::vector<double>& v1);

std::vector<double> operator/(const std::vector<double>& v1, const int& s1);

std::vector<double> operator/(const int& s1, const std::vector<double>& v1);

void operator*=(std::vector<double>& v1, const double& s1);

void operator/=(std::vector<double>& v1, const double& s1);

double abs(const std::vector<double>& v1);

#endif
