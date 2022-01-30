#pragma once
#include <afxsock.h>
#include "clientDlg.h"
class CclientDlg;
class mysocket:public CSocket
{
public:
	mysocket(CclientDlg* pDlg);
protected:
	CclientDlg* pClientdlg;
private:

public:
	virtual void OnReceive(int nErrorCode);
};


