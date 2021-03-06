
// clientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "login.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
protected:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	

END_MESSAGE_MAP()


// CclientDlg 对话框
CString jstr;
void CclientDlg::ReceiveData()
{
	char buffer[BUFFERSIZE];
	//接收传来的数据
	int factdata = pmysocket->Receive(buffer, BUFFERSIZE);

	buffer[factdata] = '\0';
	CString str;
	str.Format("%s", buffer);
	CEdit* pEdit = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INFO);
	pEdit->SetSel(pEdit->GetWindowTextLength(), false);

	jstr += (str += "\r\n");
	pEdit->ReplaceSel(jstr);
	//输入焦点设置
	CEdit* pEdita = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_input);
	pEdita->SetWindowText("");
	pEdita->SetFocus();
}





CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	,pmysocket(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}






void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_info);
	DDX_Control(pDX, IDC_EDIT_input, m_input);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CclientDlg::OnBnClickedOk)
	ON_MESSAGE(WM_SUBCLOSE, &CclientDlg::OnSubclose)
	ON_BN_CLICKED(IDC_BTN_VIDEOWINDOW, &CclientDlg::OnBnClickedBtnVideowindow)
END_MESSAGE_MAP()


// CclientDlg 消息处理程序

BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

a:login logindlg;
	INT_PTR result = logindlg.DoModal();
	if (result == IDOK)
	{
		pmysocket = new mysocket(this); //创建套接字对象

		if (!pmysocket->Create()) //创建套接字
		{
			delete pmysocket;
			MessageBox("套接字创建失败.");
			return false;
		}
		CString m_ipserver = logindlg.m_ipserver; //读取服务器名称
		int m_iport;
		m_iport = logindlg.m_iport; //获取端口

		m_id = logindlg.m_id; //获取用户名


		if (!pmysocket->Connect(m_ipserver, m_iport)) //连接服务器
		{
			if (MessageBox("连接服务器失败,是否尝试重新连接?", "提示", MB_YESNO) == IDYES)
			{
				delete pmysocket;
				goto a; //重新连接
			}
		}
		//子窗口SOCKET 初始化；
		if (pvideoDlg == nullptr)
		{
			pvideoDlg = std::make_shared<videodlg>();
			pvideoDlg->Create(IDD_DLG_VIDEOCLIENT);
			pvideoDlg->ShowWindow(SW_HIDE);			
		}
	}
	else
		return false;



	CString str;
	str.Format("%s----->%s \r\n", (LPCTSTR)m_id, (LPCTSTR)"进入聊天室");
	int num = pmysocket->Send(str.GetBuffer(0), str.GetLength());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CclientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, temp;
	m_input.GetWindowText(str);
	if (str.IsEmpty() | m_id.IsEmpty())
		return;
	temp.Format("%s说: %s \r\n", (LPCTSTR)m_id, (LPCTSTR)str);
	int num = pmysocket->Send(temp.GetBuffer(temp.GetLength()), temp.GetLength());

}







afx_msg LRESULT CclientDlg::OnSubclose(WPARAM wParam, LPARAM lParam)
{
	if (pvideoDlg)
	{
		pvideoDlg.reset();
	}
	return 0;
}


void CclientDlg::OnBnClickedBtnVideowindow()
{
	// TODO: 在此添加控件通知处理程序代码
		if (pvideoDlg)
	{
		pvideoDlg->ShowWindow(SW_SHOW);
	}
}
