
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "dlgPage2.h"
//#include "CvImgCtrl.h"
//#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
//#include <vector>
//#include <iostream>
//#include <iomanip>
//#include <sstream>
 #include <fstream>
//#include <Windows.h>
//#include <string>
//#include <mmsystem.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

CvCapture* camera_capture;
CRect showImage_clientRect;
CDC *showImage_pDC;
HDC showImage_hDC;
CWnd *showImage_pwnd;
string cameraVideoSavePath;
CString infoStr;
//CvVideoWriter* cameraVideoWriter = 0;
VideoWriter cameraVideoWriter;
//进度条
//int   G_slider_position = 0;
//CvCapture*    G_capture = NULL;
//cv::VideoCapture*    G_capture   =  NULL;
////HBITMAP hbitmap;
//CRect rect;
//CStatic* pStc_PictureForVideo; //标识图像显示的Picture控件
//CDC* pDC; //视频显示控件设备上下文
//HDC hDC; //视频显示控件设备句柄
// 保存视频处理数据

//原来的视频对比处理程序函数

string videoFilename_std = "std_ROI_J0101.avi";
string	videoFilename_test = cameraVideoSavePath;
string	videoFilename_color_std = "std_color_ROI_J0101.avi";
VideoCapture capture_std;
VideoCapture capture_color_std;
VideoCapture capture_test;			//新兵的视频，从录入的视频中读取

VideoWriter testVideoWrite;
VideoWriter contrastVideoWriter;

ofstream coin_rate_file;
Mat Body_center = Mat(480, 640, CV_8UC3);		//用于绘制外接框中心点的运动范围
Mat s_test;		//计算胸点像素值，承载图片用的矩阵变量
int chest_test_x, chest_test_y;		//计算胸点像素值，胸点横坐标的值
int	Num_of_frame = 0;
//int keyboard = 0;
Mat	frame_std, frame_test, frame_color_std;
// 【利用图像G像素值-R像素值得到前景图】	
void extract(const Mat &std, Mat &outing);
// 输入二值图，输出将二值的白色变成绿色
void dif_imge(const Mat &std, const Mat &test, Mat &outimg, const Mat &input, float &area, float &body_area);
void save_coincidence(int Num_frame, float coincidence_rate, int middle_dot_width);
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
protected:
//	CvImgCtrl m_show;
public:
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
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
//	ON_WM_HSCROLL()
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
	//DDX_Control(pDX, IDC_SLIDER_videoPlay, sliderVideoPlay);
	DDX_Control(pDX, IDC_SLIDERVideo, sliderVideo);
	DDX_Control(pDX, IDC_EDITInfo, editInfo);
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
	ON_BN_CLICKED(IDC_btnOpenVideo, &CMFMarchTrainDlg::OnBnClickedbtnopenvideo)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_videoPlay, &CMFMarchTrainDlg::OnNMCustomdrawSlidervideoplay)
	ON_BN_CLICKED(IDC_btnPausePlay, &CMFMarchTrainDlg::OnBnClickedbtnpauseplay)
	ON_BN_CLICKED(IDC_startPlay, &CMFMarchTrainDlg::OnBnClickedstartplay)
	ON_BN_CLICKED(IDC_btnStopPlay, &CMFMarchTrainDlg::OnBnClickedbtnstopplay)
