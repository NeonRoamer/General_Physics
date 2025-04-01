#include<iostream>
#include<vector>
#include<cmath>
#include "CelestialBody.h"

const double G = 6.6743e-11; //standard units
const double mass_earth = 5.9722e24; //kg
const double mass_moon = 7.348e22; //kg
const double radius_earth = 6.378e6; //m
const double radius_moon = 1.737e6; //m
const double orb_rad_moon = 3.844e8; //m
const double orb_vel_moon = 1022; //ms^-1

const std::vector<std::vector<double>> tableau{
  {0,                  0,          0,         0,      0, 0},
  {1/4,                0,          0,         0,      0, 0},
  {3/32,            9/32,          0,         0,      0, 0},
  {1932/2197, -7200/2197,  7296/2197,         0,      0, 0},
  {439/216,           -8,   3680/513, -845/4104,      0, 0},
  {-8/27,              2, -3544/2565, 1859/4104, -11/40, 0}
  };

void print_vector(const std::vector<double>& v)
{
  std::cout << '[';
  for(auto i = v.begin(); i < v.end(); i++) std::cout << *i << ", ";
  std::cout << ']' << std::endl;
  return;
}

CelestialBody::CelestialBody(const double& body_mass, const double& body_radius, const double& posx, const double& posy, const double& posz, const double& velx, const double& vely, const double& velz) :
  mass(body_mass), radius(body_radius), position(std::vector<double>{posx, posy, posz}), velocity(std::vector<double>{velx, vely, velz}) {}

CelestialBody::CelestialBody(const double& body_mass, const double& body_radius, const std::vector<double>& body_position, const std::vector<double>& body_velocity) :
  mass(body_mass), radius(body_radius), position(body_position), velocity(body_velocity){}

void CelestialBody::set_velocity(const std::vector<double>& v)
{
  CelestialBody::velocity = std::vector<double>{ v[0], v[1], v[2] };
  return;
}
void CelestialBody::set_velocity(const double& vx, const double& vy, const double& vz)
{
  CelestialBody::velocity = std::vector<double>{vx, vy, vz};
  return;
}
void CelestialBody::set_position(const std::vector<double>& p)
{
  CelestialBody::position = std::vector<double>{ p[0], p[1], p[2] };
  return;
}
void CelestialBody::set_position(const double& px, const double& py, const double& pz)
{
  CelestialBody::position = std::vector<double>{px, py, pz};
  return;
}
void CelestialBody::set_mass(const double& m)
{
  CelestialBody::mass = m;
  return;
}
void CelestialBody::set_radius(const double& rad)
{
  CelestialBody::radius = rad;
  return;
}

//Position, velocity and acceleration vectors are consistent length std::vectors of doubles
std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2)
{
  std::vector<double> temp(v1.size());
  if(v1.size() == v2.size()) for(int i=0; i<v1.size(); i++) temp[i] = v1[i] + v2[i];
  else for(int i = 0; i < v1.size(); i++) temp[i] = v1[i];
  return temp;
}

void operator+=(std::vector<double>& v1, const std::vector<double>& v2)
{
  if(v1.size() == v2.size()) for(int i=0; i<v1.size(); i++) v1[i]+=v2[i];
}

std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2)
{
  std::vector<double> temp(v1.size());
  if(v1.size() == v2.size()) for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] - v2[i];
  else for(int i = 0; i < v1.size(); i++) temp[i] = v1[i];
  return temp;
}

void operator-=(std::vector<double>& v1, const std::vector<double>& v2)
{
  if(v1.size() == v2.size()) for(int i = 0; i < v1.size(); i++) v1[i] -= v2[i];
}

std::vector<double> operator+(const std::vector<double>& v1, const double& s1)
{
  std::vector<double> temp(v1.size());
  for(int i=0; i<v1.size(); i++) temp[i] = v1[i] + s1;
  return temp;
}

std::vector<double> operator+(const double& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] + s1;
  return temp;
}

std::vector<double> operator+(const std::vector<double>& v1, const int& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] + s1;
  return temp;
}

std::vector<double> operator+(const int& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] + s1;
  return temp;
}

std::vector<double> operator-(const std::vector<double>& v1, const double& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] - s1;
  return temp;
}

std::vector<double> operator-(const double& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = s1 - v1[i];
  return temp;
}

std::vector<double> operator-(const std::vector<double>& v1, const int& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] - s1;
  return temp;
}

std::vector<double> operator-(const int& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = s1 - v1[i];
  return temp;
}

std::vector<double> operator*(const std::vector<double>& v1, const double& s1)
{
  std::vector<double> temp(v1.size());
  for(int i=0; i<v1.size(); i++) temp[i] = v1[i]*s1;
  return temp;
}

std::vector<double> operator*(const double& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] * s1;
  return temp;
}

std::vector<double> operator*(const std::vector<double>& v1, const int& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] * s1;
  return temp;
}

std::vector<double> operator*(const int& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] * s1;
  return temp;
}

