// videodlg.cpp: 实现文件
//

#include "pch.h"
#include "client.h"
#include "videodlg.h"
#include "afxdialogex.h"

cv::VideoCapture Capture(0);
// videodlg 对话框
BOOL videodlg::m_bcap = FALSE;
//const int BUFFERSIZE = 28800 + 4;
char  videodlg::img[blocksize * 32] = { 0 };
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
videodate videodlg::data = {};


IMPLEMENT_DYNAMIC(videodlg, CDialogEx)
videodlg::videodlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_VIDEOCLIENT, pParent)
	,pmyvideosocket(nullptr)
	,threadHandle(nullptr)
{

}

videodlg::~videodlg()
{
}

void videodlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool videodlg::capinit()
{
	capture.open(0);
	//capture.open("3.mkv");
	if (capture.isOpened())
	{
		TRACE("摄像头已经打开！！并将摄像头文件输入到  类成员 capimg 变量中！ 返回真");
		return true;
	}
	else
	{
		return false;
	}
}

void videodlg::OnBnClickedBtnExit()
{
	GetParent()->PostMessage(WM_SUBCLOSE, 0, 0);
}

void videodlg::OnTimer(UINT_PTR nIDEvent)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{

		Capture >> v_frame;											//从摄像头或者文件中抓取并返回一帧  					
		v_frame.copyTo(m_grabframe);								//复制该帧图像   
		DrawPicToHDC(m_grabframe, IDC_PIC_SUB);						//显示到设备的矩形框内  
		}
		break;
	case 2:
		{

			if (data.flag == 2)
			{
	
				static cv::Mat frame(cv::Size(640, 480), CV_8UC3);
				frame.data = (uchar*)img;
				frame.copyTo(m_grabframe);
				DrawPicToHDC(m_grabframe, IDC_PIC_SUB);

			
			}
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);


}

void videodlg::DrawPicToHDC(cv::Mat p, UINT IDC)
{
	cv::Mat p1;															//opencv中的图像
	CImage img;
	MatCImage mi;													//mat和cimage互相转化的类
	CRect rect;														//定义矩形类

	//mi.CImageToMat(img, p1);										//将cimage转变为mat

	CWnd* pWnd = GetDlgItem(IDC);									//获取控件句柄  
	pWnd->GetClientRect(&rect);										//获取句柄指向控件区域的大小  
	CDC* pDc = pWnd->GetDC();										//获取picture控件的DC  
	int win_w = rect.Width(), win_h = rect.Height();				//获取控件窗口宽高
	pDc->SetStretchBltMode(COLORONCOLOR);

	resize(p, p1, cv::Size(win_w, win_h));								//调整图片大小。
	mi.MatToCImage(p1, img);										//将mat转变为cimage
	img.Draw(pDc->m_hDC, 0, 0, win_w, win_h, 0, 0, win_w, win_h);	//画出图片
	ReleaseDC(pDc);
}

void videodlg::OnBnClickedBtnVideoshow()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Capture.isOpened() == 0)
	{
		MessageBox(NULL, "无法打开摄像头");
		return;
	}
	Capture >> v_frame;
	/*	v_frame = cvQueryFrame(Capture);	*/							//从摄像头或者文件中抓取并返回一帧  
	normalize(v_frame, m_grabframe, 0, 255, cv::NORM_MINMAX, -1);		//将mat数据归一化到0—255内   
	DrawPicToHDC(m_grabframe, IDC_PIC_SUB);							//显示到设备的矩形框内  

	SetTimer(1, 25, NULL);											//定时器，定时时间和帧率一致 
}

void videodlg::OnBnClickedBtnClosevideo()
{
	// TODO: 在此添加控件通知处理程序代码


	if (!Capture.isOpened())													//判断摄像头状态

	{
		MessageBoxA(NULL, "没有打开摄像头！！");
		return;
	}

	/*cvReleaseCapture(&Capture);*/
	//Capture.release();

	v_frame = cv::imread("lena.jpg");				//图片读取路径可以自己设定  
	v_frame.copyTo(m_grabframe);									//复制该帧图像    
	DrawPicToHDC(m_grabframe, IDC_PIC_SUB);							//显示到设备的矩形框内   
	KillTimer(1);													//关闭定时器
}

void videodlg::OnBnClickedBtnRemote()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(2, 25, NULL);
}