//	ON_MESSAGE(MsgVideoFrame, &CMFMarchTrainDlg::OnFrame)
	ON_MESSAGE(MsgVideoFrame, &CMFMarchTrainDlg::OnFrame)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_btnStartRecord, &CMFMarchTrainDlg::OnBnClickedbtnstartrecord)
	ON_BN_CLICKED(IDC_btnStopRecord, &CMFMarchTrainDlg::OnBnClickedbtnstoprecord)
	ON_BN_CLICKED(ID_btnVideoAnalyse, &CMFMarchTrainDlg::OnBnClickedbtnvideoanalyse)
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
	showImage_pDC = showImage_pwnd->GetDC();	  
	showImage_hDC = showImage_pDC->GetSafeHdc();
	showImage_pwnd->GetClientRect(&showImage_clientRect);

	//pStc_PictureForVideo = (CStatic *)GetDlgItem(IDC_ShowImage);//IDC_VIEW为Picture控件ID
	//pStc_PictureForVideo->GetClientRect(&showImage_clientRect);//Picture的大小传给矩形rect
	//pDC = pStc->GetDC(); //得到Picture控件设备上下文
	//hDC = pDC->GetSafeHdc(); //得到Picture控件设备上下文的句柄 
	//hbitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));//加载位图资源
	//pStc_PictureForVideo->ModifyStyle(1, SS_BITMAP);//修改控件的属性
	//pStc_PictureForVideo->SetBitmap(hbitmap);//显示位图
	/*CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	showImage_pDC->StretchBlt(showImage_clientRect.left, showImage_clientRect.top, showImage_clientRect.Width(), showImage_clientRect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	*/
	//m_Exit.EnableWindow(FALSE);
	//链接到控件
	//绑定窗口
	//CWnd  *pWnd1 = GetDlgItem(IDC_ShowImage);//CWnd是MFC窗口类的基类,提供了微软基础类库中所有窗口类的基本功能。
	//pWnd1->GetClientRect(&showImage_clientRect);//GetClientRect为获得控件相自身的坐标大小
	//namedWindow("VideoBar", WINDOW_AUTOSIZE);//设置窗口名
	//HWND hWndl = (HWND)cvGetWindowHandle("VideoBar");//hWnd 表示窗口句柄,获取窗口句柄
	//HWND hParent1 = ::GetParent(hWndl);//GetParent函数一个指定子窗口的父窗口句柄
	//::SetParent(hWndl, GetDlgItem(IDC_ShowImage)->m_hWnd);
	//::ShowWindow(hParent1, SW_HIDE);//ShowWindow指定窗口中显示
	m_show.linkDlgItem(IDC_ShowImage, this);
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
	cvReleaseCapture(&camera_capture);
	CDialogEx::OnClose();
}









void CMFMarchTrainDlg::OnBnClickedbtnopencamera()
{
	// TODO:  在此添加控件通知处理程序代码
	m_show.openVideo(0);
	//设置为居中显示
	m_show.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//if (!camera_capture)
	//{
	//	camera_capture = cvCaptureFromCAM(0);
	//	//AfxMessageBox("OK");
	//}

	//// 测试
	//IplImage* m_Frame;
	//m_Frame = cvQueryFrame(camera_capture);
	//CvvImage m_CvvImage;
	//m_CvvImage.CopyOf(m_Frame, 1);
	//if (true)
	//{
	//	m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect);
	//	//cvWaitKey(10);
	//}

	//// 设置计时器,每10ms触发一次事件
	//SetTimer(1, 10, NULL);
	
}


void CMFMarchTrainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)//nIDEvent 为定时器事件ID，1，2，3
	{
	case 1:
		{
			  if (Training_Assistant() == false)
			  {
				  
				  KillTimer(1);
				  coin_rate_file.close();
				  contrastVideoWriter.release();
				  capture_std.release();
				  capture_color_std.release();
				  capture_test.release();
				  AfxMessageBox(_T("处理完毕"));
			  }
		}
		break;
	case 2:
		{		
			//Mat img;
			//video_capture >> img;  //取出一帧图像
			//if (img.empty())
			//{
			//	KillTimer(2);
			//	video_capture.release();
			//	AfxMessageBox(_T("视频结束"));
			//	//video_capture.release();
			//}
			//else
			//{
			//	CvvImage m_CvvImage;
			//	IplImage frame(img);   //Mat 转IplImage
			//	m_CvvImage.CopyOf(&frame, 1); //复制该帧图像   
			//	m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect); //显示到设备的矩形框内
			//	sliderVideoPlay.SetPos((int)video_capture.get(CV_CAP_PROP_POS_FRAMES));//设置视频的位置
			//}						  
		}
		break;
	}

	
	CDialogEx::OnTimer(nIDEvent);
}


void CMFMarchTrainDlg::OnBnClickedbtnclosecamera()
{
	// TODO:  在此添加控件通知处理程序代码
	/*KillTimer(1);
	cvReleaseCapture(&camera_capture);
	CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	showImage_pDC->StretchBlt(showImage_clientRect.left, showImage_clientRect.top, showImage_clientRect.Width(), showImage_clientRect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);*/
	//cvReleaseVideoWriter(&cameraVideoWriter);
	m_show.closeVideo();
}


