//Testing magnitude and direction of deflection
//Sets moon orbit in xy plane, and rotates the incoming asteroid by a random inclination angle
//Assumes elastic collision between impactor and asteroid
//Impulse is delivered at a random theta and phi relative to asteroid
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<fstream>
#include<random>
#include "CelestialBody.h"

//simulation parameters
const std::string data_file = "deflection_results.txt"
const int max_sims = 500;  //Total number of results written to file;
const int max_trials = 1000;  //Max number of trials allowed to find errors below tolerance
const double time_max = 365 * 24 * 3600;  //Total time to simulate
const double tolerance = 1e15;
const double time_delta_init = 3600;  //s
const double time_delta_min = 60; //s
const double max_impulse = 1e24;  //Ns
const double min_impulse = 1e20;  //Ns

//asteroid parameters
const double mass_ceres = 9.39e20; //kg
const double mass_dart = 6.6e3; //kg - Mass of probe used in deflection
double radius_ceres = 4.73e5; //m
double init_r = 3.88e9; //m
double init_v = init_r / (3600 * 24 * 15); //15 days before impact

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
  //Declare parameters
  double time;
  double time_delta;

  std::vector<CelestialBody> bodies;
  bodies.emplace_back(mass_earth, radius_earth, std::vector<double>(3), std::vector<double>(3));
  bodies.emplace_back(mass_moon, radius_moon, std::vector<double>{orb_rad_moon, 0, 0}, std::vector<double>{0, orb_vel_moon, 0});
  std::vector<double> asteroid_position{ init_r, 0, 0 };
  std::vector<double> asteroid_velocity{ -init_v, 0, 0 };
  bodies.emplace_back(mass_ceres, radius_ceres, asteroid_position, asteroid_velocity);

  std::mt19937 mt{std::random_device{}()};
  std::uniform_real_distribution rand_angle(0.0, 2*pi);
  double inc_angle; // 0 to pi/2
  double imparted_theta; // 0 to pi
  double imparted_phi; //0 to 2*pi
  std::uniform_real_distribution rand_impulse(min_impulse, max_impulse); // May change this to logarithmic
  double asteroid_impulse_mag;
  std::vector<double> asteroid_impulse_vec(3);

  int n_sims = 0; //Total number of simulations
  int n_trials; //Number of evaluations for next step in integration
  double error;
  bool earth_hit;
  bool stuck = false;
  std::ostringstream oss;
  std::ofstream file(data_file, std::ofstream::trunc);
  oss<<"Outcome\tInclination\tImpulse mag\tImpulse theta\tImpulse phi\n";

  while(n_sims<max_sims)
  {
    //Reset model
    inc_angle = rand_angle()/4;
    imparted_theta = rand_angle(mt)/2;
    imparted_phi = rand_angle(mt);
    
    asteroid_impulse_vec{ cos(imparted_theta), sin(imparted_theta) * cos(imparted_phi), sin(imparted_theta) * sin(imparted_phi) };
    asteroid_impulse_mag = rand_impulse();
    asteroid_impulse_vec*=asteroid_impulse_mag;
    asteroid_position{init_r, 0, 0};
    asteroid_velocity{-init_v, 0, 0};
    asteroid_velocity = (asteroid_velocity*mass_ceres + asteroid_impulse_vec)/(mass_ceres + mass_dart);
    yax_rotation(asteroid_velocity, inc_angle);
    yax_rotation(asteroid_position, inc_angle);

    bodies[0].set_position(std::vector<double>(3));
    bodies[0].set_velocity(std::vector<double>(3));
    bodies[1].set_position(std::vector<double>{orb_rad_moon, 0, 0});
    bodies[1].set_velocity(std::vector<double>{0, orb_vel_moon, 0});
    bodies[2].set_position(asteroid_position);
    bodies[2].set_velocity(asteroid_velocity);

    earth_hit = false;
    time = 0;
    time_delta = time_delta_init;
    //Run simulation
    while(time<time_max)
    {
      n_trials = 0;
      do
      {
        rkf45_evaluate_coeffs(time_delta, bodies);

        error = 0;
        for(int i = 0; i < bodies.size(); i++) error += pow(abs(fifth_order_position(bodies[i]) - fourth_order_position(bodies[i])), 2) + pow(time_delta * abs(fifth_order_velocity(bodies[i]) - fourth_order_velocity(bodies[i])), 2);
        error = sqrt(error);

        time_delta *= 0.87 * pow(tolerance / error, 1 / 5);        
        //Uncomment to step through function evaluations
        //std::cout << "Time = " << time << "\tTime delta = " << time_delta << "\tNUMBER OF LOOPS: " << n_trials <<"\tError = " << error << std::endl;
        //std::getline(std::cin, stop_str);
        if(time_delta < time_delta_min) time_delta = time_delta_init;

        for(auto body = bodies.begin()+1; body<bodies.end(); body++) earth_hit |= abs(bodies[0].get_position() - body->get_position()) <= bodies[0].get_radius() + body->get_radius();

        n_trials++;
        if(n_trials>=max_trials || ) break;
      }
      while(error>tolerance);
      
      if(n_trials>=max_trials)
      {
        if(earth_hit) oss<< ;
        else stuck = true;
        break;
      }
    }

    //Log results and continue(?)
    n_sims++;
    if(stuck)
    {
      std::cout<<"Simulation stuck, last error = "<<error<<std::endl;
      break;
    }
    else if(earth_hit) oss << 1 << '\t' << std::round(inc_angle) << '\t' << std::round(asteroid_impulse_mag) << '\t' << std::round(imparted_theta) << '\t' << std::round(imparted_phi) << '\n';
    else oss << 0 << '\t' << std::round(inc_angle) << '\t' << std::round(asteroid_impulse_mag) << '\t' << std::round(imparted_theta) << '\t' << std::round(imparted_phi) << '\n';
    
    if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
    {
      file << oss.str();
      oss.str("");
      oss.clear();
    }
    std::cout<<"Completed sim "<<n_sims<<'/'<<max_sims<<std::endl;

  }
  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();
  return 0;
}