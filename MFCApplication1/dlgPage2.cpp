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
END_MESSAGE_MAP()


// dlgPage2 消息处理程序


void dlgPage2::OnBnClickedbtnprepage2()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();

}