BEGIN_MESSAGE_MAP(videodlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, &videodlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(ID_BTN_VIDEOSHOW, &videodlg::OnBnClickedBtnVideoshow)
	ON_BN_CLICKED(ID_BTN_CLOSEVIDEO, &videodlg::OnBnClickedBtnClosevideo)
	ON_BN_CLICKED(IDC_BTN_REMOTE, &videodlg::OnBnClickedBtnRemote)
	ON_BN_CLICKED(IDC_BTN_APPLIVE, &videodlg::OnBnClickedBtnApplive)
END_MESSAGE_MAP()


void videodlg::ReceiveVieo()
{

	int nRecvBuf = 1024 * 1024 * 10;//接收缓存10M  
	int xxx = setsockopt((SOCKET)*pmyvideosocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	int factdata = pmyvideosocket->Receive((char*)(&data), blocksize + 4);
	static int i = 0;

	cv::Mat frame(cv::Size(640, 480), CV_8UC3);

	static int  COUNT = 0;
	COUNT = COUNT + data.flag;
	for (int k = 0; k < blocksize; k++)
	{
		img[i * blocksize + k] = data.buf[k];
		frame.data = (uchar*)img;
	}
	i++;
	if (i == 32)
	{
		i = 0;
	}
	if (data.flag == 2)  //data.flag==2是一帧中的最后一个数据块  
	{
		if (COUNT == 33)
		{
			/*	cv::namedWindow("client");
				imshow("client", frame);*/

			char c = cv::waitKey(1000 / 300);
			if (c == 27)return;
		}
		else
		{
			COUNT = 0;
			data.flag = 1;
		}
	}


}

// videodlg 消息处理程序
DWORD __stdcall videodlg::threadwork(LPVOID pParam)
{
	TRACE("\r\n ni  \r\n ");
	videodlg* pmaindlg = reinterpret_cast<videodlg*>(pParam);
	if (pmaindlg == nullptr) return -1;
	if (m_bcap)  
		pmaindlg->capinit();
	//pmaindlg->GetDlgItem(IDC_BTN_VIDEO_START)->EnableWindow(FALSE);

	int nRecvBuf = 1024 * 1024 * 10;//接收缓存10M  
	int xxx = setsockopt((SOCKET)*pmaindlg->pmyvideosocket, SOL_SOCKET, SO_SNDBUF, (const char*)&nRecvBuf, sizeof(int));
	TRACE("/r/n%d/r/n", xxx);

	while (m_bcap)
	{
		static videodate data;
		cv::Mat temp;
		pmaindlg->capture.read(temp);

		if (temp.data)
		{
			TRACE("\r\n摄像头获取数据中！\r\n");
			char* imgsize = (char*)temp.data;                        //指向该帧的数据矩阵  
			for (int i = 0; i < 32; i++)                    //640*480*3= 921600 
			{
				for (int k = 0; k < blocksize; k++)
				{
					data.buf[k] = imgsize[i * blocksize + k];
				}
				if (i == 31)                         //标识一帧中最后一个数据包             
				{
					data.flag = 2;
				}
				else
				{
					data.flag = 1;
				}

				int sendData = pmaindlg->pmyvideosocket->Send((char*)(&data), sizeof(data));


			}
			char c = cv::waitKey(1000 / 300);
		}
		else
		{
			TRACE("\r\n获取摄像头图片数据失败了！\r\n");
		}

	}
	return 0L;
}

void videodlg::OnBnClickedBtnApplive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bcap = TRUE;
	threadHandle = ::CreateThread(NULL, 0, threadwork, (LPVOID)this, 0, 0);
}


BOOL videodlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pmyvideosocket = new videosocket((videodlg*)this);
	if (!pmyvideosocket->Create())
	{
		delete pmyvideosocket;
		MessageBox("套接字创建失败.");
		return false;
	}
	login logindlg;
	CString m_ipserver = logindlg.m_ipserver; //读取服务器名称
	int m_iport_video = logindlg.m_iport_video;

	if (!pmyvideosocket->Connect(m_ipserver, m_iport_video)) //连接服务器
	{
		if (MessageBox("连接服务器失败,是否尝试重新连接?", "提示", MB_YESNO) == IDYES)
		{
			delete pmyvideosocket;
		}
	}
	TRACE("视频窗口套接字 连接服务器成功");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



