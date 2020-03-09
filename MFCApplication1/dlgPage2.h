#pragma once
#include "CvImgCtrl.h"
using namespace std;

// dlgPage2 对话框

class dlgPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(dlgPage2)

public:
	dlgPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~dlgPage2();

// 对话框数据
	enum { IDD = IDD_DIALOGPage2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnprepage2();
	afx_msg void OnBnClickedbtnclose();
	CvImgCtrl m_showPritive; //显示原始测试视频
	CvImgCtrl m_showROI;   //显示提取的前景视频
	CvImgCtrl m_showOverlay;    //显示叠加视频
	virtual BOOL OnInitDialog();
	string primitiveVideoFile;
	string ROIVideoFile;
	string overlayVideoFile;
	// 打开3个视频
	void open3Video();
	afx_msg void OnBnClickedbtnopen3video();
};
