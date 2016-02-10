/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppLinux.cpp                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

CServerApp	* m_gAppInstance=NULL;

int main(int argc, char *argv[])
{
	if (argc>1)
	{
		if (strcmp(argv[1], "-d") == 0)
		{
			daemon(0, 1);
		}
	}
	m_gAppInstance->Run();
	return 0;
}


CServerApp::CServerApp(void)
{
	m_WantExist=FALSE;
	m_pServer=NULL;
	m_gAppInstance=this;
}

CServerApp::~CServerApp(void)
{
}



BOOL CServerApp::OnStartUp()
{
	return TRUE;
}
void CServerApp::OnShutDown()
{
}

int CServerApp::Run()
{
	if(OnStartUp())
	{
		while (m_pServer&&(!m_pServer->IsServerTerminated()))
		{
			DoSleep(1);			
		}
		OnShutDown();
	}
	return 0;
}
