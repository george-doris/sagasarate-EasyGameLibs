/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSServer.cpp                                            */
/*      ��������:  2009��10��23��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      ��������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ������������κ���ҵ�ͷ���ҵ������������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CDOSServer,CNetServer);

CDOSServer::CDOSServer(void):CNetServer()
{
	FUNCTION_BEGIN;
	m_pProxyManager = NULL;
	m_pDOSRouterService=NULL;
	m_pObjectManager=NULL;	
	FUNCTION_END;
}

CDOSServer::~CDOSServer(void)
{
	FUNCTION_BEGIN;
	ShutDown();
	FUNCTION_END;
}



BOOL CDOSServer::OnStartUp()
{
	FUNCTION_BEGIN;
	if(!m_MemoryPool.Create(m_ServerConfig.MemoryPoolBlockSize,m_ServerConfig.MemoryPoolLeveSize,m_ServerConfig.MemoryPoolLevelCount,true))
	{
		PrintDOSLog(0xffff,_T("��ʼ���ڴ��ʧ�ܣ�"));
		return FALSE;
	}
	m_pProxyManager = new CDOSProxyManager();
	if (!m_pProxyManager->Initialize(this))
	{
		PrintDOSLog(0xffff, _T("������������ʼ��ʧ�ܣ�"));
		return FALSE;
	}	

	m_pDOSRouterService=new CDOSRouter();	
	if(!m_pDOSRouterService->Init(this))
	{
		PrintDOSLog(0xffff,_T("·�ɷ�������ʧ�ܣ�"));
		return FALSE;
	}
	//m_pDOSRouterService->WaitForWorking(DEFAULT_THREAD_STARTUP_TIME);
	PrintDOSLog(0xffff,_T("·�ɷ���������"));

	m_pObjectManager=new CDOSObjectManager();

	m_pObjectManager->SetServer(this);
	if(!m_pObjectManager->Initialize())
	{
		PrintDOSLog(0xffff, _T("�����������ʼ��ʧ�ܣ�"));
		return FALSE;
	}


	PrintDOSLog(0xffff,_T("���������������"));

	PrintDOSLog(0xffff,_T("������(%d)������"),m_ServerConfig.RouterID);

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSServer::OnShutDown()
{
	FUNCTION_BEGIN;
	m_MemoryPool.Verfy(0);	

	if(m_pDOSRouterService)
	{
		m_pDOSRouterService->SafeTerminate();
	}

	SAFE_DELETE(m_pProxyManager);
	SAFE_RELEASE(m_pObjectManager);	
	SAFE_DELETE(m_pDOSRouterService);

	m_MemoryPool.Destory();

	FUNCTION_END;
}