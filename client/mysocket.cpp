#include "pch.h"
#include "mysocket.h"

mysocket::mysocket(CclientDlg* pDlg):pClientdlg(pDlg)
{
}




void mysocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnReceive(nErrorCode);
	
	if (pClientdlg)
	{
		pClientdlg->ReceiveData();
	}
}
