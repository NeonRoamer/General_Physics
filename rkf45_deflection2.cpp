//RKF45 method, 
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<fstream>
#include<random>
#include "CelestialBody.h"

const double tolerance = 1e6;
const double time_delta_init = 3600;
const double time_delta_min = 60;
const double time_max = 365 * 24 * 3600;
const int write_interval = 50;
const int max_sims = 5000;
std::string data_file = "deflection_results.txt";

void yax_rotation(std::vector<double>& vec, const double& inc_angle)  //Rotates the frame of asteroid headed to earth at random inclination angle from the z-axis
{
  std::vector<double> temp_vec(3);
  temp_vec[0] = vec[0] * cos(inc_angle) - vec[2] * sin(inc_angle);
  temp_vec[1] = vec[1];
  temp_vec[2] = vec[0] * sin(inc_angle) + vec[2] * cos(inc_angle);
  vec = temp_vec;
  return;
}

int main()
{
  double mass_dart = 610; //kg
  double mass_ceres = 9.39e20; //kg
  double radius_ceres = 4.73e5;
  double init_r = 3.88e9;
  double init_v = init_r / (3600 * 24 * 15); //15 days before impact

  double time_delta;
  double time;
  uint32_t n_loops;
  int n_sims = 0;
  double error;
  bool earth_hit;

  double imparted_theta;
  double imparted_phi;
  double inc_angle;

  std::mt19937 mt{ std::random_device{}() };
  std::uniform_real_distribution rand_angle(0.0, pi);
  std::uniform_real_distribution rand_imp(5e21, 5e22);

  std::vector<CelestialBody> bodies;
  bodies.reserve(3);
  CelestialBody earth(mass_earth, radius_earth, std::vector<double>(3), std::vector<double>(3));
  CelestialBody moon(mass_moon, radius_moon, std::vector<double>{orb_rad_moon, 0, 0}, std::vector<double>{0, orb_vel_moon, 0});
  std::vector<double> asteroid_pos(3);
  std::vector<double> asteroid_vel(3);
  CelestialBody asteroid(mass_ceres, radius_ceres, asteroid_pos, asteroid_vel);

  bodies.push_back(earth);
  bodies.push_back(moon);
  bodies.push_back(asteroid);

  std::ostringstream oss;
  std::ofstream file(data_file, std::ofstream::trunc);
  oss << "Outcome, Magnitude, Theta\n";

  while(n_sims < max_sims)
  {
    imparted_theta = rand_angle(mt) / 2;
    imparted_phi = rand_angle(mt) * 2;

    inc_angle = rand_angle(mt) / 2;
    asteroid_pos = std::vector<double>{ init_r, 0, 0 };
    asteroid_vel = std::vector<double>{ -init_v, 0, 0 };
    double asteroid_impulse_mag{ rand_imp(mt) }; //N

    std::vector<double> asteroid_impulse_vec{ cos(imparted_theta), sin(imparted_theta) * cos(imparted_phi), sin(imparted_theta) * sin(imparted_phi) };
    asteroid_impulse_vec *= asteroid_impulse_mag;
    asteroid_vel = (asteroid_vel * mass_ceres + asteroid_impulse_vec) / (mass_ceres + mass_dart);
    yax_rotation(asteroid_impulse_vec, inc_angle);
    std::vector<std::vector<double>> const_acc{ std::vector<double>(3), std::vector<double>(3), asteroid_impulse_vec / mass_ceres };

    yax_rotation(asteroid_pos, inc_angle);
    yax_rotation(asteroid_vel, inc_angle);
    bodies[2].set_position(asteroid_pos);
    bodies[2].set_velocity(asteroid_vel);

    bodies[0].set_position(std::vector<double>(3));
    bodies[0].set_velocity(std::vector<double>(3));
    bodies[1].set_position(std::vector<double>{orb_rad_moon, 0, 0});
    bodies[1].set_velocity(std::vector<double>{0, orb_vel_moon, 0});

    std::vector<double> momentum(3);
    for(int i = 1; i < bodies.size(); i++) momentum -= bodies[i].get_mass() * bodies[i].get_velocity();
    bodies[0].set_velocity(momentum / bodies[0].get_mass());

    std::string stop_str;
    time = 0;
    time_delta = time_delta_init;

    while(time < time_max)
    {
      n_loops = 0;
      do
      {
        rkf45_evaluate_coeffs(time_delta, bodies);

        error = 0;
        for(int i = 0; i < bodies.size(); i++) error += pow(abs(fifth_order_position(bodies[i]) - fourth_order_position(bodies[i])), 2) + pow(time_delta * abs(fifth_order_velocity(bodies[i]) - fourth_order_velocity(bodies[i])), 2);
        error = sqrt(error);
        time_delta *= 0.87 * pow(tolerance / error, 1 / 5);

        //Uncomment to step through function evaluations
        //std::cout << "Time = " << time << "\tTime delta = " << time_delta << "\tNUMBER OF LOOPS: " << n_loops <<"\tError = " << error << std::endl;
        //std::getline(std::cin, stop_str);
        if(time_delta < time_delta_min) time_delta = time_delta_init;
        n_loops++;
        if(n_loops > 1000) break;
      } while(error > tolerance);

      for(int i = 0; i < bodies.size(); i++)
      {
        bodies[i].set_position(fourth_order_position(bodies[i]));
        bodies[i].set_velocity(fourth_order_velocity(bodies[i]));
      }
      time += time_delta;
      earth_hit = false;
      for(int i = 1; i < bodies.size(); i++) earth_hit |= abs(bodies[0].get_position() - bodies[i].get_position()) <= bodies[0].get_radius() + bodies[i].get_radius();
      if(earth_hit) break;
    }

    if(earth_hit) oss << 1 << ", " << asteroid_impulse_mag << ", " << imparted_theta << '\n';
    else if(time < time_max)
    {
      std::cout << "Simulation stuck, last error = " << error << std::endl;
      continue;
    }
    else oss << 0 << ", " << asteroid_impulse_mag << ", " << imparted_theta << '\n';

    if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
    {
      file << oss.str();
      oss.str("");
      oss.clear();
    }

    //bodies.clear();
    n_sims++;
    std::cout << "Completed sim " << n_sims << '/' << max_sims << std::endl;
  }
  //Readjust momenta

  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();

  return 0;
}