#include "stdafx.h"



CWebSocketProxyService::CWebSocketProxyService()
{
	m_WSConnectionPool.SetTag(_T("CWebSocketProxyService"));
}
CWebSocketProxyService::~CWebSocketProxyService()
{
	Destory();
}


BOOL CWebSocketProxyService::OnStart()
{
	m_ThreadPerformanceCounter.Init(GetThreadHandle(), THREAD_CPU_COUNT_TIME);

	if (!m_MsgQueue.Create(m_Config.ProxyMsgQueueSize))
	{
		PrintDOSLog(_T("�������[%u]����%u��С����Ϣ����ʧ�ܣ�"),
			GetID(),
			m_Config.ProxyMsgQueueSize);
		return FALSE;
	}

	if (!m_MessageMap.Create(m_Config.GlobalMsgMapSize, m_Config.GlobalMsgMapSize, 32))
	{
		PrintDOSLog(_T("�������[%u]����%u��С����Ϣӳ���ʧ�ܣ�"),
			GetID(),
			m_Config.GlobalMsgMapSize);
		return FALSE;
	}

	if (m_Config.MinMsgCompressSize)
	{
		switch (m_Config.MsgCompressType)
		{
		case MSG_COMPRESS_LZO:
		{
			if (lzo_init() != LZO_E_OK)
			{
				PrintDOSLog(_T("�����������Ϣѹ��ʧ��"));
				m_Config.MsgCompressType = MSG_COMPRESS_NONE;
				m_Config.MinMsgCompressSize = 0;
			}
			PrintDOSLog(_T("�����������Ϣѹ��"));
		}
		break;
		default:
			PrintDOSLog(_T("�������[%u]��֧����Ϣѹ��ģʽ%u��"),
				GetID(), m_Config.MsgCompressType);
			m_Config.MsgCompressType = MSG_COMPRESS_NONE;
			m_Config.MinMsgCompressSize = 0;
			break;
		}
	}
	else
	{
		m_Config.MsgCompressType = MSG_COMPRESS_NONE;
	}

	if (!CheckEncryptConfig())
	{
		m_Config.MsgEnCryptType = MSG_ENCRYPT_NONE;
		PrintDOSLog(_T("�������ò��Ϸ�����Ϣ���ܱ�ȡ��"));
	}

	if (m_Config.EnableBlackList)
	{
		CAutoLock Lock(m_BlackListCriticalSection);

		m_IPBlackList.Create(m_Config.BlackListPoolSetting);
		for (const CIPAddress& IP : m_Config.InitBlackList)
		{
			IP_INFO* pInfo = NULL;
			m_IPBlackList.New(IP, &pInfo);
			if (pInfo)
			{
				pInfo->IP = IP;
				pInfo->ExpireTime = 0;
			}
		}
		PrintDOSLog(_T("�Ѽ���%u��������"), m_IPBlackList.GetObjectCount());

		m_RecvProtectedIPList.Create(m_Config.BlackListPoolSetting);
	}

	m_BlackListUpdateTimer.SaveTime();

	if (!Create(IPPROTO_TCP,
		m_Config.AcceptQueueSize,
		m_Config.RecvBufferSize,
		m_Config.SendBufferSize,
		m_Config.ParallelAcceptCount,
		DEFAULT_PARALLEL_RECV,
		false))
	{
		PrintDOSLog(_T("�������[%u]����ʧ�ܣ�"), GetID());
		return FALSE;
	}

	if (!StartListen(m_Config.ListenAddress))
	{
		PrintDOSLog(_T("�������[%u]������(%s:%u)ʧ�ܣ�"),
			GetID(),
			m_Config.ListenAddress.GetIPString(),
			m_Config.ListenAddress.GetPort());
		return FALSE;
	}
	PrintDOSLog(_T("�������[%u]������(%s:%u)��"),
		GetID(),
		m_Config.ListenAddress.GetIPString(),
		m_Config.ListenAddress.GetPort());

	PrintDOSLog(_T("�������[%u]�߳�[%u]������"), GetID(), GetThreadID());

	if (m_Config.MsgCompressType == MSG_COMPRESS_LZO)
	{
		UINT CompressBufferSize = m_Config.SendBufferSize * NET_DATA_BLOCK_SIZE;
		if (m_CompressBuffer.GetBufferSize() < CompressBufferSize)
		{
			if (m_CompressBuffer.Create(CompressBufferSize))
			{
				PrintDOSLog(_T("�Ѵ���%u��С��ѹ������"),
					CompressBufferSize);
			}
			else
			{
				PrintDOSLog(_T("����%u��С��ѹ������ʧ�ܣ�"),
					CompressBufferSize);
				return FALSE;
			}
		}
	}

	if (m_Config.MsgEnCryptType != MSG_ENCRYPT_NONE)
	{
		UINT EncyptBufferSize = m_Config.RecvBufferSize * NET_DATA_BLOCK_SIZE;
		if (m_EncyptBuffer.GetBufferSize() < EncyptBufferSize)
		{
			if (m_EncyptBuffer.Create(EncyptBufferSize))
			{
				PrintDOSLog(_T("�Ѵ���%u��С�Ľ��ܻ���"),
					EncyptBufferSize);
			}
			else
			{
				PrintDOSLog(_T("����%u��С�Ľ��ܻ���ʧ�ܣ�"),
					EncyptBufferSize);
				return FALSE;
			}
		}
	}

	if (m_WSConnectionPool.Create(m_Config.ConnectionPoolSetting))
	{
		PrintDOSDebugLog(_T("�������[%u]����(%u,%u,%u)��С�����ӳسɹ���"),
			GetID(),
			m_Config.ConnectionPoolSetting.StartSize, m_Config.ConnectionPoolSetting.GrowSize, m_Config.ConnectionPoolSetting.GrowLimit);
	}
	else
	{
		PrintDOSLog(_T("�������[%u]����(%u,%u,%u)��С�����ӳ�ʧ�ܣ�"),
			GetID(),
			m_Config.ConnectionPoolSetting.StartSize, m_Config.ConnectionPoolSetting.GrowSize, m_Config.ConnectionPoolSetting.GrowLimit);
		return FALSE;
	}



	if (m_Config.ConnectionGroupCount)
	{
		if (m_DestoryConnectionList.Create(m_Config.ConnectionPoolSetting))
		{
			PrintDOSDebugLog(_T("�������[%u]����(%u,%u,%u)��С���������ٶ��гɹ���"),
				GetID(),
				m_Config.ConnectionPoolSetting.StartSize, m_Config.ConnectionPoolSetting.GrowSize, m_Config.ConnectionPoolSetting.GrowLimit);
		}
		else
		{
			PrintDOSLog(_T("�������[%u]����(%u,%u,%u)��С���������ٶ���ʧ�ܣ�"),
				GetID(),
				m_Config.ConnectionPoolSetting.StartSize, m_Config.ConnectionPoolSetting.GrowSize, m_Config.ConnectionPoolSetting.GrowLimit);
			return FALSE;
		}


		m_ConnectionGroups.Resize(m_Config.ConnectionGroupCount);
		PrintDOSDebugLog(_T("�������[%u]������%u���������߳�"), GetID(), (UINT)m_ConnectionGroups.GetCount());
		for (UINT i = 0; i < m_ConnectionGroups.GetCount(); i++)
		{
			if (!m_ConnectionGroups[i].Init(this, m_Config, i))
				return FALSE;
			m_ConnectionGroups[i].Start();
			PrintDOSDebugLog(_T("�������߳�[%u]������"), m_ConnectionGroups[i].GetThreadID());
		}
	}

	if (m_Config.EnableGuardThread)
	{
		m_GuardThread.SetTargetThread(this);
		m_GuardThread.SetKeepAliveTime(m_Config.GuardThreadKeepAliveTime, m_Config.GuardThreadKeepAliveCount);
		m_GuardThread.Start();
	}



	return TRUE;
}

