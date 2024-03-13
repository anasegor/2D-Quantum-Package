#include "pch.h"
#include "Drawer.h"
struct Point {
	double x, y;
};
void Drawer::Create(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;
}

vector<double> Drawer::convert_range(vector<double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}

double Drawer::convert_range_d(double data, double outmax, double outmin, double inmax, double inmin)
{
	double output = data;
	double k = (outmax - outmin) / (inmax - inmin);

	return (output - inmin) * k + outmin;
}

void Drawer::DrawOneSig(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX/*, CDC* mem*/)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(37, 52, 148));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
		);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	

	double data_x_max = MaxF(dataX);
	double data_x_min = -data_x_max;
	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
	vector<double> x = convert_range(dataX, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(x[0], y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(x[i], y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		
		str.Format(L"%.1g", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1g", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawOneSigFromZero(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX, double posX/*, CDC* mem*/)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen line_pen(PS_SOLID, 2, RGB(32, 125, 57));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	data_y_max = MaxF(data);
	data_y_min = MinF(data);


	double data_x_max = MaxF(dataX);
	double data_x_min = 0;
	if (data.empty()) return;

	memDC.SelectObject(&line_pen);
	//memDC.SelectObject(&points_brush);

	posX= convert_range_d(posX, actual_right, actual_left, data_x_max, data_x_min);
	memDC.MoveTo(posX, convert_range_d(data_y_min, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(posX, convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush); 

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
	vector<double> x = convert_range(dataX, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(x[0], y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(x[i], y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1g", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1g", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawWave(const vector<vector<long double>>& wave, double R)
{
	if (!init) return;

	int padding = 0;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	double y_max = R;
	double y_min = -R;
	double x_max = R;
	double x_min = -R;

	long double stepX = (x_max - x_min) / wave.size();
	long double stepY = (y_max - y_min) / wave[0].size();
	for (int i = 0; i < wave.size() ; i++)
	{
		double x1 = convert_range_d(i * stepX + x_min, actual_right, actual_left, x_max, x_min);
		double x2 = convert_range_d(i * stepX + x_min + stepX, actual_right, actual_left, x_max, x_min);
		for (int j = 0; j < wave[0].size() ; j++)
		{
			long double module = wave[i][j];
			int col = module * 256;
			CBrush br(RGB(col, 0, 0));
			memDC.SelectObject(&br);
			CPen br_pen(PS_SOLID, 1, RGB(col, 0, 0));
			memDC.SelectObject(&br_pen);
			double y1 = convert_range_d(j * stepY + y_min, actual_top, actual_bottom, y_max, y_min);
			double y2 = convert_range_d(j * stepY + y_min + stepY, actual_top, actual_bottom, y_max, y_min);
			memDC.Rectangle(x1, y1, x2, y2);
		}
	}
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
// сортировка используется в алгоритме рисовки изолиний
void Sort(double arr[3], Point points[3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				swap(points[j], points[j + 1]);
			}
		}
	}
}
long double FindMaxDouble(const vector<vector<long double>>&vec)
{
	long double max = vec[0][0];
	for (int i = 0; i < vec.size(); i++)
	{
		long double temp = *max_element(vec[i].begin(), vec[i].end());
		if (temp > max) max = temp;
	}
	return max;
}
long double FindMinDouble(const vector<vector<long double>> &vec)
{
	long double min = vec[0][0];
	for (int i = 0; i < vec.size(); i++)
	{
		long double temp = *max_element(vec[i].begin(), vec[i].end());
		if (temp < min) min = temp;
	}
	return min;
}
void Drawer::DrawWaveIsolines(const vector<vector<long double>>& wave, double R)
{
	if (!init) return;

	int padding = 0;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	double y_max = R;
	double y_min = -R;
	double x_max = R;
	double x_min = -R;

	long double stepX = (x_max - x_min) / wave.size();
	long double stepY = (y_max - y_min) / wave[0].size();
	for (int i = 0; i < wave.size(); i++)
	{
		double x1 = convert_range_d(i * stepX + x_min, actual_right, actual_left, x_max, x_min);
		double x2 = convert_range_d(i * stepX + x_min + stepX, actual_right, actual_left, x_max, x_min);
		for (int j = 0; j < wave[0].size(); j++)
		{
			long double module = wave[i][j];
			int col = module * 256;
			CBrush br(RGB(col, 0, 0));
			memDC.SelectObject(&br);
			CPen br_pen(PS_SOLID, 1, RGB(col, 0, 0));
			memDC.SelectObject(&br_pen);
			double y1 = convert_range_d(j * stepY + y_min, actual_top, actual_bottom, y_max, y_min);
			double y2 = convert_range_d(j * stepY + y_min + stepY, actual_top, actual_bottom, y_max, y_min);
			memDC.Rectangle(x1, y1, x2, y2);
		}
	}

	int numIsolines = 5;
	CPen is_pen(PS_SOLID, 1, RGB(255, 255, 255));
	memDC.SelectObject(&is_pen);

	//double xx= convert_range_d(x_min, actual_right, actual_left, x_max, x_min);
	//double yy = convert_range_d(x_max, actual_right, actual_left, x_max, x_min);

	double PhiMax = FindMaxDouble(wave);
	double PhiMin = FindMinDouble(wave);
	for (int i = 1; i < numIsolines; i++)
	{
		double isoline = PhiMin + i * (PhiMax - PhiMin) / numIsolines;
		double nodes[3];
		Point points[3];
		for (int i = 0; i < wave.size() - 1; i++)
		{
			for (int j = 0; j < wave[0].size() - 1; j++)
			{
				for (int k = 0; k < 2; k++) {
					double values[2], keys[2];
					if (k == 0) {
						nodes[0] = wave[i][j];
						nodes[1] = wave[i + 1][j];
						nodes[2] = wave[i][j + 1];

						points[0].x = i * stepX + x_min;
						points[0].y = j * stepY + y_min;
						points[1].x = i * stepX + x_min + stepX;
						points[1].y = j * stepY + y_min;
						points[2].x = i * stepX + x_min ;
						points[2].y = j * stepY + y_min + stepY;
					}
					else {
						nodes[0] = wave[i + 1][j];
						nodes[1] = wave[i + 1][j + 1];
						nodes[2] = wave[i][j + 1];

						points[0].x = i * stepX + x_min + stepX;
						points[0].y = j * stepY + y_min;
						points[1].x = i * stepX + x_min + stepX;
						points[1].y = j * stepY + y_min + stepY;
						points[2].x = i * stepX + x_min;
						points[2].y = j * stepY + y_min + stepY;
					}
					Sort(nodes, points);

					if (isoline < nodes[0] || isoline > nodes[2])
						continue;

					double x1, y1, x2, y2;

					if (isoline >= nodes[0] && isoline < nodes[1]) {
						x1 = points[1].x * 1. - (nodes[1] - isoline) *
							(points[1].x * 1. - points[0].x * 1.) / (nodes[1] - nodes[0]);
						y1 = points[1].y * 1. - (nodes[1] - isoline) *
							(points[1].y * 1. - points[0].y * 1.) / (nodes[1] - nodes[0]);
					}
					else {
						x1 = points[2].x * 1. - (nodes[2] - isoline) *
							(points[2].x * 1. - points[1].x * 1.) / (nodes[2] - nodes[1]);
						y1 = points[2].y * 1. - (nodes[2] - isoline) *
							(points[2].y * 1. - points[1].y * 1.) / (nodes[2] - nodes[1]);
					}

					x2 = points[2].x * 1. - (nodes[2] - isoline) * (points[2].x * 1. - points[0].x * 1.) / (nodes[2] - nodes[0]);
					y2 = points[2].y * 1. - (nodes[2] - isoline) * (points[2].y * 1. - points[0].y * 1.) / (nodes[2] - nodes[0]);

					x1 = convert_range_d(x1, actual_right, actual_left, x_max, x_min);
					x2 = convert_range_d(x2, actual_right, actual_left, x_max, x_min);
					y1 = convert_range_d(y1, actual_top, actual_bottom, y_max, y_min);
					y2 = convert_range_d(y2, actual_top, actual_bottom, y_max, y_min);
					memDC.MoveTo(x1, y1);
					memDC.LineTo(x2, y2);
				}
			}
		}
	}
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	
}

double Drawer::MaxF(vector<double> y)
{
	if (y.empty()) return 0;
	double max = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] > max)
		{
			pr = max;
			max = y[i];
			y[i] = pr;
		}
	}

	return max;
}

double Drawer::MinF(vector<double> y)
{
	double min = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] < min)
		{
			pr = min;
			min = y[i];
			y[i] = pr;
		}
	}

	return min;
}


