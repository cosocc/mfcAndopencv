#pragma once
#include "ServerDlg.h"
#include <afxsock.h>
class CServerDlg;

class ClientSocket :
    public CSocket
{
public:
	
	ClientSocket(CServerDlg* pdlg);
	virtual ~ClientSocket();
	CServerDlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);

};

