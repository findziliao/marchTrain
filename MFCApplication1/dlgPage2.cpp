// dlgPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "dlgPage2.h"
#include "afxdialogex.h"


// dlgPage2 对话框

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


// dlgPage2 消息处理程序


void dlgPage2::OnBnClickedbtnprepage2()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();

}


void dlgPage2::OnBnClickedbtnclose()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL dlgPage2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_showPritive.linkDlgItem(IDC_picPrimitive, this);
	m_showROI.linkDlgItem(IDC_picROI, this);
	m_showOverlay.linkDlgItem(IDC_picOverlay, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


// 打开3个视频
void dlgPage2::open3Video()
{
	//打开原始视频
	m_showPritive.openVideo(primitiveVideoFile.c_str());
	m_showPritive.setWaitTime((int)(1000 * 1.0 / m_showPritive.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//设置为居中显示
	m_showPritive.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//打开提取的前景视频
	m_showROI.openVideo(ROIVideoFile.c_str());
	m_showROI.setWaitTime((int)(1000 * 1.0 / m_showROI.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//设置为居中显示
	m_showROI.setResizeType(CvImgCtrl::ResizeType_CenterResize);
	//打开叠加视频
	m_showOverlay.openVideo(overlayVideoFile.c_str());
	m_showOverlay.setWaitTime((int)(1000 * 1.0 / m_showOverlay.getVideoCapture()->get(CV_CAP_PROP_FPS)));
	//设置为居中显示
	m_showOverlay.setResizeType(CvImgCtrl::ResizeType_CenterResize);
}


void dlgPage2::OnBnClickedbtnopen3video()
{
	// TODO:  在此添加控件通知处理程序代码
	open3Video();
}
