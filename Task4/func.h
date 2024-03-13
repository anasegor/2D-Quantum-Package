#include <iostream>
#include <vector>
#include <ccomplex>
#include <cmath>
#include <algorithm>


extern long double pi;
typedef std::complex<long double> complexld;
extern complexld I;


std::vector<complexld> Double2Cmplx(std::vector<long double> vec);
std::vector<long double> Cmplx2Double(std::vector<complexld> vec);
std::vector<double> Cmplx2Doublee(std::vector<complexld> vec);
std::vector<std::vector<long double>> Cmplx2Double(std::vector<std::vector<complexld>> vec);
std::vector<std::vector<long double>> CreateWaveFunction(long double R, long double muX, long double sigmaX, long double muY, long double sigmaY, int N);
void CalculateWaveEvolution(std::vector <std::vector<long double>> wave0,std::vector<std::vector<std::vector<complexld>>>& out, long double R, int num_waves, long double dt, bool& thEvolutionDone, long double a, long double V0, long double b, std::vector<std::vector<std::vector<long double>>>& out_modul);
void fourea(long v_size, std::vector<complexld>& F, double type);
void fourea(std::vector< std::vector<complexld>>& in);
