#include "stdafx.h"



CDOSObjectProxyServiceCustom::CDOSObjectProxyServiceCustom()
{
	m_pServer = NULL;
	m_pProxyService = NULL;
}


CDOSObjectProxyServiceCustom::~CDOSObjectProxyServiceCustom()
{
}

UINT CDOSObjectProxyServiceCustom::AddUseRef()
{
	return CEasyThread::AddUseRef();
}
void CDOSObjectProxyServiceCustom::Release()
{
	CEasyThread::Release();
}
void CDOSObjectProxyServiceCustom::Destory()
{

	LPVOID Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CDOSObjectProxyConnectionCustom * pConnection = m_ConnectionPool.GetNext(Pos);

		pConnection->Destory();

	}
	m_ConnectionPool.Clear();

	SAFE_RELEASE(m_pProxyService);


#ifdef WIN32
	if (FreeLibrary(m_PluginInfo.hModule))
#else
	if (dlclose(m_PluginInfo.hModule) == 0)
#endif
	{
		Log("�������ͷųɹ�%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
	}
	else
	{
		Log("�������ͷ�ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
		Log("������Ϣ:%s", dlerror());
#endif
	}

	m_PluginInfo.hModule = NULL;
}
BYTE CDOSObjectProxyServiceCustom::GetProxyType()
{
	return m_Config.ProxyType;
}
void CDOSObjectProxyServiceCustom::SetID(UINT ID)
{
	CEasyThread::SetID(ID);
}
UINT CDOSObjectProxyServiceCustom::GetID()
{
	return CEasyThread::GetID();
}
bool CDOSObjectProxyServiceCustom::StartService()
{
	return Start() != FALSE;
}
void CDOSObjectProxyServiceCustom::StopService()
{
	SafeTerminate();
}
bool CDOSObjectProxyServiceCustom::PushMessage(CDOSMessagePacket * pPacket)
{
	m_pServer->AddRefMessagePacket(pPacket);
	pPacket->SetAllocTime(4);
	if (m_MsgQueue.PushBack(pPacket))
	{
		return true;
	}
	else
	{
		m_pServer->ReleaseMessagePacket(pPacket);
	}
	return false;
}
bool CDOSObjectProxyServiceCustom::PushBroadcastMessage(CDOSMessagePacket * pPacket)
{
	LPVOID Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CDOSObjectProxyConnectionCustom * pConnection = m_ConnectionPool.GetNext(Pos);
		if (pConnection)
		{
			pConnection->PushMessage(pPacket);
		}
	}
	return true;
}
IDOSObjectProxyConnectionBase * CDOSObjectProxyServiceCustom::GetConnection(UINT ID)
{
	return m_ConnectionPool.GetObject(ID);
}

float CDOSObjectProxyServiceCustom::GetCPUUsedRate()
{
	return m_ThreadPerformanceCounter.GetCPUUsedRate();
}
float CDOSObjectProxyServiceCustom::GetCycleTime()
{
	return m_ThreadPerformanceCounter.GetCycleTime();
}


OBJECT_ID CDOSObjectProxyServiceCustom::GetObjectID()
{
	return m_ObjectID;
}
const CLIENT_PROXY_CONFIG& CDOSObjectProxyServiceCustom::GetConfig()
{
	return m_Config;
}

CNetServer * CDOSObjectProxyServiceCustom::GetNetServer()
{
	return m_pServer;
}
bool CDOSObjectProxyServiceCustom::SendMessage(OBJECT_ID ReceiverID, MSG_ID_TYPE MsgID, WORD MsgFlag, LPCVOID pData, UINT DataSize)
{
	return m_pServer->GetRouter()->RouterMessage(m_ObjectID, ReceiverID, MsgID, MsgFlag, pData, DataSize) != FALSE;
}

