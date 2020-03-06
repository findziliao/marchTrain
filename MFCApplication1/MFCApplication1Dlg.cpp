
// MFCApplication1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
//#include "CvImgCtrl.h"
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

//CvVideoWriter* cameraVideoWriter = 0;
VideoWriter cameraVideoWriter;
//������
//int   G_slider_position = 0;
//CvCapture*    G_capture = NULL;
//cv::VideoCapture*    G_capture   =  NULL;
////HBITMAP hbitmap;
//CRect rect;
//CStatic* pStc_PictureForVideo; //��ʶͼ����ʾ��Picture�ؼ�
//CDC* pDC; //��Ƶ��ʾ�ؼ��豸������
//HDC hDC; //��Ƶ��ʾ�ؼ��豸���

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFMarchTrainDlg �Ի���



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


// CMFMarchTrainDlg ��Ϣ�������

BOOL CMFMarchTrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	//showImage_pwnd = GetDlgItem(IDC_ShowImage);	
	//showImage_pDC = showImage_pwnd->GetDC();	  
	//showImage_hDC = showImage_pDC->GetSafeHdc();
	//showImage_pwnd->GetClientRect(&showImage_clientRect);

	//pStc_PictureForVideo = (CStatic *)GetDlgItem(IDC_ShowImage);//IDC_VIEWΪPicture�ؼ�ID
	//pStc_PictureForVideo->GetClientRect(&showImage_clientRect);//Picture�Ĵ�С��������rect
	//pDC = pStc->GetDC(); //�õ�Picture�ؼ��豸������
	//hDC = pDC->GetSafeHdc(); //�õ�Picture�ؼ��豸�����ĵľ�� 
	//hbitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));//����λͼ��Դ
	//pStc_PictureForVideo->ModifyStyle(1, SS_BITMAP);//�޸Ŀؼ�������
	//pStc_PictureForVideo->SetBitmap(hbitmap);//��ʾλͼ
	/*CDC MemDC;
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1);
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	showImage_pDC->StretchBlt(showImage_clientRect.left, showImage_clientRect.top, showImage_clientRect.Width(), showImage_clientRect.Height(), &MemDC, 0, 0, 48, 48, SRCCOPY);
	*/
	//m_Exit.EnableWindow(FALSE);
	//���ӵ��ؼ�
	//�󶨴���
	//CWnd  *pWnd1 = GetDlgItem(IDC_ShowImage);//CWnd��MFC������Ļ���,�ṩ��΢�������������д�����Ļ������ܡ�
	//pWnd1->GetClientRect(&showImage_clientRect);//GetClientRectΪ��ÿؼ�������������С
	//namedWindow("VideoBar", WINDOW_AUTOSIZE);//���ô�����
	//HWND hWndl = (HWND)cvGetWindowHandle("VideoBar");//hWnd ��ʾ���ھ��,��ȡ���ھ��
	//HWND hParent1 = ::GetParent(hWndl);//GetParent����һ��ָ���Ӵ��ڵĸ����ھ��
	//::SetParent(hWndl, GetDlgItem(IDC_ShowImage)->m_hWnd);
	//::ShowWindow(hParent1, SW_HIDE);//ShowWindowָ����������ʾ
	m_show.linkDlgItem(IDC_ShowImage, this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFMarchTrainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFMarchTrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CMFMarchTrainDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(_T("ȷ���˳���"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		return;
	}
	cvReleaseCapture(&camera_capture);
	CDialogEx::OnClose();
}









void CMFMarchTrainDlg::OnBnClickedbtnopencamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_show.openVideo(0);
	//����Ϊ������ʾ
	m_show.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//if (!camera_capture)
	//{
	//	camera_capture = cvCaptureFromCAM(0);
	//	//AfxMessageBox("OK");
	//}

	//// ����
	//IplImage* m_Frame;
	//m_Frame = cvQueryFrame(camera_capture);
	//CvvImage m_CvvImage;
	//m_CvvImage.CopyOf(m_Frame, 1);
	//if (true)
	//{
	//	m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect);
	//	//cvWaitKey(10);
	//}

	//// ���ü�ʱ��,ÿ10ms����һ���¼�
	//SetTimer(1, 10, NULL);
	
}


void CMFMarchTrainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	

	//switch (nIDEvent)//nIDEvent Ϊ��ʱ���¼�ID��1��2��3
	//{
	//case 1:
	//	{
	//		IplImage* m_Frame;
	//		m_Frame = cvQueryFrame(camera_capture);
	//		CvvImage m_CvvImage;
	//		m_CvvImage.CopyOf(m_Frame, 1);
	//		if (true)
	//		{
	//			m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect);
	//			cvWriteFrame(cameraVideoWriter, m_Frame);
	//			//cvWaitKey(10);
	//		}
	//	}
	//	break;
	//case 2:
	//	{		
	//		Mat img;
	//		video_capture >> img;  //ȡ��һ֡ͼ��
	//		if (img.empty())
	//		{
	//			KillTimer(2);
	//			video_capture.release();
	//			AfxMessageBox(_T("��Ƶ����"));
	//			//video_capture.release();
	//		}
	//		else
	//		{
	//			CvvImage m_CvvImage;
	//			IplImage frame(img);   //Mat תIplImage
	//			m_CvvImage.CopyOf(&frame, 1); //���Ƹ�֡ͼ��   
	//			m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect); //��ʾ���豸�ľ��ο���
	//			sliderVideoPlay.SetPos((int)video_capture.get(CV_CAP_PROP_POS_FRAMES));//������Ƶ��λ��
	//		}						  
	//	}
	//	break;
	//}

	
	CDialogEx::OnTimer(nIDEvent);
}


void CMFMarchTrainDlg::OnBnClickedbtnclosecamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFMarchTrainDlg::OnBnClickedbtnvideo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFMarchTrainDlg::OnBnClickedbtnopenvideo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString caption;
	//GetDlgItemText(IDC_btnOpenVideo, caption);
	//if (caption == _T("�ر���Ƶ"))    //�ر���Ƶ��ť
	//{
	//	KillTimer(2);
	//	video_capture.release();
	//	SetDlgItemText(IDC_btnOpenVideo, _T("����Ƶ"));
	//	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(FALSE);  //������ʧЧ
	//	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->SetPos(0); //���û�����λ��
	//	//pStc_PictureForVideo->SetBitmap(hbitmap); //�ָ�λͼ��Դ
	//	return;
	//}


	//����Ƶ,����ͨ�öԻ���,ѡ��Ҫ���ŵ���Ƶ�ļ�
	string _Path;
	CFileDialog Openfiledlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (Openfiledlg.DoModal() == IDOK) //����ģ̬�Ի���
	{
		//CString����ת��Ϊstring����
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
		AfxMessageBox(_T("�޷�����Ƶ��"));
		return;
	}
	else
	{
			//��ȡ��Ƶ��֡��
		Mat img;
		int frameCount = 0;

		do
		{
			m_show.getVideoCapture()->read(img);
			frameCount++;
		} while (!img.empty());
		((CSliderCtrl *)GetDlgItem(IDC_SLIDERVideo))->EnableWindow(TRUE);  //�������
		int sliderRange = frameCount;
		//int sliderRange = 100;
		sliderVideo.SetRange(0, sliderRange);//���û������ķ�Χ,Ϊ��Ƶ����֡��
		m_show.closeVideo();
		m_show.openVideo(_Path.c_str());  //���´���Ƶ
		//������ʾ֡��
		m_show.setWaitTime((int)(1000 * 1.0 / m_show.getVideoCapture()->get(CV_CAP_PROP_FPS)));
		//����Ϊ������ʾ
		m_show.setResizeType(CvImgCtrl::ResizeType_CenterResize);
		
		
	}
	
}


//void CMFMarchTrainDlg::OnNMCustomdrawSlidervideoplay(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	//m_show.getVideoCapture()->set(CV_CAP_PROP_POS_FRAMES, sliderVideoPlay.GetPos());  //������Ƶ����ʼ֡
//	*pResult = 0;
//}


void CMFMarchTrainDlg::OnBnClickedbtnpauseplay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_show.pauseVideo();
}


void CMFMarchTrainDlg::OnBnClickedstartplay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_show.resumeVideo();
}


void CMFMarchTrainDlg::OnBnClickedbtnstopplay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_show.closeVideo();
	//((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(FALSE);  //������ʧЧ
	//((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->SetPos(0); //���û�����λ��
}

//����ÿһ֡����Ƶ


