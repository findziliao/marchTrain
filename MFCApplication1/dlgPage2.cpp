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
END_MESSAGE_MAP()


// dlgPage2 ��Ϣ�������


void dlgPage2::OnBnClickedbtnprepage2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();

}
