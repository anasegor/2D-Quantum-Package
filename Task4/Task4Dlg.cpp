
// Task4Dlg.cpp: файл реализации
//
#include <algorithm>
#include <fstream>
#include <thread>
#include "pch.h"
#include "framework.h"
#include "Task4.h"
#include "Task4Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()




CTask4Dlg::CTask4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK4_DIALOG, pParent)
	, dt(0.005)
	, Nt(128)
	, R(1)
	, N(128)
	, a(0.1)
	, V0(0.1)
	, b(0.1)
	, sigmaX(0.1)
	, sigmaY(0.1)
	, muX(0)
	, muY(0)
	, numberX(30)
	, numberY(30)
	, E(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTask4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_for_dt, dt);
	DDX_Text(pDX, IDC_for_N, Nt);
	DDX_Text(pDX, IDC_for_x, R);
	DDX_Text(pDX, IDC_for_a, a);
	DDX_Text(pDX, IDC_for_V0, V0);
	DDX_Text(pDX, IDC_for_b, b);
	DDX_Text(pDX, IDC_for_sigmaX, sigmaX);
	DDX_Text(pDX, IDC_for_sigmaY, sigmaY);
	DDX_Text(pDX, IDC_for_muX, muX);
	DDX_Text(pDX, IDC_for_muY, muY);
	DDX_Text(pDX, IDC_for_numberX, numberX);
	DDX_Text(pDX, IDC_for_numberY, numberY);
	DDX_Text(pDX, IDC_for_E, E);
	DDX_Control(pDX, IDC_SLIDER3, pos_t);
}

BEGIN_MESSAGE_MAP(CTask4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTask4Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTask4Dlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_WavePacket2, &CTask4Dlg::OnBnClickedWavepacket2)
	ON_BN_CLICKED(ID_WavePacket3, &CTask4Dlg::OnBnClickedWavepacket3)
	ON_BN_CLICKED(ID_WavePacket4, &CTask4Dlg::OnBnClickedWavepacket4)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



BOOL CTask4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	PicWave0.Create(GetDlgItem(IDC_Pic2)->GetSafeHwnd());
	PicWave.Create(GetDlgItem(IDC_Pic3)->GetSafeHwnd());
	Spectr.Create(GetDlgItem(IDC_Pic1)->GetSafeHwnd());
	Spectr_t.Create(GetDlgItem(IDC_Pic4)->GetSafeHwnd());


	return TRUE;  
}

void CTask4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CTask4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CTask4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI FunctionThread(PVOID pv)
{
	CTask4Dlg* p = (CTask4Dlg*)pv;
	p->StartCalcWave();
	return 0;
}
void CTask4Dlg::StartCalcWave()
{	
	CalculateWaveEvolution(wave0, wave,R, Nt, dt, flag, a, V0, b, wave_modul);
	wavefft = wave;
	spectr.resize(Nt);
	for (int ix = 0; ix < wave[0].size(); ix++)
	{
		for (int iy = 0; iy < wave[0][ix].size(); iy++)
		{
			for (int i = 0; i < Nt; i++)
				spectr[i] = wave[i][ix][iy];
			fourea(Nt,spectr,-1);

			for (int i = 0; i < Nt; i++)
				wavefft[i][ix][iy] = spectr[i];
		}
	}
}

void CTask4Dlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CTask4Dlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CTask4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if ((flag) && (timer_iter < Nt))
	{
		PicWave.DrawWaveIsolines(wave_modul[timer_iter],R);
		timer_iter++;
	}
}


void CTask4Dlg::OnBnClickedWavepacket2()
{
	flag = false;
	timer_iter = 0;
	wave.clear();
	wave0.clear();
	wave_modul.clear();
	UpdateData(TRUE);
	pos_t.SetRange(0, int(Nt - 1));
	if (pow(2, (int)log2(Nt)) != Nt || Nt < 1)
	{
		MessageBox(L"Количество волн должно быть больше 0\nи являться степенью 2!");
		Beep(5000, 500);
		return;
	}
	wave0 = CreateWaveFunction(R, muX, sigmaX,muY, sigmaY, N);
	PicWave0.DrawWave(wave0, R);
	SetTimer(NULL, 100, NULL);
	hThread = CreateThread(NULL, 0, FunctionThread, this, 0, &dwThread);
	
	
}


void CTask4Dlg::OnBnClickedWavepacket3()
{
	KillTimer(NULL);
}



void CTask4Dlg::OnBnClickedWavepacket4()
{
	if (flag)
	{
		UpdateData(TRUE);
		spectr.resize(Nt);
		spectr_step_t.resize(Nt);
		for (int i{}; i < Nt; i++)
		{
			spectr[i] = wavefft[i][numberX][numberY];
			spectr_step_t[i] = i / (Nt* dt);
		}
		spectr_module.resize(Nt);
		spectr_module = Cmplx2Doublee(spectr);
		posF = 0;
		double maxE{};
		for (int i{}; i < Nt; i++)
			if (spectr_module[i] > maxE)
			{
				posF = i;
				maxE = spectr_module[i];
			}
		double f = posF/(Nt * dt) ;
		E = h * f;
		UpdateData(FALSE);
		Spectr.DrawOneSigFromZero(spectr_module, L"f", L"E", spectr_step_t, posF / (Nt * dt));
		spectr_t = wavefft[posF];
		spectr_modul_t = Cmplx2Double(spectr_t);
		Spectr_t.DrawWave(spectr_modul_t, R);
		
	}
	else
	{
		MessageBox(L"Волновые функции еще не готовы!");
	}
	
}


void CTask4Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (flag)
	{
		post=pos_t.GetPos();
		//post = posF;
		spectr_t = wavefft[post];
		spectr_modul_t = Cmplx2Double(spectr_t);
		Spectr_t.DrawWave(spectr_modul_t, R);
		Spectr.DrawOneSigFromZero(spectr_module, L"f", L"E", spectr_step_t, post / (Nt * dt));
	}
}
