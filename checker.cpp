#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "./checker.h"

using namespace std;

double ParticleChecker(double* Particle, int RelevantParticles, float RapidityCut) {
  int pid = Particle[1];
  double p0 = Particle[5];
  double p1 = Particle[2];
  double p2 = Particle[3];
  double p3 = Particle[4];
  double rapidity = 0.5 * log((p0 + p3) / (p0 - p3));

  if (p1 == 0 && p2 == 0) return 0;  // These are spectators, some generators leave them in output files
  if (abs(rapidity) > RapidityCut) return 0;
  switch (RelevantParticles) {
    case 1 :
      if (abs(pid) == 211) return 1;
      else
        return 0;
    case 2 :
      if (abs(pid) == 321) return 1;
      else
        return 0;
    case 3 :
      if (abs(pid) == 2212) return 1;
      else
        return 0;
    default:
      if ((abs(pid) == 211) || (abs(pid) == 321) || (abs(pid) == 2212) ||
        (abs(pid) == 3112) || (abs(pid) == 3222) || (abs(pid) == 3312) ||
        (abs(pid) == 3334) || (abs(pid) == 213)) {
        return 1;
      } else {
        return 0;
      }
  }
}
