//RKF45 method, 
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<fstream>
#include<chrono>
#include<random>
#include "CelestialBody.h"

const double time_max = 365*24*3600;
const int write_interval = 50;
std::string data_file = "celestial_object_simulation.txt";
std::string diagnotic_file = "celestial_object_diagnostics.txt";

void yax_rotation(std::vector<double>& vec, const double& inc_angle)  //Rotates the frame of asteroid headed to earth at random inclination angle from the z-axis
{
  std::vector<double> temp_vec(3);
  temp_vec[0] = vec[0]*cos(inc_angle) - vec[2]*sin(inc_angle);
  temp_vec[1] = vec[1];
  temp_vec[2] = vec[0]*sin(inc_angle) + vec[2]*cos(inc_angle);
  vec = temp_vec;
  return;
}

double total_energy(const std::vector<CelestialBody>& bodies)
{
  double distance;
  double tot = 0;
  for(auto body=bodies.begin(); body<bodies.end(); body++)
  {
    tot+=0.5 * body->get_mass() * pow(abs(body->get_velocity()), 2);
    for(auto other_body=body+1; other_body<bodies.end(); other_body++)
    {
      distance = abs(body->get_position() - other_body->get_position());
      if(distance>0) tot -= G*body->get_mass()*other_body->get_mass()/distance;
    }
  }
  return tot;
}