std::vector<double> operator/(const std::vector<double>& v1, const double& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] / s1;
  return temp;
}

std::vector<double> operator/(const double& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = s1/v1[i];
  return temp;
}

std::vector<double> operator/(const std::vector<double>& v1, const int& s1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = v1[i] / s1;
  return temp;
}

std::vector<double> operator/(const int& s1, const std::vector<double>& v1)
{
  std::vector<double> temp(v1.size());
  for(int i = 0; i < v1.size(); i++) temp[i] = s1 / v1[i];
  return temp;
}

double abs(const std::vector<double>& v1)
{
  double temp = 0;
  for(auto val = v1.begin(); val<v1.end(); val++) temp += pow(*val, 2);
  return sqrt(temp);
}

std::vector<double> acceleration_calc(const int& i, const int& rkf45_step, const std::vector<CelestialBody>& bodies)
{
  std::vector<double> acc(3);
  for(int j=0; j<bodies.size(); j++)
  {
    std::vector<double> displacement = bodies[i].position - bodies[j].position;
    for(int k=0; k<rkf45_step; k++) displacement += tableau[rkf45_step][k] * (bodies[i].position_coeffs[k] - bodies[j].position_coeffs[k]);
    double denominator = pow(abs(displacement), 3);
    
    if(denominator!=0) acc-=G*bodies[j].mass*displacement/denominator;
  }
  //print_vector(acc);
  return acc;
}
std::vector<double> acceleration_calc(const int& i, const int& rkf45_step, const std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc)
{
  std::vector<double> acc(3);
  for(int j = 0; j < bodies.size(); j++)
  {
    std::vector<double> displacement = bodies[i].position - bodies[j].position;
    for(int k = 0; k < rkf45_step; k++) displacement += tableau[rkf45_step][k] * (bodies[i].position_coeffs[k] - bodies[j].position_coeffs[k]);
    double denominator = pow(abs(displacement), 3);

    if(denominator != 0) acc -= G * bodies[j].mass * displacement / denominator;
  }
  acc += const_acc[i];
  return acc;
}

std::vector<double> fourth_order_position(const CelestialBody& body)
{
  return body.position + body.position_coeffs[0]*25/216 + body.position_coeffs[2]*1408/2565 + body.position_coeffs[3]*2197/4101 - body.position_coeffs[4]/5;
}
std::vector<double> fifth_order_position(const CelestialBody& body)
{
  return body.position + body.position_coeffs[0]*16/135 + body.position_coeffs[2]*6656/12825 + body.position_coeffs[3]*28561/56430 - body.position_coeffs[4]*9/50 + body.position_coeffs[5]*2/55;
}

std::vector<double> fourth_order_velocity(const CelestialBody& body)
{
  return body.velocity + body.velocity_coeffs[0] * 25/216 + body.velocity_coeffs[2] * 1408/2565 + body.velocity_coeffs[3] * 2197/4101 - body.velocity_coeffs[4]/5;
}
std::vector<double> fifth_order_velocity(const CelestialBody& body)
{
  return body.velocity + body.velocity_coeffs[0]*16/135 + body.velocity_coeffs[2]*6656/12825 + body.velocity_coeffs[3]*28561/56430 - body.velocity_coeffs[4]*9/50 + body.velocity_coeffs[5]*2/55;
}

void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies)
{
  for(int rkf45_step=0; rkf45_step<6; rkf45_step++) for(int body_index=0; body_index<bodies.size(); body_index++)
  {
    //if(rkf45_step == 0) std::cout << "Body #" << body_index << ":\t[" << bodies[body_index].velocity[0] << ", " << bodies[body_index].velocity[1] << ", " << bodies[body_index].velocity[2] << "]" << std::endl;
    bodies[body_index].position_coeffs[rkf45_step] = delta_t * bodies[body_index].velocity;
    for(int sub_step = 0; sub_step <= rkf45_step; sub_step++) bodies[body_index].position_coeffs[rkf45_step] += delta_t * tableau[rkf45_step][sub_step] * bodies[body_index].velocity_coeffs[sub_step];
    bodies[body_index].velocity_coeffs[rkf45_step] = delta_t * acceleration_calc(body_index, rkf45_step, bodies);
  }
  return;
}

void rkf45_evaluate_coeffs(const double& delta_t, std::vector<CelestialBody>& bodies, const std::vector<std::vector<double>>& const_acc)
{
  for(int rkf45_step = 0; rkf45_step < 6; rkf45_step++) for(int body_index = 0; body_index < bodies.size(); body_index++)
  {
    bodies[body_index].position_coeffs[rkf45_step] = delta_t * bodies[body_index].velocity;
    bodies[body_index].velocity_coeffs[rkf45_step] = delta_t * acceleration_calc(body_index, rkf45_step, bodies, const_acc);
    for(int sub_step = 0; sub_step < rkf45_step; sub_step++) bodies[body_index].position_coeffs[rkf45_step] += delta_t * tableau[rkf45_step][sub_step] * bodies[body_index].velocity_coeffs[sub_step];
  }
  return;
}