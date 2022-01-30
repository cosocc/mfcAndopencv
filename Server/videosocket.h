#pragma once
#include "ServerDlg.h"
#include <afxsock.h>
class videosocket :
    public CSocket
{
public:

	videosocket(CServerDlg* pdlg);
	virtual ~videosocket();
	CServerDlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
};

