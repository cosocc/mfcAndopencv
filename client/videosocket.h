#pragma once
#include "clientDlg.h"
#include "videodlg.h"
#include <afxsock.h>

class videodlg;


class videosocket :
    public CSocket
{
public:
	videosocket(videodlg* pDlg);
protected:
	videodlg* pvideodlg;

private:

public:
	virtual void OnReceive(int nErrorCode);
};

