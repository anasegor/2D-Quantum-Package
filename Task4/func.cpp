
#include "pch.h"

long double pi = 3.141592653589793238;
complexld I(0., 1.);
long double Pow2(long double z)
{
	return z * z;
}

long double Gauss(long double x,long double y, long double muX, long double sigmaX, long double muY, long double sigmaY)
{
	return exp(-Pow2((x - muX) / (2 * sigmaX))) * exp(-Pow2((y - muY) / (2 * sigmaY)));

}


std::vector < std::vector<long double>> CreateWaveFunction(long double R, long double muX, long double sigmaX, long double muY, long double sigmaY, int N)
{
	long double step = 2 * R / N;
	std::vector<std::vector<long double>> masPsi(N, std::vector<long double>(N));
	for (int i = 0; i < N; i++)
	{
		long double x = -R + i * step;
		for (int j = 0; j < N; j++)
		{
			long double y = -R + j * step;
			masPsi[i][j] = Gauss(x, y, muX, sigmaX, muY, sigmaY);
		}
	}
	return masPsi;
}

long double U(long double x, long double y, long double a, long double V0, long double b)
{
	if ((y > -a) && (y < a)) return -V0 / (Pow2((exp(b * x) + exp(-b * x)) / 2.));
	else return 0;
}

template <typename T>
int sign(T x)
{
	if (x > 0) return 1;
	else if (x == 0) return 0;
	else return -1;
}
std::vector<complexld> Double2Cmplx(std::vector<long double> vec)
{
	std::vector<complexld> out(vec.size());
	for (int i = 0; i < vec.size(); i++)
		out[i] = complexld((long double)vec[i], 0);
	return out;
}
std::vector<std::vector<complexld>> Double2Cmplx(std::vector<std::vector<long double>> vec)
{
	std::vector<std::vector<complexld>> out(vec.size());
	for (int i = 0; i < vec.size(); i++)
		out[i] = Double2Cmplx(vec[i]);
	return out;
}
std::vector<long double> Cmplx2Double(std::vector<complexld> vec)
{
	std::vector<long double> out;
	for (int i = 0; i < vec.size(); i++)
		out.push_back(abs(vec[i]));
	return out;
}
std::vector<double> Cmplx2Doublee(std::vector<complexld> vec)
{
	std::vector<double> out;
	for (int i = 0; i < vec.size(); i++)
		out.push_back(abs(vec[i]));
	return out;
}
std::vector<std::vector<long double>> Cmplx2Double(std::vector<std::vector<complexld>> vec)
{
	std::vector<std::vector<long double>> out;
	for (int i = 0; i < vec.size(); i++)
		out.push_back(Cmplx2Double(vec[i]));
	return out;
}

