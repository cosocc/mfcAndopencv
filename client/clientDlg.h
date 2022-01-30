
// clientDlg.h: 头文件
//

#pragma once
#include <iostream>
#include "mysocket.h"
#include "videodlg.h"
#include "videosocket.h"

class mysocket;
class videodlg;

// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	void ReceiveData();						//处理收到的文字
	
	std::shared_ptr<videodlg>  pvideoDlg;

	CclientDlg(CWnd* pParent = nullptr);	// 标准构造函数
	mysocket* pmysocket;

	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_input;
	CString m_id;
	CEdit m_info;
	afx_msg void OnBnClickedOk();
protected:
	afx_msg LRESULT OnSubclose(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnVideowindow();
};
