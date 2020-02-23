
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
//#include <opencv2/opencv.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

CvCapture* camera_capture;
CRect showImage_clientRect;
CDC *showImage_pDC;
HDC showImage_hDC;
CWnd *showImage_pwnd;

CvVideoWriter* cameraVideoWriter = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMFMarchTrainDlg 对话框



CMFMarchTrainDlg::CMFMarchTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFMarchTrainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFMarchTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON1, m_Enable);
	//DDX_Control(pDX, IDC_BUTTON2, m_Exit);
}

BEGIN_MESSAGE_MAP(CMFMarchTrainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//ON_BN_CLICKED(IDCANCEL, &CMFMarchTrainDlg::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON1, &CMFMarchTrainDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CMFMarchTrainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_btnOpenCamera, &CMFMarchTrainDlg::OnBnClickedbtnopencamera)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnCloseCamera, &CMFMarchTrainDlg::OnBnClickedbtnclosecamera)
	ON_BN_CLICKED(ID_btnNextPage, &CMFMarchTrainDlg::OnBnClickedbtnnextpage)
	//ON_BN_CLICKED(ID_btnVideo, &CMFMarchTrainDlg::OnBnClickedbtnvideo)
END_MESSAGE_MAP()


// CMFMarchTrainDlg 消息处理程序

BOOL CMFMarchTrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO:  在此添加额外的初始化代码

	showImage_pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);  
	showImage_pDC = showImage_pwnd->GetDC();
	//pDC =GetDC();  
	showImage_hDC = showImage_pDC->GetSafeHdc();
	showImage_pwnd->GetClientRect(&showImage_clientRect);

	//m_Exit.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFMarchTrainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFMarchTrainDlg::OnPaint()
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
HCURSOR CMFMarchTrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CMFMarchTrainDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(_T("确定退出吗"), _T("提示"), MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		return;
	}
	CDialogEx::OnClose();
}









void CMFMarchTrainDlg::OnBnClickedbtnopencamera()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!camera_capture)
	{
		camera_capture = cvCaptureFromCAM(0);
		//AfxMessageBox("OK");
	}

	if (!camera_capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		return;
	}
	cameraVideoWriter = cvCreateVideoWriter("MyVideo.avi", CV_FOURCC('x', 'v', 'I', 'D'), 25, cvSize(640, 480));
	// 测试
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(camera_capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect);
		//cvWaitKey(10);
	}

	// 设置计时器,每10ms触发一次事件
	SetTimer(1, 10, NULL);
}


void CMFMarchTrainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(camera_capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect);
		cvWriteFrame(cameraVideoWriter, m_Frame);
		//cvWaitKey(10);
	}

	
	CDialogEx::OnTimer(nIDEvent);
}


void CMFMarchTrainDlg::OnBnClickedbtnclosecamera()
{
	// TODO:  在此添加控件通知处理程序代码
	cvReleaseCapture(&camera_capture);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	showImage_pDC->StretchBlt(showImage_clientRect.left, showImage_clientRect.top, showImage_clientRect.Width(), showImage_clientRect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
}


void CMFMarchTrainDlg::OnBnClickedbtnnextpage()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CMFMarchTrainDlg::OnBnClickedbtnvideo()
{
	// TODO:  在此添加控件通知处理程序代码
}
