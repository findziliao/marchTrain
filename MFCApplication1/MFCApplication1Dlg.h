
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "afxcmn.h"
#include "CvImgCtrl.h"
#include <stdio.h>
//#include <iostream>
//#include <iomanip>
//#include <sstream>
#include <fstream>
using namespace std;
//#include "resource.h"
// CMFMarchTrainDlg 对话框
class CMFMarchTrainDlg : public CDialogEx
{
// 构造
public:
	CMFMarchTrainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CvImgCtrl m_show;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
//	afx_msg void OnBnClickedCancel();
	//CButton m_Enable;
	//CButton m_Exit;
//	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedbtnopencamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedbtnclosecamera();
	afx_msg void OnBnClickedbtnnextpage();
	afx_msg void OnBnClickedbtnvideo();
	// 播放视频滑动条
//	CSliderCtrl sliderVideoPlay;
	afx_msg void OnBnClickedbtnopenvideo();
//	afx_msg void OnNMCustomdrawSlidervideoplay(NMHDR *pNMHDR, LRESULT *pResult);

	
	afx_msg void OnBnClickedbtnpauseplay();
	afx_msg void OnBnClickedstartplay();
	afx_msg void OnBnClickedbtnstopplay();
	
protected:

	afx_msg LRESULT OnFrame(WPARAM wParam, LPARAM lParam);
public:
	// 视频播放控件
	CSliderCtrl sliderVideo;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedbtnstartrecord();
	afx_msg void OnBnClickedbtnstoprecord();
	string cameraVideoSavePath;

	afx_msg void OnBnClickedbtnvideoanalyse();
	// 保存视频处理数据
	ofstream coin_rate_file;
	// 【利用图像G像素值-R像素值得到前景图】	
	void extract(const Mat& std, Mat& outing);
	// 输入二值图，输出将二值的白色变成绿色
	void dif_imge(const Mat& std, const Mat& test, Mat& outimg, const Mat& input, float& area, float& body_area);
	void save_coincidence(int Num_frame, float coincidence_rate, int middle_dot_width);
};