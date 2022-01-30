// login.cpp: 实现文件
//

#include "pch.h"
#include "client.h"
#include "login.h"
#include "afxdialogex.h"


// login 对话框

IMPLEMENT_DYNAMIC(login, CDialog)

login::login(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LOGIN_DIALOG, pParent)
	, m_id(_T(""))
	, m_ipserver(_T("192.168.1.168"))
	, m_iport(5000)
	, m_iport_video(5001)
{

}

login::~login()
{
}

void login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_IPSER, m_ipserver);
	DDX_Text(pDX, IDC_EDIT_IPORT, m_iport);
	DDX_Text(pDX, IDC_EDIT_IPORT_VIDEO, m_iport_video);
}


BEGIN_MESSAGE_MAP(login, CDialog)
	ON_BN_CLICKED(IDOK, &login::OnBnClickedOk)
END_MESSAGE_MAP()


// login 消息处理程序


void login::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