UINT CWebSocketProxyService::GetConnectionCount()
{
	return m_WSConnectionPool.GetObjectCount();
}
void CWebSocketProxyService::OnClose()
{
	CAutoLockEx Lock;
	if (m_Config.ConnectionGroupCount)
	{
		//����������ģʽ��Ҫ����
		Lock.Lock(m_EasyCriticalSection);
	}

	DISPATCHED_MSG Msg;
	while (m_MsgQueue.PopFront(&Msg))
	{
		if (!((CDOSServer*)GetServer())->ReleaseMessagePacket(Msg.pMsgPacket))
		{
			PrintDOSLog(_T("�ͷ���Ϣ�ڴ��ʧ�ܣ�"));
		}
	}
	for (UINT i = 0; i < m_ConnectionGroups.GetCount(); i++)
	{
		m_ConnectionGroups[i].SafeTerminate();
	}
	LPVOID Pos = m_WSConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CWebSocketProxyConnection* pConnection = m_WSConnectionPool.GetNextObject(Pos);

		pConnection->Destory();

	}
	m_WSConnectionPool.Clear();
	m_MessageMap.Clear();
}
CBaseNetConnection* CWebSocketProxyService::CreateConnection(CIPAddress& RemoteAddress)
{
	CWebSocketProxyConnection* pConnection = NULL;

	UINT ID = m_WSConnectionPool.NewObject(&pConnection);
	if (pConnection)
	{
		if (!pConnection->Init(this, m_Config, ID))
		{
			m_WSConnectionPool.DeleteObject(ID);
			pConnection = NULL;
			PrintDOSDebugLog(_T("����������%u"), m_WSConnectionPool.GetObjectCount());
		}
	}
	return pConnection;
}
bool CWebSocketProxyService::DeleteConnection(CBaseNetConnection* pConnection)
{
	pConnection->Destory();

	if (m_WSConnectionPool.DeleteObject(pConnection->GetID()))
	{
		PrintDOSDebugLog(_T("����������%u"), m_WSConnectionPool.GetObjectCount());
		return true;
	}
	return false;
}
CDOSObjectProxyConnectionDefault* CWebSocketProxyService::GetConnection(UINT ID)
{
	return m_WSConnectionPool.GetObject(ID);
}
LPVOID CWebSocketProxyService::GetFirstConnectionPos()
{
	return m_WSConnectionPool.GetFirstObjectPos();
}
CDOSObjectProxyConnectionDefault* CWebSocketProxyService::GetNextConnection(LPVOID& Pos)
{
	return m_WSConnectionPool.GetNextObject(Pos);
}
int CWebSocketProxyService::CheckFreeObject()
{
	int ProcessCount = 0;
	UINT CurTime = (UINT)time(NULL);
	for (UINT i = 0; i < CONNECTION_FREE_CHECK_BATCH; i++)
	{
		LPVOID Pos = m_WSConnectionPool.GetFreeObjectPosByID(m_FreeObjectCheckPtr);
		if (Pos)
		{
			CWebSocketProxyConnection* pConnection = m_WSConnectionPool.GetFreeObject(Pos);
			if (pConnection)
			{
				if (pConnection->GetReleaseTime() + m_Config.MaxIdleTimeToFree <= CurTime)
				{
					m_WSConnectionPool.ReleaseFreeObject(Pos);
					ProcessCount++;
				}
			}
		}
		m_FreeObjectCheckPtr++;
		if (m_FreeObjectCheckPtr > m_WSConnectionPool.GetBufferSize())
		{
			m_FreeObjectCheckPtr = 1;
		}
	}
	if (ProcessCount)
	{
		PrintDOSDebugLog(_T("�ѳ����ͷ�%d������"), ProcessCount);
	}
	return ProcessCount;
}