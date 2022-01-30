#include "pch.h"
#include "videosocket.h"
videosocket::videosocket(CServerDlg* pdlg)
	: m_pDlg(pdlg)
{
}

videosocket::~videosocket()
{
}


void videosocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	if (m_pDlg)
	{
		m_pDlg->ReceviveVideo(this);
	}
}
