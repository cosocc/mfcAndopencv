#include "pch.h"
#include "videosocket.h"
videosocket::videosocket(videodlg* pDlg) :pvideodlg(pDlg)

{
}




void videosocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnReceive(nErrorCode);

	if (pvideodlg)
	{
		pvideodlg->ReceiveVieo();
	}
}