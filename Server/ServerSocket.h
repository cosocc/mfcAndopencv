#pragma once
#include <afxsock.h>
#include "ServerDlg.h"
class CServerDlg;
class ServerSocket :
    public CSocket
{


public:
    CServerDlg* m_pDlg;
   /* ServerSocket();*/
    ServerSocket(CServerDlg* dlg);
    virtual ~ServerSocket();
    virtual void OnAccept(int nErrorCode);
    

};

