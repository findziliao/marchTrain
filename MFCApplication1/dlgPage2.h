#pragma once


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
};