afx_msg LRESULT CMFMarchTrainDlg::OnFrame(WPARAM wParam, LPARAM lParam)
{
	//Mat frame((IplImage*)lParam, false);
	//������������·�ת
	//flip(frame, frame, -1);
	sliderVideo.SetPos((int)m_show.getVideoCapture()->get(CV_CAP_PROP_POS_FRAMES));//������Ƶ��λ��
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
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	
//	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
//}


void CMFMarchTrainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//m_show.pauseVideo();
	//CString strTest;
	//strTest.Format(_T("%d"), sliderVideo.GetPos());
	//AfxMessageBox(_T("��ת��֡����") + strTest);
	m_show.getVideoCapture()->set(CV_CAP_PROP_POS_FRAMES, sliderVideo.GetPos());
	//Mat img;
	//m_show.getVideoCapture()->read(img);  //ȡ��һ֡ͼ��
	//if (img.empty())
	//{
	//	KillTimer(2);
	//	m_show.getVideoCapture()->release();
	//	AfxMessageBox(_T("��Ƶ����"));
	//	//video_capture.release();
	//}
	//else
	//{
	//	CvvImage m_CvvImage;
	//	IplImage frame(img);   //Mat תIplImage
	//	m_CvvImage.CopyOf(&frame, 1); //���Ƹ�֡ͼ��   
	//	m_CvvImage.DrawToHDC(showImage_hDC, &showImage_clientRect); //��ʾ���豸�ľ��ο���		
	//}	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMFMarchTrainDlg::OnBnClickedbtnstartrecord()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!m_show.getVideoCapture()->isOpened())
	{
		AfxMessageBox(_T("����ͷû�򿪣�"));
		return;
	}
	//����Ƶ,����ͨ�öԻ���,ѡ��Ҫ���ŵ���Ƶ�ļ�

	CFileDialog saveFiledlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (saveFiledlg.DoModal() == IDOK) //����ģ̬�Ի���
	{
		//CString����ת��Ϊstring����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//cvReleaseVideoWriter(&cameraVideoWriter);
	cameraVideoWriter.release();
}


void CMFMarchTrainDlg::OnBnClickedbtnvideoanalyse()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}




// �����ֵͼ���������ֵ�İ�ɫ�����ɫ
//Mat CMFMarchTrainDlg::convertTo3Channels(const Mat& binImg)
//{
//	return Mat();
//}


// ������ͼ��G����ֵ-R����ֵ�õ�ǰ��ͼ��
void CMFMarchTrainDlg::extract(const Mat& std, Mat& outing)
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
			float dif = std_p[j][1] - std_p[j][2];
			if (dif > 70)
				out[j][0] = out[j][1] = out[j][2] = 0;
			else
				out[j][0] = out[j][1] = out[j][2] = 255;
		}

	}
}


// �����ֵͼ���������ֵ�İ�ɫ�����ɫ
void CMFMarchTrainDlg::dif_imge(const Mat& std, const Mat& test, Mat& outimg, const Mat& input, float& area, float& body_area)
{
	int nr = std.rows;
	int nc = std.cols;
	input.copyTo(outimg);
	for (int i = 0; i < nr; ++i)
	{
		const uchar *s = std.ptr<uchar>(i);     //��ȡstd��i�е�һ�����ص�ָ��
		const uchar *t = test.ptr<uchar>(i);    //��ȡtest��i�е�һ�����ص�ָ��
		Vec3b* out = outimg.ptr<Vec3b>(i);

		for (int j = 0; j < nc; ++j)
		{
			if (s[j] == 0 && t[j] == 255)
			{
				out[j][0] = 0;
				out[j][1] = 255;
				out[j][2] = 0;
			}
			//�غ����,�ڶ�ֵͼ����£���std��test��ĳ���ϵ�����ֵ����255ʱ��˵�������������ͼƬ���غϵ�

			if (t[j] == 255)
			{
				++body_area;		//testȫ��������һ��
				if (s[j] == 255)
				{
					++area;			//ȫ���غϵ����
				}
			}

		}

	}
}


void CMFMarchTrainDlg::save_coincidence(int Num_frame, float coincidence_rate, int middle_dot_width)
{
	coin_rate_file << Num_frame << " " << coincidence_rate << " " << middle_dot_width << " ";
	coin_rate_file << "\n";
}