std::vector<double> total_momentum(const std::vector<CelestialBody>& bodies)
{
  std::vector<double> tot(3);
  for(auto body=bodies.begin(); body<bodies.end(); body++) tot+=body->get_mass()*body->get_velocity();
  return tot;
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<CelestialBody> bodies;
  CelestialBody earth(mass_earth, radius_earth, std::vector<double>(3), std::vector<double>(3));
  CelestialBody moon(mass_moon, radius_moon, std::vector<double>{orb_rad_moon, 0, 0}, std::vector<double>{0, orb_vel_moon, 0});
  bodies.push_back(earth);
  bodies.push_back(moon);
  
  //Make asteroid
  double mass_ceres = 9.39e20; //kg
  double radius_ceres = 4.73e5;
  double init_r = 3.88e9;
  double init_v = init_r/(3600*24*15); //15 days before impact
  //double init_v = init_r / (3600 * 24 * 365 * 10); //15 days before impact

  //Generate asteroid's random incoming angle
  std::mt19937 mt{ std::random_device{}() };
  std::uniform_real_distribution rand_angle( 0.0, pi );

  double inc_angle{rand_angle(mt)/2};
  //inc_angle = pi/2;
  std::vector<double> asteroid_pos{init_r, 0, 0};
  std::vector<double> asteroid_vel{-init_v, 0, 0};
  
  double imparted_theta = 0, imparted_phi = pi/2; //Polar angles of applied force/impulse relative to initial displacement to asteroid
  
  //Kinetic impactor
  //*
  double mass_dart = 610, speed_dart = 6.6e3; //kg, ms^-1
  double asteroid_impulse_mag = mass_dart * speed_dart; // Ns, impulse delivered by Dart probe
  std::vector<double> asteroid_impulse_vec{cos(imparted_theta), sin(imparted_theta)*cos(imparted_phi), sin(imparted_theta)*sin(imparted_phi)};  //Before random rotation about y axis
  asteroid_impulse_mag = 4e24;
  asteroid_impulse_vec*=asteroid_impulse_mag;
  //asteroid_impulse_vec*=0;
  asteroid_vel = (asteroid_vel*mass_ceres + asteroid_impulse_vec) / (mass_ceres + mass_dart);
  //*/
  
  //Gravity tractor - Add const_acc at end of evaluate coeffs function - it's overloaded
  /*
  double asteroid_force_mag = 10e16; //N  - Average tractor force found on wikipedia = 0.032N, but would be applied over 10 years
  std::vector<double> asteroid_force_vec{ cos(imparted_theta), sin(imparted_theta) * cos(imparted_phi), sin(imparted_theta) * sin(imparted_phi) };
  asteroid_force_vec*=asteroid_force_mag;
  yax_rotation(asteroid_force_vec, inc_angle);
  std::vector<std::vector<double>> const_acc{std::vector<double>(3), std::vector<double>(3), asteroid_force_vec/mass_ceres};  //No constant accelerations on earth or moon, assume intact asteroid
  //*/
  
  //Randomly rotate asteroid headed straight to earth about y axis
  //inc_angle = 0;
  yax_rotation(asteroid_pos, inc_angle);
  yax_rotation(asteroid_vel, inc_angle);
  std::cout<<"Inclination angle = "<<inc_angle*180/pi<<std::endl;

  //Comment out if exploding
  CelestialBody asteroid(mass_ceres, radius_ceres, asteroid_pos, asteroid_vel);
  bodies.push_back(asteroid);
  
  //Explosion
  /*
  std::uniform_real_distribution rand_frac(0.0, 1.0);
  double nuclear_yield = 2.1e17; //J, Energy supplied to explosion of largest ever nuclear bomb
  int n_fragments = 2;

  std::vector<double> fragment_masses(n_fragments);
  for(int frag=0; frag<n_fragments-1; frag++)
  {
    fragment_masses[frag] = rand_frac(mt);
    for(int i=0; i<frag; i++) fragment_masses[frag] -= fragment_masses[i];
  }
  for(int i=0; i<n_fragments-1; i++) fragment_masses[n_fragments-1] -= fragment_masses[i];
  fragment_masses *= mass_ceres;

  std::vector<std::vector<double>> fragment_momenta( n_fragments, std::vector<double>(3) );
  for(int dim=0; dim<3; dim++)
  {
    double test_momentum = 0;
    for(int frag = 0; frag < n_fragments; frag++)
    {
      fragment_momenta[frag][dim] = rand_frac(mt);
      test_momentum += fragment_momenta[frag][dim];
    }
    for(int frag = 0; frag < n_fragments; frag++) fragment_momenta[frag][dim] -= test_momentum;
  }
  double test_KE = 0;
  for(int frag = 0; frag < n_fragments; frag++) test_KE += pow(abs(fragment_momenta[frag]),2)/fragment_masses[frag];
  for(int frag = 0; frag < n_fragments; frag++)
  {
    fragment_momenta[frag] *= sqrt(nuclear_yield / test_KE);
    bodies.push_back(CelestialBody(fragment_masses[frag], radius_ceres * pow(fragment_masses[frag] / mass_ceres, 1 / 3), asteroid_pos, asteroid_vel + fragment_momenta[frag] / fragment_masses[frag]));
  }
  //*/
  //Readjust momenta
  std::vector<double> momentum(3);
  for(int i=1; i<bodies.size(); i++) momentum -= bodies[i].get_mass() * bodies[i].get_velocity();
  bodies[0].set_velocity(momentum/bodies[0].get_mass());

  for(auto body = bodies.begin(); body<bodies.end(); body++)
  {
    std::cout<<"Body #"<<body-bodies.begin()<<std::endl;
    for(int i=0; i<3; i++) std::cout<<"e"<<i<<": "<<body->get_position()[i]<<", v"<<i << ": " <<body->get_velocity()[i]<<std::endl;
  }

  const double tolerance = 1e6;
  const double time_delta_init = 3600;
  const double time_delta_min = 60;

  double time_delta = time_delta_init;
  double time = 0;
  uint32_t n_loops = 0;
  double error;
  bool earth_hit = false;
  std::ostringstream oss;
  std::ostringstream oss_diag;
  std::ofstream file(data_file, std::ofstream::trunc);
  std::ofstream diag(diagnotic_file, std::ofstream::trunc);
  double system_energy;
  std::vector<double> system_momentum(3);

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

      system_energy = total_energy(bodies);
      system_momentum = total_momentum(bodies);
      oss_diag << std::round(system_energy) << ", ";
      for(int i = 0; i < 3; i++) oss_diag << std::round(system_momentum[i]) << ", ";
      oss_diag << std::round(error) << ", ";
      oss_diag << std::round(time_delta) << '\n';

      if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
      {
        file << oss.str();
        oss.str("");
        oss.clear();

        diag << oss_diag.str();
        oss_diag.str("");
        oss_diag.clear();
      }
    }
  }

  while(time<time_max)
  {
    std::string stop_str;
    do
    {
      //rkf45_evaluate_coeffs(time_delta, bodies, const_acc);
      rkf45_evaluate_coeffs(time_delta, bodies);

      error = 0;
      for(int i=0; i<bodies.size(); i++) error += pow(abs(fifth_order_position(bodies[i]) - fourth_order_position(bodies[i])), 2) + pow(time_delta*abs(fifth_order_velocity(bodies[i]) - fourth_order_velocity(bodies[i])), 2);
      error = sqrt(error);
      time_delta *= 0.87*pow(tolerance/error, 1/5);

      //Uncomment to step through function evaluations
      //std::cout << "Time = " << time << "\tTime delta = " << time_delta << "\tNUMBER OF LOOPS: " << n_loops <<"\tError = " << error << std::endl;
      //std::getline(std::cin, stop_str);
      if(time_delta<time_delta_min) time_delta=time_delta_init;
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

          system_energy = total_energy(bodies);
          system_momentum = total_momentum(bodies);
          oss_diag << std::round(system_energy) << ", ";
          for(int i = 0; i < 3; i++) oss_diag << std::round(system_momentum[i]) << ", ";
          oss_diag << std::round(error)  << ", ";
          oss_diag << std::round(time_delta) << '\n';

          if(oss.rdbuf()->pubseekoff(0, std::ios::end, std::ios::in) > (5.0 * 1024 * 1024))
          {
            file << oss.str();
            oss.str("");
            oss.clear();
            diag << oss_diag.str();
            oss_diag.str("");
            oss_diag.clear();
          }
        }
      }
    }
    n_loops++;
    time+=time_delta;
    earth_hit = false;  //Remove in normal operation
    for(int i = 1; i < bodies.size(); i++) earth_hit |= abs(bodies[0].get_position() - bodies[i].get_position()) <= bodies[0].get_radius() + bodies[i].get_radius();
    //if(earth_hit) break;
    if(earth_hit) std::cout<<"Earth hit in loop number: "<<n_loops<<std::endl;
  }
  file << oss.str();
  oss.str("");
  oss.clear();
  file.close();
  diag << oss_diag.str();
  oss_diag.str("");
  oss_diag.clear();
  diag.clear();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Elapsed time: " << duration.count() / 1000 << " seconds" << std::endl;
  std::cout << "Simulated time: " << time << '/'<<time_max<<" seconds"<<std::endl;
  std::cout << "No. loops: " << n_loops << std::endl;
  std::cout<<"Set values:\n\tTolerance = "<<tolerance<<"\n\tInitial time step = "<<time_delta_init<<"\n\tMinimum time step = "<<time_delta_min<<std::endl;
  if(earth_hit) std::cout<<"We're all going to die!"<<std::endl;
  return 0;
}