void CMFMarchTrainDlg::OnBnClickedbtnnextpage()
{
	// TODO:  在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
	dlgPage2 dlgShowPage2;
	dlgShowPage2.DoModal();
	this->ShowWindow(SW_SHOW);
}


void CMFMarchTrainDlg::OnBnClickedbtnvideo()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CMFMarchTrainDlg::OnBnClickedbtnopenvideo()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString caption;
	//GetDlgItemText(IDC_btnOpenVideo, caption);
	//if (caption == _T("关闭视频"))    //关闭视频按钮
	//{
	//	KillTimer(2);
	//	video_capture.release();
	//	SetDlgItemText(IDC_btnOpenVideo, _T("打开视频"));
	//	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(FALSE);  //滑动条失效
	//	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->SetPos(0); //设置滑动条位置
	//	//pStc_PictureForVideo->SetBitmap(hbitmap); //恢复位图资源
	//	return;
	//}


	//打开视频,弹出通用对话框,选择要播放的视频文件
	string _Path;
	CFileDialog Openfiledlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (Openfiledlg.DoModal() == IDOK) //弹出模态对话框
	{
		//CString类型转换为string类型
		CString  filepath;
		filepath = Openfiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		filepath.ReleaseBuffer();
		_Path = temp.GetBuffer(0);
		temp.ReleaseBuffer();
	}
	else
		return;

	m_show.openVideo(_Path.c_str());	
	
	if (!m_show.getVideoCapture()->isOpened())
	{
		AfxMessageBox(_T("无法打开视频！"));
		return;
	}
	else
	{
			//获取视频的帧数
		Mat img;
		int frameCount = 0;

		do
		{
			m_show.getVideoCapture()->read(img);
			frameCount++;
		} while (!img.empty());
		((CSliderCtrl *)GetDlgItem(IDC_SLIDERVideo))->EnableWindow(TRUE);  //激活滑动条
		int sliderRange = frameCount;
		//int sliderRange = 100;
		sliderVideo.SetRange(0, sliderRange);//设置滑动条的范围,为视频的总帧数
		m_show.closeVideo();
		m_show.openVideo(_Path.c_str());  //重新打开视频
		//设置显示帧率
		m_show.setWaitTime((int)(1000 * 1.0 / m_show.getVideoCapture()->get(CV_CAP_PROP_FPS)));
		//设置为居中显示
		m_show.setResizeType(CvImgCtrl::ResizeType_CenterResize);
		
		
	}
	
}


//void CMFMarchTrainDlg::OnNMCustomdrawSlidervideoplay(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO:  在此添加控件通知处理程序代码
//	//m_show.getVideoCapture()->set(CV_CAP_PROP_POS_FRAMES, sliderVideoPlay.GetPos());  //设置视频的起始帧
//	*pResult = 0;
//}


void CMFMarchTrainDlg::OnBnClickedbtnpauseplay()
{
	// TODO:  在此添加控件通知处理程序代码
	m_show.pauseVideo();
}


void CMFMarchTrainDlg::OnBnClickedstartplay()
{
	// TODO:  在此添加控件通知处理程序代码
	m_show.resumeVideo();
}


void CMFMarchTrainDlg::OnBnClickedbtnstopplay()
{
	// TODO:  在此添加控件通知处理程序代码
	m_show.closeVideo();
	//((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(FALSE);  //滑动条失效
	//((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->SetPos(0); //设置滑动条位置
}

//处理每一帧的视频


afx_msg LRESULT CMFMarchTrainDlg::OnFrame(WPARAM wParam, LPARAM lParam)
{
	//Mat frame((IplImage*)lParam, false);
	//这里仅仅做了下翻转
	//flip(frame, frame, -1);
	sliderVideo.SetPos((int)m_show.getVideoCapture()->get(CV_CAP_PROP_POS_FRAMES));//设置视频的位置
	if (cameraVideoWriter.isOpened())
	{
		Mat frame((IplImage*)lParam, false);
		//cvWriteFrame(cameraVideoWriter, m_Frame);
		cameraVideoWriter.write(frame);
		//cvWaitKey(10);
	}
	return 0;
}


//void CAboutDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	
//	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
//}


void CMFMarchTrainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//m_show.pauseVideo();
	//CString strTest;
	//strTest.Format(_T("%d"), sliderVideo.GetPos());
	//AfxMessageBox(_T("跳转到帧数：") + strTest);
	m_show.getVideoCapture()->set(CV_CAP_PROP_POS_FRAMES, sliderVideo.GetPos());
	//Mat img;
	//m_show.getVideoCapture()->read(img);  //取出一帧图像
	//if (img.empty())
	//{
	//	KillTimer(2);
	//	m_show.getVideoCapture()->release();
	//	AfxMessageBox(_T("视频结束"));
	//	//video_capture.release();
	//}
	//else
	//{
	//	CvvImage m_CvvImage;
	//	IplImage frame(img);   //Mat 转IplImage
	//	m_CvvImage.CopyOf(&frame, 1); //复制该帧图像   
	//	m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect); //显示到设备的矩形框内		
	//}	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMFMarchTrainDlg::OnBnClickedbtnstartrecord()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_show.getVideoCapture()->isOpened())
	{
		AfxMessageBox(_T("摄像头没打开！"));
		return;
	}
	//打开视频,弹出通用对话框,选择要保存的视频文件

	CFileDialog saveFiledlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (saveFiledlg.DoModal() == IDOK) //弹出模态对话框
	{
		//CString类型转换为string类型
		CString  filepath;
		filepath = saveFiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		filepath.ReleaseBuffer();
		cameraVideoSavePath = temp.GetBuffer(0);
		temp.ReleaseBuffer();
	}
	else
		return;

	cameraVideoWriter = VideoWriter(cameraVideoSavePath.c_str(), CV_FOURCC('x', 'v', 'I', 'D'), 25, cvSize(640, 480));

}


void CMFMarchTrainDlg::OnBnClickedbtnstoprecord()
{
	// TODO:  在此添加控件通知处理程序代码
	//cvReleaseVideoWriter(&cameraVideoWriter);
	cameraVideoWriter.release();
}


