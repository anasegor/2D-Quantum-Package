
#include "Drawer.h"
#include <chrono>
#pragma once


// Диалоговое окно CTask4Dlg
class CTask4Dlg : public CDialogEx
{
// Создание
public:
	CTask4Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void StartCalcWave();
	double h = (1e-27)*6.63;
	double dt;
	int Nt;
	double R;
	double N;

	double d_step;
	DWORD dwThread;
	HANDLE hThread;
	std::vector<vector<long double>> wave0; 
	std::vector<vector<vector<long double>>> wave_modul;
	std::vector<vector<vector<complexld>>> wave;
	


	std::vector<std::vector<std::vector <complexld>>> wavefft;
	std::vector<double> spectr_step_t;
	std::vector<complexld> spectr;
	std::vector<double> spectr_module;
	std::vector<vector<long double>> spectr_modul_t;
	std::vector<vector<complexld>> spectr_t;
	bool flag = false;
	int timer_iter = 0;
	Drawer PicWave0;
	Drawer PicWave;
	Drawer Spectr;
	Drawer Spectr_t;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedWavepacket2();
	afx_msg void OnBnClickedWavepacket3();
	int posF;
	double a;
	double V0;
	double b;
	double sigmaX;
	double sigmaY;
	double muX;
	double muY;
	int numberX;
	int numberY;
	double E;
	int post = 0;
	afx_msg void OnBnClickedWavepacket4();
	CSliderCtrl pos_t;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
