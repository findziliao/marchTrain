#pragma once


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
};
