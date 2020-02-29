
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

CvVideoWriter* cameraVideoWriter = 0;

VideoCapture video_capture;
//HBITMAP hbitmap;
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
	DDX_Control(pDX, IDC_SLIDER_videoPlay, sliderVideoPlay);
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_videoPlay, &CMFMarchTrainDlg::OnNMCustomdrawSlidervideoplay)
	ON_BN_CLICKED(IDC_btnPausePlay, &CMFMarchTrainDlg::OnBnClickedbtnpauseplay)
	ON_BN_CLICKED(IDC_startPlay, &CMFMarchTrainDlg::OnBnClickedstartplay)
	ON_BN_CLICKED(IDC_btnStopPlay, &CMFMarchTrainDlg::OnBnClickedbtnstopplay)
//	ON_MESSAGE(MsgVideoFrame, &CMFMarchTrainDlg::OnFrame)
ON_MESSAGE(MsgVideoFrame, &CMFMarchTrainDlg::OnFrame)
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

	//if (!camera_capture)
	//{
	//	AfxMessageBox(_T("�޷�������ͷ"));
	//	return;
	//}
	//cameraVideoWriter = cvCreateVideoWriter("MyVideo.avi", CV_FOURCC('x', 'v', 'I', 'D'), 25, cvSize(640, 480));
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

	m_show.setWaitTime((int)(1000 * 1.0 / m_show.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//����Ϊ������ʾ
	m_show.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	
	if (!m_show.getVideoCapture()->isOpened())
	{
		AfxMessageBox(_T("�޷�����Ƶ��"));
		return;
	}
	else
	{
		((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(TRUE);  //�������
		sliderVideoPlay.SetRange(0, (int)m_show.getVideoCapture()->get(CV_CAP_PROP_FRAME_COUNT));//���û������ķ�Χ,Ϊ��Ƶ����֡��
		
	}
	
}


void CMFMarchTrainDlg::OnNMCustomdrawSlidervideoplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_show.getVideoCapture()->set(CV_CAP_PROP_POS_FRAMES, sliderVideoPlay.GetPos());  //������Ƶ����ʼ֡
	*pResult = 0;
}


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
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->EnableWindow(FALSE);  //������ʧЧ
	((CSliderCtrl *)GetDlgItem(IDC_SLIDER_videoPlay))->SetPos(0); //���û�����λ��
}

//����ÿһ֡����Ƶ


afx_msg LRESULT CMFMarchTrainDlg::OnFrame(WPARAM wParam, LPARAM lParam)
{
	//Mat frame((IplImage*)lParam, false);
	//������������·�ת
	//flip(frame, frame, -1);
	sliderVideoPlay.SetPos((int)m_show.getVideoCapture()->get(CV_CAP_PROP_POS_FRAMES));//������Ƶ��λ��

	return 0;
}
