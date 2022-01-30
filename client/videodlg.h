#pragma once
#include "clientDlg.h"
#include "videosocket.h"
#include "login.h"
// videodlg 对话框
class CclientDlg;
class videosocket;
class videodlg : public CDialogEx
{
	DECLARE_DYNAMIC(videodlg)

public:
	videodlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~videodlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VIDEOCLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool capinit();
	HANDLE threadHandle;
	static BOOL m_bcap;
	cv::Mat capimg;
	cv::VideoCapture capture;
	static char img[blocksize * 32];
	static videodate data;
	cv::Mat  v_frame;
	cv::Mat m_grabframe;
	videosocket* pmyvideosocket;
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);				//定时器处理函数
	void DrawPicToHDC(cv::Mat p, UINT IDC);						//在控件中显示图片
	afx_msg void OnBnClickedBtnVideoshow();
	afx_msg void OnBnClickedBtnClosevideo();
	afx_msg void OnBnClickedBtnRemote();
	afx_msg void OnBnClickedBtnApplive();

	void ReceiveVieo();
	static DWORD WINAPI threadwork(LPVOID pParam);
	virtual BOOL OnInitDialog();

};