std::vector <std::vector<complexld>> WaveInNextTime(long double R,std::vector <std::vector<complexld>> wave, long double dt, long double a, long double V0, long double b)
{
	long double step = 2 * R/ wave.size();
	std::vector <std::vector<complexld>> Psi = wave;
	for (int y = 0; y < Psi.size(); y++)
	{
		Psi[Psi.size() - 1][y] = 0;
		Psi[0][y] = 0;
	}

	for (int x = 0; x < Psi.size(); x++)
	{
		Psi[x][Psi.size() - 1] = 0;
		Psi[x][0] = 0;
	}
	std::vector<complexld> A(wave.size()), B(wave.size()), C(wave.size()), D(wave.size());
	std::vector<complexld> alfa(wave.size()), betta(wave.size());
	for (int y = 1; y < wave.size() - 1; y++)
	{
		 complexld AA = -(I * dt) / (2 * step * step);
		 complexld derivY = ((wave[0][y + 1] - wave[0][y]) - (wave[0][y] - wave[0][y - 1])) / (step * step);
		for (int x = 1; x < wave.size() - 1; x++)
		{
			A[x] = AA;
			B[x] = AA;
			C[x] = (long double)1 + I * dt * U(-R + step * x, -R + step * y, a, V0, b) * (long double)0.5 - (long double)2. * AA;
			complexld derivX = ((wave[x + 1][y] - wave[x][y]) - (wave[x][y] - wave[x - 1][y])) / (step * step);
			D[x] = wave[x][y] + I * dt * (long double)0.5 * (derivX + (long double)2 * derivY - U(-R + step * x, -R + step * y, a, V0, b) * wave[x][y]);
			
		}
		alfa[1] = 0;
		betta[1] = 0;
		for (int x = 1; x < wave.size() - 1; x++)
		{
			alfa[x + 1] = -B[x] / (C[x] + A[x] * alfa[x]);
			betta[x + 1] = (D[x] - A[x] * betta[x]) / (C[x] + A[x] * alfa[x]);
		}
		Psi[Psi.size() - 1][y] = 0;
		Psi[0][y] = 0;
		for (int x = Psi.size() - 1; x > 0; x--)
		{
			Psi[x-1][y] = alfa[x] * Psi[x][y] + betta[x];
		}
	}
	wave = Psi;
	for (int y = 0; y < Psi.size(); y++)
	{
		Psi[Psi.size() - 1][y] = 0;
		Psi[0][y] = 0;
	}

	for (int x = 0; x < Psi.size(); x++)
	{
		Psi[x][Psi.size() - 1] = 0;
		Psi[x][0] = 0;
	}
	for (int x = 1; x < wave.size() - 1; x++)
	{
		complexld AA = -(I * dt) / (2. * step * step);
		complexld derivX = ((wave[x + 1][0] - wave[x][0]) - (wave[x][0] - wave[x - 1][0])) / (step * step);
		for (int y = 1; y < wave.size() - 1; y++)
		{
			A[y] = AA;
			B[y] = AA;
			C[y] = (long double)1 + I * dt * U(-R + step * x, -R + step * y, a, V0, b) * (long double)0.5 - (long double)2. * AA;
			complexld derivY = ((wave[x][y + 1] - wave[x][y]) - (wave[x][y] - wave[x][y - 1])) / (step * step);
			D[y] = wave[x][y] + I * dt * (long double)0.5 * ((long double)2 * derivX + derivY - U(-R + step * x, -R + step * y, a, V0, b) * wave[x][y]);
			
		}
		alfa[1] = 0;
		betta[1] = 0;
		for (int y = 1; y < wave.size() - 1; y++)
		{
			alfa[y + 1] = -B[y] / (C[y] + A[y] * alfa[y]);
			betta[y + 1] = (D[y] - A[y] * betta[y]) / (C[y] + A[y] * alfa[y]);
		}

		Psi[x][Psi.size() - 1] = 0;
		Psi[x][0] = 0;
		for (int y = Psi.size() - 1; y >0; y--)
		{
			Psi[x][y-1] = alfa[y] * Psi[x][y] + betta[y];
		}
	}
	return Psi;
}

void CalculateWaveEvolution(std::vector <std::vector<long double>> wave0, std::vector<std::vector<std::vector<complexld>>> & out, long double R, int num_waves, long double dt, bool& thEvolutionDone, long double a, long double V0, long double b, std::vector<std::vector<std::vector<long double>>>& out_modul)
{
	std::vector<std::vector<complexld>> waveCmplx = Double2Cmplx(wave0);
	out.clear();
	out.push_back(waveCmplx);
	out_modul.push_back(Cmplx2Double(waveCmplx));
	for (int i = 0; i < num_waves - 1; i++)
	{
		waveCmplx = WaveInNextTime(R, waveCmplx, dt, a, V0,b);
		out.push_back(waveCmplx);
		out_modul.push_back(Cmplx2Double(waveCmplx));
	}
	thEvolutionDone = true;
}

// Быстрое фурье преобразование
			// type = -1 - прямое преобразование
			// type = 1 - обратное преобразование
void fourea(long v_size, std::vector<complexld>& F, double type)
{
	complexld temp, w, c;
	long i(0), i1(0), j(1), j1(0), istep(0);
	long m(0), mmax(1);
	long n = v_size;
	double fn, r1(0), theta(0);
	fn = (double)n;
	double r = pi * type;
	for (i = 1; i <= n; i++)
	{
		i1 = i - 1;
		if (i < j)
		{
			j1 = j - 1;
			temp = F[j1];
			F[j1] = F[i1];
			F[i1] = temp;
		}
		m = n / 2;
		while (j > m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	while (mmax < n)
	{
		istep = 2 * mmax;
		r1 = r / (double)mmax;
		for (m = 1; m <= mmax; m++)
		{
			theta = r1 * (double)(m - 1);
			w = complexld(cos(theta), sin(theta));
			for (i = m - 1; i < n; i += istep)
			{
				j = i + mmax;
				c = F[j];
				temp = w * c;
				F[j] = F[i] - temp;
				F[i] = F[i] + temp;
			}
		}
		mmax = istep;
	}
	if (type > 0)
		for (i = 0; i < n; i++)
		{
			complexld buf(F[i].real() / fn, F[i].imag() / fn);
			F[i] = buf;
		}
	return;
}

//fft для двухмерной волновой функции
void fourea(std::vector< std::vector<complexld>>& in)
{
	for (int i = 0; i < in.size(); i++)
		fourea(in[i].size(), in[i],-1);
}



