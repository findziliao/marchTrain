// dlgPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "dlgPage2.h"
#include "afxdialogex.h"


// dlgPage2 �Ի���

IMPLEMENT_DYNAMIC(dlgPage2, CDialogEx)

dlgPage2::dlgPage2(CWnd* pParent /*=NULL*/)
	: CDialogEx(dlgPage2::IDD, pParent)
{

}

dlgPage2::~dlgPage2()
{
}

void dlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dlgPage2, CDialogEx)
	ON_BN_CLICKED(ID_btnPrePage2, &dlgPage2::OnBnClickedbtnprepage2)
	ON_BN_CLICKED(ID_btnClose, &dlgPage2::OnBnClickedbtnclose)
	ON_BN_CLICKED(IDC_btnOpen3Video, &dlgPage2::OnBnClickedbtnopen3video)
END_MESSAGE_MAP()


// dlgPage2 ��Ϣ�������


void dlgPage2::OnBnClickedbtnprepage2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();

}


void dlgPage2::OnBnClickedbtnclose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


BOOL dlgPage2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_showPritive.linkDlgItem(IDC_picPrimitive, this);
	m_showROI.linkDlgItem(IDC_picROI, this);
	m_showOverlay.linkDlgItem(IDC_picOverlay, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


// ��3����Ƶ
void dlgPage2::open3Video()
{
	//��ԭʼ��Ƶ
	m_showPritive.openVideo(primitiveVideoFile.c_str());
	m_showPritive.setWaitTime((int)(1000 * 1.0 / m_showPritive.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//����Ϊ������ʾ
	m_showPritive.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//����ȡ��ǰ����Ƶ
	m_showROI.openVideo(ROIVideoFile.c_str());
	m_showROI.setWaitTime((int)(1000 * 1.0 / m_showROI.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//����Ϊ������ʾ
	m_showROI.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//�򿪵�����Ƶ
	m_showOverlay.openVideo(overlayVideoFile.c_str());
	m_showOverlay.setWaitTime((int)(1000 * 1.0 / m_showOverlay.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//����Ϊ������ʾ
	m_showOverlay.setResizeType(CvImgCtrl::ResizeType_CenterResize);
}


void dlgPage2::OnBnClickedbtnopen3video()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	open3Video();
}
