
// MFCApplication1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
//#include "resource.h"
// CMFMarchTrainDlg �Ի���
class CMFMarchTrainDlg : public CDialogEx
{
// ����
public:
	CMFMarchTrainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
};
