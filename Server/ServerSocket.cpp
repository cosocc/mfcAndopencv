#include "pch.h"
#include "ServerSocket.h"

//ServerSocket::ServerSocket()
//{
//}

ServerSocket::ServerSocket(CServerDlg* dlg)
{
	m_pDlg = dlg;
}

ServerSocket::~ServerSocket()
{

	m_pDlg = NULL;
}

void ServerSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	if (m_pDlg)
		m_pDlg->AcceptConnect();
}
