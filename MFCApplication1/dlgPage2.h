#pragma once
#include "CvImgCtrl.h"
using namespace std;

// dlgPage2 �Ի���

class dlgPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(dlgPage2)

public:
	dlgPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~dlgPage2();

// �Ի�������
	enum { IDD = IDD_DIALOGPage2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnprepage2();
	afx_msg void OnBnClickedbtnclose();
	CvImgCtrl m_showPritive; //��ʾԭʼ������Ƶ
	CvImgCtrl m_showROI;   //��ʾ��ȡ��ǰ����Ƶ
	CvImgCtrl m_showOverlay;    //��ʾ������Ƶ
	virtual BOOL OnInitDialog();
	string primitiveVideoFile;
	string ROIVideoFile;
	string overlayVideoFile;
	// ��3����Ƶ
	void open3Video();
	afx_msg void OnBnClickedbtnopen3video();
};
