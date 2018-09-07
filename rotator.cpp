#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "./rotator.h"

using namespace std;

// Function atan2 returns angle in interval (-pi;pi), but we need result in interval (0;2pi)
double arct(double y, double x) {
  double result = atan2(y, x);
  if (result < 0) return result + 2*M_PI;
  return result;
}

void Rotate(double** Particles, long NoParticles, int AnisotropyOrder) {
  double qx = 0;
  double qy = 0;
  for (long ind = 0; ind < NoParticles; ind++) {
    if (Particles[ind][11] == 1) {
      double p1 = Particles[ind][2];
      double p2 = Particles[ind][3];
      double phi = arct(p2, p1);
      qx+=cos(AnisotropyOrder*phi);
      qy+=sin(AnisotropyOrder*phi);
    }
  }

  // Now we calculate direction of Q vector
  double qphi = arct(qy, qx);
  for (long ind = 0; ind < NoParticles; ind++) {
    // Rotation of all momenta
    double px = Particles[ind][2] * cos(qphi / AnisotropyOrder) + Particles[ind][3] * sin(qphi / AnisotropyOrder);
    double py = - Particles[ind][2] * sin(qphi / AnisotropyOrder) + Particles[ind][3] * cos(qphi / AnisotropyOrder);
    Particles[ind][2] = px;
    Particles[ind][3] = py;

    // Rotation of positions of particles
    double x = Particles[ind][7] * cos(qphi / AnisotropyOrder) + Particles[ind][8] * sin(qphi / AnisotropyOrder);
    double y = - Particles[ind][7] * sin(qphi / AnisotropyOrder) + Particles[ind][8] * cos(qphi / AnisotropyOrder);
    Particles[ind][7] = x;
    Particles[ind][8] = y;
  }
}
