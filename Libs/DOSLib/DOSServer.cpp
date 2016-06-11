﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSServer.cpp                                            */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

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

void CDOSServer::SetConfig(const DOS_CONFIG& Config)
{
	m_ServerConfig = Config;

	if (m_ServerConfig.RouterID == 0)
	{
		CEasyArray<CIPAddress> AddressList;
		CIPAddress::FetchAllHostAddress(AddressList);

		for (size_t i = 0; i < AddressList.GetCount(); i++)
		{
			if (AddressList[i].IsIPv4() && !AddressList[i].IsLoopbackAddress())
			{
				UINT IP = AddressList[i].GetIPv4();
				m_ServerConfig.RouterID = IP & 0xFFFF;
			}
		}		
	}
}

BOOL CDOSServer::OnStartUp()
{
	FUNCTION_BEGIN;
	if(!m_MemoryPool.Create(m_ServerConfig.MemoryPoolBlockSize,m_ServerConfig.MemoryPoolLeveSize,m_ServerConfig.MemoryPoolLevelCount,true))
	{
		PrintDOSLog(_T("DOSLib"),_T("初始化内存池失败！"));
		return FALSE;
	}
	m_pProxyManager = new CDOSProxyManager();
	if (!m_pProxyManager->Initialize(this))
	{
		PrintDOSLog(_T("DOSLib"), _T("代理管理器初始化失败！"));
		return FALSE;
	}

	m_pDOSRouterService=new CDOSRouter();
	if(!m_pDOSRouterService->Init(this))
	{
		PrintDOSLog(_T("DOSLib"),_T("路由服务启动失败！"));
		return FALSE;
	}
	//m_pDOSRouterService->WaitForWorking(DEFAULT_THREAD_STARTUP_TIME);
	PrintDOSLog(_T("DOSLib"),_T("路由服务启动！"));

	m_pObjectManager=new CDOSObjectManager();

	m_pObjectManager->SetServer(this);
	if(!m_pObjectManager->Initialize())
	{
		PrintDOSLog(_T("DOSLib"), _T("对象管理器初始化失败！"));
		return FALSE;
	}


	PrintDOSLog(_T("DOSLib"),_T("对象管理器启动！"));

	PrintDOSLog(_T("DOSLib"),_T("服务器(%d)启动！"),m_ServerConfig.RouterID);

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