void CMFMarchTrainDlg::OnBnClickedbtnvideoanalyse()
{
	// TODO:  在此添加控件通知处理程序代码
	if (cameraVideoSavePath.empty())
	{
		//string _Path;
		CFileDialog Openfiledlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
		if (Openfiledlg.DoModal() == IDOK) //弹出模态对话框
		{
			//CString类型转换为string类型
			CString  filepath;
			filepath = Openfiledlg.GetPathName();
			CStringA temp(filepath.GetBuffer(0));
			filepath.ReleaseBuffer();
			cameraVideoSavePath = temp.GetBuffer(0);
			temp.ReleaseBuffer();
		}
		else
			return;
	}
	CString hintStr(cameraVideoSavePath.c_str());
	CString tmpSavePathStr(cameraVideoSavePath.c_str());
	hintStr = _T("确定处理名为") + tmpSavePathStr + _T(" 的视频？");
	//hintStr.Format(_T("确定处理名为 /'%s/'的视频？"),cameraVideoSavePath.c_str());
	
	if (AfxMessageBox(hintStr, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		//=========================================【提取test前景视频并时行处理】=================================================
		videoFilename_std = "std_ROI_J0101.avi";
		videoFilename_test.assign(cameraVideoSavePath);
		videoFilename_color_std = "std_color_ROI_J0101.avi";
		capture_std = VideoCapture(videoFilename_std);
		capture_color_std = VideoCapture(videoFilename_color_std);
		capture_test = VideoCapture(videoFilename_test);			//新兵的视频，从录入的视频中读取	
		//coin_rate_file.open("D:\\M.A.thesis\\2019.04.11.TrainProject\\TrainProject\\coin_rate_file\\JBQ\\coin_rate_file_k1111.xls");
		coin_rate_file.open("coin_rate_file_k1111.xls");

		
		//string outVideo_test = "D:\\M.A.thesis\\ProjectData\\2019.04.16.JINGBEIQU\\k1111.avi";					//test视频处理好的视频保存位置
		string outVideo_test = "k1111.avi";					//test视频处理好的视频保存位置

		testVideoWrite.open(outVideo_test, CV_FOURCC('D', 'I', 'V', '3'), 5, Size(360, 480), false);
		CFileDialog saveFiledlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
		if (saveFiledlg.DoModal() == IDOK) //弹出模态对话框
		{
			//CString类型转换为string类型
			CString  filepath;
			filepath = saveFiledlg.GetPathName();
			CStringA temp(filepath.GetBuffer(0));
			filepath.ReleaseBuffer();
			string outVideo = temp.GetBuffer(0);
			contrastVideoWriter.open(outVideo, CV_FOURCC('D', 'I', 'V', '3'), 5, Size(360, 480), true);
			temp.ReleaseBuffer();
		}
		else
			return;
		//string outVideo = "D:\\M.A.thesis\\ProjectData\\2019.04.16.JINGBEIQU\\k1111.avi";			//视频比对处理好后保存的位置
		//string outVideo = "k1111duibi.avi";			//视频比对处理好后保存的位置
		//contrastVideoWriter.open(outVideo, CV_FOURCC('D', 'I', 'V', '3'), 5, Size(360, 480), true);

		//if (!capture_std.isOpened() || !capture_test.isOpened())
		if (!capture_test.isOpened())
		{
			//error in opening the video input
			/*cerr << "Unable to open video file: " << videoFilename_test << endl;
			exit(EXIT_FAILURE);*/
			CString tmpStr(_T("无法打开文件："));
			//tmpStr.Format(_T("无法打开文件： /'%s/'"), cameraVideoSavePath);		
			tmpStr += cameraVideoSavePath.c_str();
			AfxMessageBox(tmpStr);
			return;
		}
		infoStr = _T("开始处理视频");
		//CString tmpStr(_T("开始处理视频"));
		editInfo.SetWindowTextW(infoStr);
		SetTimer(1, 40, NULL);//激活处理程序时钟消息
	}


}




// 输入二值图，输出将二值的白色变成绿色
//Mat CMFMarchTrainDlg::convertTo3Channels(const Mat& binImg)
//{
//	return Mat();
//}


// 【利用图像G像素值-R像素值得到前景图】
void extract(const Mat& std, Mat& outing)
{
	int nr = std.rows;
	int nc = std.cols;
	outing.create(std.size(), std.type());
	for (int i = 0; i < 465; ++i)
	{
		const Vec3b * std_p = std.ptr<Vec3b>(i);
		Vec3b * out = outing.ptr<Vec3b>(i);

		for (int j = 0; j < nc; ++j)
		{
			//float dif = std_p[j][1] - std_p[j][2];
			int dif = std_p[j][1] - std_p[j][2];
			if (dif > 70)
				out[j][0] = out[j][1] = out[j][2] = 0;
			else
				out[j][0] = out[j][1] = out[j][2] = 255;
		}

	}
}


// 输入二值图，输出将二值的白色变成绿色
void dif_imge(const Mat& std, const Mat& test, Mat& outimg, const Mat& input, float& area, float& body_area)
{
	int nr = std.rows;
	int nc = std.cols;
	input.copyTo(outimg);
	for (int i = 0; i < nr; ++i)
	{
		const uchar *s = std.ptr<uchar>(i);     //获取std第i行第一个像素的指针
		const uchar *t = test.ptr<uchar>(i);    //获取test第i行第一个像素的指针
		Vec3b* out = outimg.ptr<Vec3b>(i);

		for (int j = 0; j < nc; ++j)
		{
			if (s[j] == 0 && t[j] == 255)
			{
				out[j][0] = 0;
				out[j][1] = 255;
				out[j][2] = 0;
			}
			//重合面积,在二值图情况下，当std和test在某点上的像素值都是255时，说明在这点上两个图片是重合的

			if (t[j] == 255)
			{
				++body_area;		//test全身的面积算一遍
				if (s[j] == 255)
				{
					++area;			//全身重合的面积
				}
			}

		}

	}
}


void save_coincidence(int Num_frame, float coincidence_rate, int middle_dot_width)
{
	coin_rate_file << Num_frame << " " << coincidence_rate << " " << middle_dot_width << " ";
	coin_rate_file << "\n";
}

bool CMFMarchTrainDlg::Training_Assistant()
{
			
	CString tmpStr(_T(""));
	//editInfo.SetWindowTextW(tmpStr);
	//while ((char)keyboard != 'q' && (char)keyboard != 27)
	{
		if (!capture_std.read(frame_std))
		{
			/*cerr << "Unable to read next frame_test." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);*/
			//AfxMessageBox(_T("无法打开frame_std！"));	
			
			infoStr.Append(_T("无法打开frame_std！\r\n"));
			editInfo.SetWindowTextW(infoStr);
			return false;
		}
		if (!capture_color_std.read(frame_color_std))
		{
			/*cerr << "Unable to read next frame_test." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);*/
			//AfxMessageBox(_T("无法打开frame_color_std！"));
			infoStr.Append(_T("无法打开frame_color_std！\r\n"));
			editInfo.SetWindowTextW(infoStr);
			return false;
		}
		if (!capture_test.read(frame_test))
		{
			/*cerr << "Unable to read next frame_test." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);*/
			//AfxMessageBox(_T("无法打开frame_test！"));
			infoStr.Append(_T("无法打开frame_test！\r\n"));
			editInfo.SetWindowTextW(infoStr);
			return false;
		}

		GaussianBlur(frame_test, frame_test, Size(3, 3), 0, 0);
		Mat fore_test = Mat::zeros(frame_test.size(), frame_test.type());

		extract(frame_test, fore_test);							//对test的图像提取前景
		cvtColor(fore_test, fore_test, CV_BGR2GRAY);

		//imshow("fore_std", frame_std);
		//imshow("fore_test", fore_test);							//显示提取前景的视频，这个视频显示的十值图像

		Mat element_std = getStructuringElement(MORPH_CROSS, Size(5, 5));	//第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
		morphologyEx(fore_test, fore_test, MORPH_CLOSE, element_std);		//对前景提取的图像进行闭运算

		Mat fore_color_test;
		frame_test.copyTo(fore_color_test, fore_test);

		//waitKey(20);			//加waitkey,用于缓冲，否则视频的速度太快了。40ms保证fps为25.

		Mat Rect_test = fore_test.clone();			//用于在test人的轮廓画矩形框

		vector<vector<Point>> contours_test;
		vector<Vec4i> hierarcy_test;

		//fore_test.copyTo(s_test);		//复制到s_test的矩阵中，为后面test找胸前点使用

		findContours(fore_test, contours_test, hierarcy_test, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		vector<Rect>boundRect_test(contours_test.size());		//定义外接矩形集合
		vector<RotatedRect>box_test(contours_test.size());		//定义最小外接矩形集合
		Point2f rect_test[4];

		//======================【给test轮廓画最小外接框，标记矩形框的中心点；找出test画面中，胸点高度7/30处的点，并进行标记】===================

		int Max_length_test = 0;		//这个值最后得出是高度最大的区域。
		int k = 0;				//用于记录test高度最高的区域点，再画矩形框
		if (contours_test.size() == 0) return true;

		for (int i = 0; i < contours_test.size(); i++)
		{
			box_test[i] = minAreaRect(Mat(contours_test[i]));  //计算每一帧的轮廓最小外接矩形
			//box_std[i] = minAreaRect(Mat(contours_std[i]));    //计算每一帧的轮廓最小外接矩形
			//boundRect_std[i] = boundingRect(Mat(contours_std[i]));
			boundRect_test[i] = boundingRect(Mat(contours_test[i]));

			if (boundRect_test[i].height > Max_length_test)			//if循环，定位test中人形前景，为后面计算高度和加矩形框使用
			{
				Max_length_test = boundRect_test[i].height;
				k = i;
			}
		}
		//cout << k << " boundrect" << boundRect_test[k].height << endl;

		circle(Rect_test, Point(box_test[k].center.x, box_test[k].center.y), 3, Scalar(0, 255, 0), -1, 8);			//绘制最小外接矩形的中心点
		circle(Body_center, Point(box_test[k].center.x, box_test[k].center.y), 3, Scalar(0, 255, 0), -1, 8);			//专门用一个矩阵“Body_center”来显示矩形框中心点的轨迹
		//imshow("Body_center", Body_center);

		//s_test = Rect_test;
		//chest_test_x = boundRect_test[k].x;
		//int value_test = 0;			//定义某点的像素值		

		for (int j = 0; j < contours_test[k].size(); j++)			//在轮廓区域中，找到高度为11/60高度的胸前点
		{
			if (contours_test[k][j].y == boundRect_test[k].height * 14 / 60)			// && contours_test[k][j].x < box_test[k].center.x
			{
				circle(Rect_test, Point(contours_test[k][j].x, contours_test[k][j].y), 3, 200, -1, 8);
				chest_test_x = contours_test[k][j].x;
				chest_test_y = contours_test[k][j].y;
				break;
			}
		}

		//while (value_test == 0 && chest_test_x < s_test.size().width - 1)		//在height*11/60的高度，从图像的左边开始向右寻找和身体的交线点
		//{
		//	chest_test_x++;
		//	value_test = s_test.at<uchar>(boundRect_test[k].height * 4 / 15, chest_test_x);
		//}
		circle(Rect_test, Point(chest_test_x, boundRect_test[k].height * 14 / 60), 3, 200, -1, 8);
		//rectangle(Rect_test, Point(boundRect_test[k].x, boundRect_test[k].y), Point(boundRect_test[k].x + boundRect_test[k].width, boundRect_test[k].y + boundRect_test[k].height), Scalar(255, 0, 0), 5, 8);

		//cout << "T = " << boundRect_test[k].width / 2 << endl;		//★★★test的动作周期★★★
		//CString tmpStr;
		tmpStr.Format(_T("%d"), boundRect_test[k].width / 2);
		infoStr.Append(_T("T = ") + tmpStr + _T(" \r\n"));
		editInfo.SetWindowTextW(infoStr);
		//cout << "chest_test_x = " << chest_test_x << endl;
		tmpStr.Format(_T("%d"), chest_test_x);
		infoStr.Append(_T("chest_test_x = " )+ tmpStr + _T(" \r\n"));
		editInfo.SetWindowTextW(infoStr);
		//imshow("Rect_test", Rect_test);		//播放test二值化图，是否带矩形框，标记232行rectangle函数来完成。

		//==============【围绕两图的胸点，设置ROI区域，两图以胸点为基准对接，std显示color图，test露出的部位显示绿色，计算重合率】===============
		
		Num_of_frame++;
		//cout << "Num_of_frame = " << Num_of_frame << endl;			//★★★当前显示的哪一帧★★★
		tmpStr.Format(_T("%d"), Num_of_frame);
		infoStr.Append(_T("Num_of_frame = ") + tmpStr + _T(" \r\n"));
		editInfo.SetWindowTextW(infoStr);
		//if (Num_of_frame < 10) continue;

		if (chest_test_x > 400) return true;
		Mat test_ROI = Rect_test(Rect(chest_test_x - 150, 0, 360, 480));
		threshold(test_ROI, test_ROI, 0, 255, CV_THRESH_BINARY);

		//imshow("test_ROI", test_ROI);
		testVideoWrite.write(test_ROI);

		//==================================【将处理后的test视频与标准模板进行比对，分析数据】===================================

		cvtColor(frame_std, frame_std, CV_BGR2GRAY);
		//cvtColor(test_ROI, test_ROI, CV_BGR2GRAY);
		threshold(frame_std, frame_std, 20, 255, CV_THRESH_BINARY);
		//threshold(test_ROI, test_ROI, 20, 255, CV_THRESH_BINARY);

		Mat add_frame = Mat::zeros(frame_std.rows, frame_std.cols, CV_8UC3);

		float overlap_area = 0;
		float test_body_area = 0;
		dif_imge(frame_std, test_ROI, add_frame, frame_color_std, overlap_area, test_body_area);

		//cout << "percent of pass = " << overlap_area / test_body_area << endl;			//★★★【test行进动作的重合率】★★★
		tmpStr.Format(_T("%f"), overlap_area / test_body_area);
		infoStr.Append(_T("percent of pass = = ") + tmpStr + _T("\r\n"));
		editInfo.SetWindowTextW(infoStr);

		//imshow("Result", add_frame);		//★★★主要展示的画面，std和test对比的画面★★★
		//Sleep(50);
		contrastVideoWriter.write(add_frame);
		CvvImage m_CvvImage;
		IplImage frame(add_frame);   //Mat 转IplImage
		m_CvvImage.CopyOf(&frame, 1); //复制该帧图像   
		m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect); //显示到设备的矩形框内
		//sliderVideo.SetPos(Num_of_frame);
		/*char c = waitKey(10);
		if (c == 'q')
			break;*/

		save_coincidence(Num_of_frame, overlap_area / test_body_area, boundRect_test[k].width / 2);
		return true;
	}
	
}
