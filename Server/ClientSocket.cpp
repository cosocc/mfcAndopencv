#include "pch.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket(CServerDlg* pdlg)
	: m_pDlg(pdlg)
{

}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	if (m_pDlg)
	{
		m_pDlg->ReceiveData(this);
	
	}
}
