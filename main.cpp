#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <time.h>
#include <iomanip>
#include "./sorter.h"
#include "./params.h"
#include "./rotator.h"
#include "./checker.h"

using namespace std;

int main() {
  if (NoAzibins < 6) {
    cout << "Error! NoAzibins has to be greater than 5" << endl;
    return 1;
  }

  if (NoBins < 6) {
    cout << "Error! NoBins has to be greater than 5" << endl;
    return 1;
  }

  if (RelevantParticles < 0 || RelevantParticles > 3) {
    cout << "RelevantParticles has invalid value. Default value will be used." << endl;
    return 1;
  }

  if (RapidityCut <= 0) {
    cout << "Error! ŔapidityCut has to be greater than 0" << endl;
    return 1;
  }

  if (AnisotropyOrder < 1) {
    cout << "Error! AnisotropyOrder has to be greater than 0" << endl;
    return 1;
  }

  // Allocation of arrays
  double** Event = new double*[NoEvents];
  const int BinIndexZero = NoProperties - NoAzibins;
  for (long i=0; i < NoEvents; i++) {
    Event[i] = new double[NoProperties];
  }
  /*  Properties of each event
      0 NoParticles
      1 size of q_n
      2 original event order number
      3 mu
      4+ Azimuthal bins
  */
  for (long i=0; i < NoEvents; i++) {
    Event[i][0] = 0;
    Event[i][1] = 0;
    for (int j=BinIndexZero; j < NoProperties; j++) {
      Event[i][j] = 0;
    }
  }

  // Load input file
  ostringstream out;
  out << DataDirectory << DataFile;
  string outstring(out.str());
  const char* filename = outstring.c_str();
  ifstream data(filename);
  if (data.fail()) {
    cout << "Error! The file " << filename << " hasn't been found." << endl;
    return 1;
  }

  // Skip first three lines in datafile
  string s;
  getline(data, s);
  getline(data, s);
  getline(data, s);

  // Load events and bin particles
  long EventNumber;
  long NoParticles;
  int BinNumber;
  double EmptyInput;
  double BinWidth = 2*M_PI/NoAzibins;
  double phi;
  double qphi[NoEvents];

  // Loop over all events
  while (!data.eof()) {
    data >> EventNumber;
    if (EventNumber > NoEvents) {
      cout << "Error! Current event number exceeded total number of events." << endl;
      return 1;
    }
    data >> NoParticles;
    data >> EmptyInput;
    data >> EmptyInput;
    EventNumber--;  // Array has numbering from 0, datafiles from 1

    // Create array of particles in event
    double** Particles = new double*[NoParticles];
    for (long i=0; i < NoParticles; i++) {
      Particles[i] = new double[12];
      for (int j=0; j < 11; j++) {
        data >> Particles[i][j];
      }
      Particles[i][11] = ParticleChecker(Particles[i], RelevantParticles, RapidityCut, ptMin, ptMax);
    }

    // Rotate event
    qphi[EventNumber] = Rotate(Particles, NoParticles, AnisotropyOrder);

    // Fill event array
    for (long i=0; i < NoParticles; i++) {
      if (Particles[i][11] == 1) {
        phi = arct(Particles[i][3], Particles[i][2]);
        BinNumber = floor(phi/BinWidth);
        Event[EventNumber][1] += cos(AnisotropyOrder*phi);
        Event[EventNumber][BinNumber + BinIndexZero]++;
        Event[EventNumber][0]++;
        Event[EventNumber][2] = EventNumber;
      }
    }

    Event[EventNumber][1] /= sqrt(Event[EventNumber][0]);
    cout << "Event number " << EventNumber << " loaded with ";
    cout << Event[EventNumber][0] << " particles." << endl;

    for (long i = 0; i < NoParticles; i++) {
      delete[] Particles[i];
    }
    delete [] Particles;
    
    if (EventNumber == NoEvents-1) break;
  }

  // Initial sorting according to value of q_n vector
  cout << "Initial sorting begun." << endl;
  Sort(Event, 0, NoEvents-1, 1, NoProperties);
  cout << "Initial sorting done." << endl;

  cout << "Event shape sorting begun" << endl;

  double* previousSort = new double[NoEvents];
  long iter = 0;
  bool Sorted = false;
  long NoChanges = 0;
  double** P_mu_ni = new double*[NoBins];
  for (long i=0; i < NoBins; i++) {
    P_mu_ni[i] = new double[NoEvents];
  }
  double P_i_mu[NoAzibins][NoBins];

  while (Sorted == false) {
    for (long enr = 0; enr < NoEvents; enr++) {
      previousSort[enr] = Event[enr][2];
    }

    cout << iter << ". cycle begun ";

    for (int mu=0; mu < NoBins; mu++) {
      for (int i=0; i < NoAzibins; i++) {
        double TotalParticles = 0;
        double ParticlesInI = 0;
        for (long enr=mu*NoEvents/NoBins; enr < (mu+1)*NoEvents/NoBins; enr++) {
          ParticlesInI += Event[enr][i + BinIndexZero];
          for (int bnr = 0; bnr < NoAzibins; bnr++) {
            TotalParticles += Event[enr][bnr + BinIndexZero];
          }
        }
        P_i_mu[i][mu] = ParticlesInI/TotalParticles;
      }
    }

    for (long enr=0; enr < NoEvents; enr++) {
      double sum = 0;
      for (int mu=0; mu < NoBins; mu++) {
        long double citatel = 0;
        for (int i=0; i< NoAzibins; i++) {
          citatel += Event[enr][i + BinIndexZero]*log(P_i_mu[i][mu]);
        }

        long double menovatel = 0;
        for (int muu=0; muu < NoBins; muu++) {
          long double clen = 0;
          for (int i=0; i< NoAzibins; i++) {
            clen += Event[enr][i + BinIndexZero]*logl(P_i_mu[i][muu]);
          }
          clen -= citatel;
          menovatel += expl(clen);
        }

        P_mu_ni[mu][enr] = pow(menovatel, -1);
        sum += P_mu_ni[mu][enr];
      }

      Event[enr][3] = 0;
      for (int mu=0; mu < NoBins; mu++) {
        Event[enr][3] += (mu+1)*P_mu_ni[mu][enr];
      }
    }

    Sort(Event, 0, NoEvents-1, 3, NoProperties);

    Sorted = true;
    NoChanges = 0;
    for (long enr = 0; enr < NoEvents; enr++) {
      if (previousSort[enr] != Event[enr][2]) {
        Sorted = false;
        NoChanges++;
      }
    }

    cout << NoChanges << " changes made." << endl;
    iter++;
    if (iter > MaxNoCycles) {
      cout << "Maximum number of cycles reached." << endl;
      break;
    }
  }

  cout << "Event shape sorting done." << endl;

  // Taking time for file names
  time_t t = time(0);
  struct tm* now = localtime(&t);

  // Create result files
  ofstream file;
  ostringstream outDir;
  outDir << "./results/" << fixed << setfill('0') << setw(2);
  outDir << now->tm_year - 100 << setw(2) << now->tm_mon + 1 << setw(2);
  outDir << now->tm_mday << "_" << setw(2) << now->tm_hour << setw(2);
  outDir << now->tm_min << setw(2) << now->tm_sec << "_";
  string outstringdir(outDir.str());
  const char* outdir = outstringdir.c_str();

  ostringstream outFile1;
  outFile1 << outdir << "av_bin";
  string outstring1(outFile1.str());
  const char* filename1 = outstring1.c_str();
  file.open(filename1);
  for (int i=0; i < NoAzibins; i++) {
    file << BinWidth*(2*i+1)*0.5 << "\t";
    for (int mu=0; mu < NoBins; mu++) {
      double ParticlesInI = 0;
      for (long enr=mu*NoEvents/NoBins; enr < (mu+1)*NoEvents/NoBins; enr++) {
        ParticlesInI += Event[enr][i + BinIndexZero];
      }
      file << floor(ParticlesInI*NoBins/NoEvents) << "\t";
    }
    file << endl;
  }
  file.close();

  ostringstream outFile2;
  outFile2 << outdir << "id_events";
  string outstring2(outFile2.str());
  const char* filename2 = outstring2.c_str();
  file.open(filename2);
  for (long enr=0; enr < NoEvents; enr++) {
    file << Event[enr][2] << endl;
  }
  file.close();

  for (int mu=0; mu < NoBins; mu++) {
    ostringstream outFile3;
    outFile3 << outdir << "bin_" << mu+1;
    string outstring3(outFile3.str());
    const char* filename3 = outstring3.c_str();
    file.open(filename3);
    for (long enr=mu*NoEvents/NoBins; enr < (mu+1)*NoEvents/NoBins; enr++) {
      file << enr - mu*NoEvents/NoBins + 1 << "\t";
      for (int i=0; i < NoAzibins; i++) {
        file << Event[enr][i + BinIndexZero] << "\t";
      }
      file << Event[enr][3] << endl;
    }
    file.close();
  }

  ostringstream outFile4;
  outFile4 << outdir << "rotations";
  string outstring4(outFile4.str());
  const char* filename4 = outstring4.c_str();
  file.open(filename4);
  for (long enr=0; enr < NoEvents; enr++) {
    file << enr << "\t" << qphi[enr] << endl;
  }
  file.close();

  return 0;
}
