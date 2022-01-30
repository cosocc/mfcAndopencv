#include "pch.h"
#include "mysocket.h"

mysocket::mysocket(CclientDlg* pDlg):pClientdlg(pDlg)
{
}




void mysocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnReceive(nErrorCode);
	
	if (pClientdlg)
	{
		pClientdlg->ReceiveData();
	}
}
