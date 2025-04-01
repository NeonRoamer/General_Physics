//RKF45 method, 
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<fstream>
#include<chrono>
#include "CelestialBody.h"

const double time_max = 365*24*3600;
int write_interval = 100;

std::string data_file = "celestial_object_simulation.txt";

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<CelestialBody> bodies;
  CelestialBody earth(mass_earth, radius_earth, std::vector<double>(3), std::vector<double>(3));
  CelestialBody moon(mass_moon, radius_moon, std::vector<double>{orb_rad_moon, 0, 0}, std::vector<double>{0, orb_vel_moon, 0});
  double init_r = 13.7e9;
  CelestialBody asteroid(1.75e21, 1e6, std::vector<double>{init_r, 0, 0}, std::vector<double>{0, 0.5 * sqrt(G * mass_earth / init_r), 0});
  
  bodies.push_back(earth);
  bodies.push_back(moon);
  bodies.push_back(asteroid);

  double time = 0;
  double time_delta = 3600;
  uint32_t n_loops = 0;
  double error;
  double tolerance = 10000;// 2*bodies.size()*radius_earth;
  std::ostringstream oss;
  std::ofstream file(data_file, std::ofstream::trunc);

  for(int i = 0; i < bodies.size(); i++)
  {    
    oss << std::round(bodies[i].get_posx()) << ", ";
    oss << std::round(bodies[i].get_posy()) << ", ";
    oss << std::round(bodies[i].get_posz()) << ", ";
    oss << std::round(bodies[i].get_radius());
    if(i != bodies.size() - 1) oss << ", ";
    else
    {
      oss << '\n';
      if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
      {
        file << oss.str();
        oss.str("");
        oss.clear();
      }
    }
  }

  while(time<time_max)
  {
    std::string stop_str;
    do
    {
      rkf45_evaluate_coeffs(time_delta, bodies);

      error = 0;
      for(int i=0; i<bodies.size(); i++) error += pow(abs(fifth_order_position(bodies[i]) - fourth_order_position(bodies[i])), 2) + pow(abs(fifth_order_velocity(bodies[i]) - fourth_order_velocity(bodies[i])), 2);
      error = sqrt(error);
      time_delta *= 0.9*pow(tolerance/error, 1/5);

      //Uncomment to step through function evaluations
      //std::cout << "Time = " << time << "\tTime delta = " << time_delta << "\tNUMBER OF LOOPS: " << n_loops <<"\tError = " << error << std::endl;
      //std::getline(std::cin, stop_str);
      if(time_delta<500) time_delta=3600;
    }
    while(error>tolerance);
    for(int i=0; i<bodies.size(); i++)
    {

      bodies[i].set_position(fourth_order_position(bodies[i]));
      bodies[i].set_velocity(fourth_order_velocity(bodies[i]));

      if(n_loops%write_interval == 0)
      {
        oss<<std::round(bodies[i].get_posx())<<", ";
        oss<<std::round(bodies[i].get_posy()) << ", ";
        oss<<std::round(bodies[i].get_posz()) << ", ";
        oss<<std::round(bodies[i].get_radius());
        if(i!=bodies.size()-1) oss<<", ";
        else
        {
          oss<<'\n';
          if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
          {
            file << oss.str();
            oss.str("");
            oss.clear();
          }
        }
      }
    }
    n_loops++;
    time+=time_delta;
  }
  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time: " << duration.count() / 1000 << " seconds" << std::endl;
  std::cout << "Simulated time: " << time << '/'<<time_max<<" seconds"<<std::endl;
  std::cout << "No. loops: " << n_loops << std::endl;
  return 0;
}