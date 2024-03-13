#pragma once

#include <afxwin.h>
#include <vector>
#include <time.h>

using namespace std;
struct PotPt
{
	//потенциал
	double phi;
	//координата
	double x, y;

	PotPt& operator=(PotPt& other)
	{
		//i = other.i;
		//j = other.j;
		x = other.x;
		y = other.y;

		phi = other.phi;
		return *this;
	}

};


//			   1  -----------  2
//				 |   	   / |
//				 Y   	 /   |
//				 |     / pt  |
//				 |	 /       |
//				 | /	     |
//        	   4  -------X---  3

//ячейка
struct Cell
{

	bool upperTri; //верхний треугольник
	bool lowerTri; //нижний треугольник
	PotPt  pt1, pt2, pt3, pt4;
	double nx_upper, ny_upper;
	double nx_lower, ny_lower;

	//проверка, что точка в ячейке
	bool IsPointIn(double x, double y)
	{
		/// Эта функция смотрит где относительно диагональной оси в квадрате находится точка. Диагон ось рассматриваю как прямую y=kx
		//
		//		Y2 - Y1
		//	K = --------	(X2; Y2) - Моя верхняя правая точка квадрата, (X1; Y1) - координаты рассматриваемой точки
		//		X2 - X1
		//	
		// Так как У увлечивается вниз, а Х вправо, если k>= -1, то точка лежит в нижнем треугольнике
		double k;
		if (x >= pt1.x && x <= pt3.x && y >= pt1.y && y <= pt3.y)
		{
			k = (y - pt2.y) / (x - pt2.x);
			if (k >= -1) { upperTri = false; lowerTri = true; }
			else { upperTri = true; lowerTri = false; }

			return true;
		}
		else return false;
	}
	//рассчет нормалей
	/*void CalculateNormals()
	{

		double length;
		nx_upper = (pt3.y - pt1.y) * (pt2.phi - pt1.phi) - (pt2.y - pt1.y) * (pt3.phi - pt1.phi);
		ny_upper = -(pt3.x - pt1.x) * (pt2.phi - pt1.phi) + (pt2.x - pt1.x) * (pt3.phi - pt1.phi);

		nx_lower = (pt4.y - pt1.y) * (pt3.phi - pt1.phi) - (pt3.y - pt1.y) * (pt4.phi - pt1.phi);
		ny_lower = -(pt4.x - pt1.x) * (pt3.phi - pt1.phi) + (pt3.x - pt1.x) * (pt4.phi - pt1.phi);


		length = sqrt(ny_upper * ny_upper + nx_upper * nx_upper);
		if (length != 0) {
			nx_upper /= length;
			ny_upper /= length;
		}
		length = sqrt(ny_lower * ny_lower + nx_lower * nx_lower);
		if (length != 0)
		{
			nx_lower /= length;
			ny_lower /= length;
		}
	}

	// где окажется в следующий момент времени
	void Propagate(double& x, double& y, double& step)
	{
		if (upperTri)
		{
			x += step * nx_upper;
			y += step * ny_upper;
		}
		else if (lowerTri)
		{
			x += step * nx_lower;
			y += step * ny_lower;
		}
	}*/
};


class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, обалсти рисования.
	CWnd * wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC * dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;
public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd);

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);

	double convert_range_d(double data, double outmax, double outmin, double inmax, double inmin);

	// Нарисовать сигнал.
	void DrawOneSig(vector<double>& data, CString Ox, CString Oy, vector<double>& y);
	void DrawOneSigFromZero(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX, double posX/*, CDC* mem*/);
	void DrawWave(const vector<vector <long double>>& wave, double R);
	void DrawWaveIsolines(const vector<vector <long double>>& wave, double R);
	double MaxF(vector<double> y);

	double MinF(vector<double> y);


};

