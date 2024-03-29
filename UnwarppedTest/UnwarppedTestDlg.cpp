
// UnwarppedTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "UnwarppedTest.h"
#include "UnwarppedTestDlg.h"
#include "afxdialogex.h"
#include"opencv.hpp"
#include "math.h"

using namespace cv;
using namespace std;

#define step 4   //步数
#define res_x 1296  //分辨率
#define res_y 966

#define gray_max 250  //最大/最小灰度值
#define gray_min 6
#define kind 0 //0为横向；1为纵向光栅

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
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


// CUnwarppedTestDlg 对话框



CUnwarppedTestDlg::CUnwarppedTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNWARPPEDTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CUnwarppedTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUnwarppedTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_CBN_SELCHANGE(IDC_COMBO1, &CUnwarppedTestDlg::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_BUTTON1, &CUnwarppedTestDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CUnwarppedTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CUnwarppedTestDlg 消息处理程序

BOOL CUnwarppedTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUnwarppedTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUnwarppedTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUnwarppedTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUnwarppedTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ofstream file;
	file.open("debug.txt");
	WarpPhase = new double**[3];  //创建三维数组:[周期数][分辨率/y][分辨率/x]
	for (int i = 0; i < 3; i++) {        //3周期
		WarpPhase[i] = new double*[res_y];
		for (int j = 0; j < res_y; j++) {
			WarpPhase[i][j] = new double[res_x];
		}
	}
if (kind == 0) {
	for (int m = 0; m < 3; m++) {

		String FilePath1 = "..\\images\\";     //光栅图地址,项目根目录下
		String FilePath2 = "..\\images\\";
		String FilePath3 = "..\\images\\";
		String FilePath4 = "..\\images\\";

		Mat demo(res_y, res_x, CV_8U);  //模板
		int fre = pow(8, m);    //三个周期0,8,64

		FilePath1 += "horizontal grating ";
		char buf1[10];
		_itoa(fre, buf1, 10);
		FilePath1 += buf1;
		FilePath1 += "-0.bmp";
		Mat img1 = imread(FilePath1,0);

		FilePath2 += "horizontal grating ";
		char buf2[10];
		_itoa(fre, buf2, 10);
		FilePath2 += buf2;
		FilePath2 += "-90.bmp";
		Mat img2 = imread(FilePath2,0);

		FilePath3 += "horizontal grating ";
		char buf3[10];
		_itoa(fre, buf3, 10);
		FilePath3 += buf3;
		FilePath3 += "-180.bmp";
		Mat img3 = imread(FilePath3,0);

		FilePath4 += "horizontal grating ";
		char buf4[10];
		_itoa(fre, buf4, 10);
		FilePath4 += buf4;
		FilePath4 += "-270.bmp";
		Mat img4 = imread(FilePath4,0);
       
		for (int j = 0; j < res_y; j++) {
			for (int i = 0; i < res_x; i++) {
				double I1 = img1.at<uchar>(j, i);
				double I2 = img2.at<uchar>(j, i);
				double I3 = img3.at<uchar>(j, i);
				double I4 = img4.at<uchar>(j, i);

				//file << I1 << endl << I2 << endl << endl;
				double num = atan2((double)(I1 - I3), (double)(I2 - I4));

				WarpPhase[m][j][i] = num;   //将相位值存入三维数组

				num = 255.0*num / (2 * CV_PI);
				demo.at<uchar>(j, i) = num;

			}
		}
		String FilePathName = "..\\result\\horizontal WarpPhasePicture";
		char buf[10];
		_itoa(fre, buf, 10);
		FilePathName += buf;
		FilePathName += ".bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo);
		imwrite(FilePathName, demo);
		waitKey(0);

	}
}
if(kind==1) {
	for (int m = 0; m < 3; m++) {

		String FilePath1 = "..\\images\\";     //光栅图地址,项目根目录下
		String FilePath2 = "..\\images\\";
		String FilePath3 = "..\\images\\";
		String FilePath4 = "..\\images\\";

		Mat demo(res_y, res_x, CV_8UC1);  //模板
		int fre = pow(8, m);    //三个周期0,8,64

		FilePath1 += "vertical grating ";
		char buf1[10];
		_itoa(fre, buf1, 10);
		FilePath1 += buf1;
		FilePath1 += "-0.bmp";
		Mat img1 = imread(FilePath1,0);

		FilePath2 += "vertical grating ";
		char buf2[10];
		_itoa(fre, buf2, 10);
		FilePath2 += buf2;
		FilePath2 += "-90.bmp";
		Mat img2 = imread(FilePath2,0);

		FilePath3 += "vertical grating ";
		char buf3[10];
		_itoa(fre, buf3, 10);
		FilePath3 += buf3;
		FilePath3 += "-180.bmp";
		Mat img3 = imread(FilePath3,0);

		FilePath4 += "vertical grating ";
		char buf4[10];
		_itoa(fre, buf4, 10);
		FilePath4 += buf4;
		FilePath4 += "-270.bmp";
		Mat img4 = imread(FilePath4,0);

		for (int j = 0; j < res_y; j++) {
			for (int i = 0; i < res_x; i++) {
				double I1 = img1.at<uchar>(j, i);
				double I2 = img2.at<uchar>(j, i);
				double I3 = img3.at<uchar>(j, i);
				double I4 = img4.at<uchar>(j, i);

				//file << I1 << endl << I2 << endl << endl;
				double num = atan2((double)(I1 - I3), (double)(I2 - I4));

				WarpPhase[m][j][i] = num;   //将相位值存入三维数组

				num = 255.0*num / (2 * CV_PI);
				demo.at<uchar>(j, i) = num;

			}
		}
		String FilePathName = "..\\result\\vertical WarpPhasePicture";
		char buf[10];
		_itoa(fre, buf, 10);
		FilePathName += buf;
		FilePathName += ".bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo);
		imwrite(FilePathName, demo);
		waitKey(0);

	}
}
file.close();

}

void CUnwarppedTestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	double **unwarp;
	unwarp = new double*[res_y];
	for (int i = 0; i < res_y; i++) {
		unwarp[i] = new double[res_x];   //创建一个二维数组来保存频率为8的展开相位
	}
	for (int m = 0; m < res_y; m++) {
		for (int n = 0; n < res_x; n++) {
			unwarp[m][n]=WarpPhase[1][m][n]+2*CV_PI*round((8 * WarpPhase[0][m][n] - WarpPhase[1][m][n]) / (2 * CV_PI));
		}
	}
	Mat demo1(res_y, res_x, CV_8UC1);

	for (int j = 0; j < res_y; j++) {
		for (int i = 0; i < res_x; i++) {
			double num = WarpPhase[2][j][i] + 2 * CV_PI*round((8 * unwarp[j][i] - WarpPhase[2][j][i]) / (2 * CV_PI));
			
			num = 255.0*((num) * 2 / 128) / (2 * CV_PI);  //先转换到0~2π，再转换为0~255灰度区间上

			demo1.at<uchar>(j, i) = num;
		}
	}
	if (kind == 0) {
		String FilePathName = "..\\result\\horizontal UnWarpPhasePicture.bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo1);
		imwrite(FilePathName, demo1);
		waitKey(0);
	}
	if (kind == 1) {
		String FilePathName = "..\\result\\vertical UnWarpPhasePicture.bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo1);
		imwrite(FilePathName, demo1);
		waitKey(0);
	}

	for (int i = 0; i < res_y; i++) {
		delete unwarp[i];    //释放unwarp二维数组
	}
	delete unwarp;

}
