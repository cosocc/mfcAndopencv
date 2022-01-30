
// ServerDlg.h: 头文件
//

#pragma once

#include <iostream>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "videosocket.h"


class ServerSocket;
class ClientSocket;
class videosocket;
const int BUFFERSIZE = 1024;
// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	
	CServerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	void ReceiveData(ClientSocket* socket);
	void AcceptConnect();
	ServerSocket* m_pSocket;
	ClientSocket* m_pClient;
	videosocket* m_videosocket;
	void ReceviveVideo(videosocket* socket);
	CPtrList m_socketlist;
	CPtrList m_socketVideolist;
	int nCmd = 0;
	
private:	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:

	UINT u_m_port;
	CString c_m_host_name;
	CIPAddressCtrl ip_m_idc;
	afx_msg void OnBnClickedBtnVideo();

protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();

	
	afx_msg void OnBnClickedButton3();
	CEdit m_input;
	afx_msg void OnBnClickedBtnVideoClose();
};
