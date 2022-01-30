#include "pch.h"
#include "videosocket.h"
videosocket::videosocket(videodlg* pDlg) :pvideodlg(pDlg)

{
}




void videosocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnReceive(nErrorCode);

	if (pvideodlg)
	{
		pvideodlg->ReceiveVieo();
	}
}