IDOSObjectProxyConnection * CDOSObjectProxyServiceCustom::CreateConnection(CIPAddress& RemoteAddress)
{
	CDOSObjectProxyConnectionCustom * pConnection = NULL;
	UINT ID = m_ConnectionPool.NewObject(&pConnection);
	if (pConnection)
	{
		IDOSObjectProxyConnection * pProxyConnection = (*(m_PluginInfo.pConnectionCreateFN))();
		if (pProxyConnection)
		{
			if (!pConnection->Init(this, ID, pProxyConnection))
			{
				m_ConnectionPool.DeleteObject(ID);
			}
			SAFE_RELEASE(pProxyConnection);
		}
		else
		{
			m_ConnectionPool.DeleteObject(ID);
		}
		return pConnection->GetInterface();
	}
	return NULL;
}
bool CDOSObjectProxyServiceCustom::DeleteConnection(UINT ID)
{
	CDOSObjectProxyConnectionCustom * pConnection = m_ConnectionPool.GetObject(ID);
	if (pConnection)
	{
		pConnection->Destory();
		if (m_ConnectionPool.DeleteObject(ID))
		{
			return true;
		}
	}
	return false;
}

BOOL CDOSObjectProxyServiceCustom::Init(CDOSServer * pServer, CLIENT_PROXY_PLUGIN_INFO& PluginInfo)
{
	m_pServer = pServer;
	m_Config = PluginInfo;
	m_PluginInfo = PluginInfo;

	if (m_PluginInfo.ModuleFileName.IsEmpty())
#ifdef _DEBUG
		m_PluginInfo.ModuleFileName = m_PluginInfo.PluginName + "D";
#else
		m_PluginInfo.ModuleFileName = m_PluginInfo.PluginName;
#endif

#ifndef WIN32
	m_PluginInfo.ModuleFileName.Replace('\\', '/');
#endif
	m_PluginInfo.ModuleFileName = MakeModuleFullPath(NULL, m_PluginInfo.ModuleFileName);
	//��չ������
	CEasyString FileExt = GetPathFileExtName(m_PluginInfo.ModuleFileName);
	if (FileExt.GetLength() <= 1)
	{
#ifdef WIN32
		m_PluginInfo.ModuleFileName = m_PluginInfo.ModuleFileName + ".dll";
#else
		m_PluginInfo.ModuleFileName = m_PluginInfo.ModuleFileName + ".so";
#endif
	}

	Log("��ʼװ�ش�����%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifdef WIN32
	m_PluginInfo.hModule = LoadLibrary(m_PluginInfo.ModuleFileName);
#else
	m_PluginInfo.hModule = dlopen(m_PluginInfo.ModuleFileName, RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
#endif
	if (m_PluginInfo.hModule)
	{
#ifdef WIN32
		if (CSystemConfig::GetInstance()->IsPreLoadModuleSym())
		{
			CExceptionParser::GetInstance()->SymLoadFromModule(m_PluginInfo.ModuleFileName);
		}
		m_PluginInfo.pInitFN = (CLIENT_PROXY_INIT_FN)GetProcAddress(m_PluginInfo.hModule, CLIENT_PROXY_INIT_FN_NAME);
		m_PluginInfo.pGetServiceFN = (CLIENT_PROXY_GET_SERVICE_FN)GetProcAddress(m_PluginInfo.hModule, CLIENT_PROXY_GET_SERVICE_FN_NAME);
		m_PluginInfo.pConnectionCreateFN = (CLIENT_PROXY_CONNECTION_CREATE_FN)GetProcAddress(m_PluginInfo.hModule, CLIENT_PROXY_CONNECTION_CREATE_FN_NAME);
		if (m_PluginInfo.pInitFN && m_PluginInfo.pGetServiceFN && m_PluginInfo.pConnectionCreateFN)
#else
		m_PluginInfo.pInitFN = (CLIENT_PROXY_INIT_FN)dlsym(m_PluginInfo.hModule, CLIENT_PROXY_INIT_FN_NAME);
		LPCTSTR InitFNError = dlerror();
		m_PluginInfo.pGetServiceFN = (CLIENT_PROXY_GET_SERVICE_FN)dlsym(m_PluginInfo.hModule, CLIENT_PROXY_GET_SERVICE_FN_NAME);
		LPCTSTR GetServiceFNError = dlerror();
		m_PluginInfo.pConnectionCreateFN = (CLIENT_PROXY_CONNECTION_CREATE_FN)dlsym(m_PluginInfo.hModule, CLIENT_PROXY_CONNECTION_CREATE_FN_NAME);
		LPCTSTR CreateFNError = dlerror();
		if (InitFNError == NULL && GetServiceFNError == NULL && CreateFNError == NULL && m_PluginInfo.pInitFN && m_PluginInfo.pGetServiceFN && m_PluginInfo.pConnectionCreateFN)
#endif
		{
			if (m_PluginInfo.pInitFN(m_PluginInfo.ID, m_PluginInfo.LogChannel))
			{
				m_pProxyService = m_PluginInfo.pGetServiceFN();
				if (m_pProxyService)
				{
					CEasyString LogFileName;
					CEasyString ModulePath = GetModulePath(NULL);

					CServerLogPrinter * pLog;
					LogFileName.Format("%s/Log/Plugin.%s", (LPCTSTR)ModulePath, (LPCTSTR)GetPathFileName(m_PluginInfo.ModuleFileName));
					pLog = new CServerLogPrinter(CDOSMainThread::GetInstance(), CServerLogPrinter::LOM_CONSOLE | CServerLogPrinter::LOM_FILE,
						CSystemConfig::GetInstance()->GetLogLevel(), LogFileName);
					CLogManager::GetInstance()->AddChannel(m_PluginInfo.LogChannel, pLog);
					SAFE_RELEASE(pLog);


					Log("������װ�سɹ�%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
					m_PluginInfo.PluginStatus = PLUGIN_STATUS_RUNNING;
					return Start();
				}
				else
				{
					Log("������񴴽�ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);

					return false;
				}
			}
			else
			{
				Log("��������ʼ��ʧ��%s", (LPCTSTR)m_PluginInfo.ModuleFileName);

				return false;
			}

		}
		else
		{
			Log("���Ϸ��Ĵ�����%s", (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
			Log("CreateFN������Ϣ:%s", CreateFNError);
#endif
		}
	}
	else
	{
		Log("�޷�װ�ش�����(%d)%s", GetLastError(), (LPCTSTR)m_PluginInfo.ModuleFileName);
#ifndef WIN32
		Log("������Ϣ:%s", dlerror());
#endif
	}

	m_PluginInfo.PluginStatus = PLUGIN_STATUS_ERROR;

	return false;
}


BOOL CDOSObjectProxyServiceCustom::OnStart()
{

	m_ObjectID.ObjectIndex = 0;
	m_ObjectID.GroupIndex = MAKE_PROXY_GROUP_INDEX(m_Config.ProxyType, GetID());
	m_ObjectID.ObjectTypeID = DOT_PROXY_OBJECT;
	m_ObjectID.RouterID = GetRouterID();


	if (!m_ConnectionPool.Create(m_Config.MaxConnection))
	{
		PrintDOSLog(_T("DOSLib"), _T("�������[%u]����%u��С�����ӳ�ʧ�ܣ�"),
			GetID(),
			m_Config.MaxConnection);
		return FALSE;
	}

	m_ThreadPerformanceCounter.Init(GetThreadHandle(), THREAD_CPU_COUNT_TIME);

	if (!m_MsgQueue.Create(m_Config.ConnectionMsgQueueSize))
	{
		PrintDOSLog(_T("DOSLib"), _T("�������[%u]����%u��С����Ϣ����ʧ�ܣ�"),
			GetID(),
			m_Config.ConnectionMsgQueueSize);
		return FALSE;
	}

	if (!m_pProxyService->Initialize(this))
		return FALSE;

	PrintDOSLog(_T("DOSLib"), _T("�������[%u]�߳�[%u]������"), GetID(), GetThreadID());
	
	return TRUE;
}

BOOL CDOSObjectProxyServiceCustom::OnRun()
{
	m_ThreadPerformanceCounter.DoPerformanceCount();
	int ProcessCount = Update();
	if (ProcessCount == 0)
	{
		DoSleep(1);
	}
	return TRUE;
}

void CDOSObjectProxyServiceCustom::OnTerminate()
{
	m_pProxyService->Destory();
	LPVOID Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CDOSObjectProxyConnectionCustom * pConnection = m_ConnectionPool.GetNext(Pos);

		pConnection->Destory();

	}
	m_ConnectionPool.Clear();
	PrintDOSLog(_T("DOSLib"), _T("�������[%u]�ر�"), GetID());
}

int CDOSObjectProxyServiceCustom::Update(int ProcessPacketLimit)
{
	int ProcessCount = 0;
	LPVOID Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CDOSObjectProxyConnectionCustom * pConnection = m_ConnectionPool.GetNext(Pos);
		if (pConnection->IsConnected())
			ProcessCount += pConnection->Update();
		else
			DeleteConnection(pConnection->GetID());
	}

	if (m_pProxyService)
		ProcessCount += m_pProxyService->Update(ProcessPacketLimit);

	CDOSMessagePacket * pPacket;
	while (m_MsgQueue.PopFront(pPacket))
	{		
		if (pPacket->GetMessage().GetMsgFlag()&DOS_MESSAGE_FLAG_SYSTEM_MESSAGE)
			OnSystemMsg(&(pPacket->GetMessage()));
		else
			OnMsg(&(pPacket->GetMessage()));

		if (!m_pServer->ReleaseMessagePacket(pPacket))
		{
			PrintDOSLog(_T("DOSLib"), _T("�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}

		ProcessCount++;
		if (ProcessCount >= ProcessPacketLimit)
			break;
	}

	return ProcessCount;
}

void CDOSObjectProxyServiceCustom::OnMsg(CDOSMessage * pMessage)
{
	m_pProxyService->OnMessage(pMessage);	
}
void CDOSObjectProxyServiceCustom::OnSystemMsg(CDOSMessage * pMessage)
{
	if (!m_pProxyService->OnSystemMessage(pMessage))
	{
		switch (pMessage->GetMsgID())
		{
		case DSM_PROXY_REGISTER_GLOBAL_MSG_MAP:
			if (pMessage->GetDataLength() >= sizeof(MSG_ID_TYPE))
			{
				int Count = (pMessage->GetDataLength()) / sizeof(MSG_ID_TYPE);
				MSG_ID_TYPE * pMsgIDs = (MSG_ID_TYPE *)(pMessage->GetDataBuffer());
				for (int i = 0; i < Count; i++)
				{
					m_pProxyService->OnRegisterMsgMap(pMsgIDs[i], pMessage->GetSenderID());
				}
			}
			break;
		case DSM_PROXY_UNREGISTER_GLOBAL_MSG_MAP:
			if (pMessage->GetDataLength() >= sizeof(MSG_ID_TYPE))
			{
				int Count = (pMessage->GetDataLength()) / sizeof(MSG_ID_TYPE);
				MSG_ID_TYPE * pMsgIDs = (MSG_ID_TYPE *)(pMessage->GetDataBuffer());
				for (int i = 0; i < Count; i++)
				{
					m_pProxyService->OnUnregisterMsgMap(pMsgIDs[i], pMessage->GetSenderID());
				}
			}
			break;
		case DSM_PROXY_SET_UNHANDLE_MSG_RECEIVER:
			m_pProxyService->SetUnhandleMsgReceiver(pMessage->GetSenderID());
			break;
		case DSM_ROUTE_LINK_LOST:
			m_pProxyService->OnClearMsgMapByRouterID(pMessage->GetSenderID().RouterID);
			break;
		default:
			PrintDOSLog(_T("DOSLib"), _T("ProxyService�յ�δ֪ϵͳ��Ϣ0x%llX"), pMessage->GetMsgID());
		}
	}
	
}
