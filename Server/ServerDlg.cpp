
// ServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
int m_id = 0;
int idmunber[10];



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerDlg 对话框

CString jstr;
CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, u_m_port(5000)
	, c_m_host_name(_T(""))
	,m_pClient()
	,m_pSocket()
	,m_input()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CServerDlg::ReceiveData(ClientSocket* socket)
{

	char bufferdata[BUFFERSIZE];
	//接收客户端传来色数据
	int result = socket->Receive(bufferdata, BUFFERSIZE);
	bufferdata[result] = 0;

	CString str;
	str.Format(_T("%s \r\n"), (LPCTSTR)bufferdata);
	CEdit* pEdit = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INFO);
	pEdit->SetSel(pEdit->GetWindowTextLength(), false);
	jstr += str;
	pEdit->ReplaceSel(jstr);
	POSITION pos = m_socketlist.GetHeadPosition();
	//将数据发送给每个客户端
	while (pos != NULL)
	{
		ClientSocket* socket = (ClientSocket*)m_socketlist.GetNext(pos);

		if (socket != NULL)
		{	
			socket->Send(bufferdata, result);
		}
		
	}
}


void CServerDlg::ReceviveVideo(videosocket* socket)
{
	int nRecvBuf = 1024 * 1024 * 10;//接收缓存10M  
	setsockopt((SOCKET)socket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	videodate data;
	int result = socket->Receive((char*)(&data), blocksize + 4);
	
	POSITION pos = m_socketVideolist.GetHeadPosition();
	//将数据发送给每个客户端
	while (pos != NULL)
	{
		videosocket* socket = (videosocket*)m_socketVideolist.GetNext(pos);

		if (socket != NULL)
		{
			int nRecvBuf = 1024 * 1024 * 10;//接收缓存10M  
			int xxx = setsockopt((SOCKET)*socket, SOL_SOCKET, SO_SNDBUF, (const char*)&nRecvBuf, sizeof(int));
			TRACE("/r/n%d/r/n", xxx);
			socket->Send((char*)(&data), blocksize + 4);
		}

	}
}

void CServerDlg::AcceptConnect()
{
	ClientSocket* socket = new ClientSocket(this);
	//接受客户框的连接
	if (m_pSocket->Accept(*socket))
	{
		m_socketlist.AddTail(socket);
	}
	else
		delete socket;
	videosocket* socketvideo = new videosocket(this);
	if (m_videosocket->Accept(*socketvideo))
	{
		m_socketVideolist.AddTail(socketvideo);
	}
	else
	{
		delete socketvideo;
	}
}






void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, u_m_port);
	DDX_Text(pDX, IDC_EDIT_HOST_NAME, c_m_host_name);
	DDX_Control(pDX, IDC_IP_HOST, ip_m_idc);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_input);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_VIDEO, &CServerDlg::OnBnClickedBtnVideo)
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(BTN_CLose, &CServerDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON3, &CServerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_VIDEO_CLOSE, &CServerDlg::OnBnClickedBtnVideoClose)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
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
	MoveWindow(0, 0, 600, 530, true);
	char szHostName[MAX_PATH] = { 0 };
	int nRetCode;
	nRetCode = gethostname(szHostName, sizeof(szHostName));
	c_m_host_name = szHostName;

	ip_m_idc.SetAddress(BYTE(192), BYTE(168), BYTE(1), BYTE(168));
	/*GetMacAdress();*/
	UpdateData(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CServerDlg::OnPaint()
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
		CPaintDC dc(this); // 用于绘制的设备上下文

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	
	return static_cast<HCURSOR>(m_hIcon);
}





void CServerDlg::OnBnClickedBtnVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	cv::namedWindow("本地视频", cv::WINDOW_NORMAL);
	cv::Mat img;
	cv::VideoCapture videoa(0);
	while (true) {
		videoa >> img;
		if ((img.cols <= 0) || (img.rows <= 0)) {
			break;
		}

		cv::imshow("本地视频", img);
		char c = cv::waitKey(30);
		if (c==27)
		{
			break;
		}
	}
	cv::destroyWindow("本地视频");
}



void CServerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData();
	m_pSocket = new ServerSocket(this);
	if (!m_pSocket->Create(u_m_port))
	{
		MessageBox("套接字创建失败");
		delete m_pSocket;
		m_pSocket = NULL;
		return;
	}

	if (!m_pSocket->Listen())
	{
		MessageBox("监听失败");

		return;
	}


	m_videosocket = new videosocket(this);
	if (!m_videosocket->Create(5001))
	{
		MessageBox("套接字创建失败");
		delete m_videosocket;
		m_videosocket = NULL;
		return;
	}

	if (!m_videosocket->Listen())
	{
		MessageBox("监听失败");
		
		return;
	}

}


void CServerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	OnOK();
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP_HOST)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HOST_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
}


void CServerDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pSocket)
		delete m_pSocket;
	if (m_videosocket)
		delete m_videosocket;
	OnCancel();
}





void CServerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, m_id="管理员",temp;

	m_input.GetWindowText(str);

	if (str.IsEmpty())
		return;
	temp.Format("%s说: %s \r\n", (LPCTSTR)m_id, (LPCTSTR)str);
	//将数据发送给每个客户端
	POSITION pos = m_socketlist.GetHeadPosition();
	while (pos != NULL)
	{
		ClientSocket* socket = (ClientSocket*)m_socketlist.GetNext(pos);
		if (socket != NULL)
			socket->Send(temp.GetBuffer(temp.GetLength()), temp.GetLength());
	}
	CEdit* pEdit = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INFO);
	pEdit->SetSel(pEdit->GetWindowTextLength(), false);
	jstr += ("管理员说:"+str + "\r\n");
	pEdit->ReplaceSel(jstr);
	

	//输入焦点设置
	CEdit* pEdita = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_EDIT_INPUT);
	pEdita->SetWindowText("");
	pEdita->SetFocus();

}


void CServerDlg::OnBnClickedBtnVideoClose()
{
	// TODO: 在此添加控件通知处理程序代码

}
