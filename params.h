/* 
Here is the part of changeable parameters of program
*/
const int64_t NoEvents = 10000;  // Number of events
const int NoAzibins = 20;  // Number of azimuthal angle bins (default: 20)
const int NoBins = 10;  // Number of classes (default: 10)

const int RelevantParticles = 0;  /* Here you can choose, which particles will be relevant for rotation and sorting of events (default: 0):
                                      0: all charged particles (pions, kaons, protons, sigmas, xis, omegas, rhos)
                                      1: charged pions
                                      2: charged kaons
                                      3: protons */
const float RapidityCut = 1.0;  // Rapidity cut on particles which are relevant (default: 1.0)
const int AnisotropyOrder = 2;  // Anisotropy order, according to which events will be rotated (default: 2)
const float ptMin = 0.0;  // p_t cut in GeV (default: 0.0)
const float ptMax = 10.0;  // (default: 10.0)
const int64_t MaxNoCycles = 1000;  // Maximum number of cycles (default: 1000)

const char* DataDirectory = "./";  // Directory of your datafile relative to this directory (default: "./")
const char* DataFile = "GENERATOR_events.out";  // Name of the file (default: "events.out")

/*
Here are other parameters, that should't be changed by users
*/
const int NoProperties = NoAzibins + 4; // Number of properties of each event. Don't change!
