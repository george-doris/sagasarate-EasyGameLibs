#include "stdafx.h"



CDOSObjectProxyConnectionGroupNoBuff::CDOSObjectProxyConnectionGroupNoBuff()
{
	m_pService = NULL;
	m_Index = 0;
	m_ConnectionPool.SetTag(_T("CDOSObjectProxyConnectionGroupNoBuff"));
	m_CompressBuffer.SetTag(_T("CDOSObjectProxyConnectionGroupNoBuff"));
	m_EncyptBuffer.SetTag(_T("CDOSObjectProxyConnectionGroupNoBuff"));
}


CDOSObjectProxyConnectionGroupNoBuff::~CDOSObjectProxyConnectionGroupNoBuff()
{
}

bool CDOSObjectProxyConnectionGroupNoBuff::Init(CDOSObjectProxyServiceNoBuff * pService, CLIENT_PROXY_CONFIG& Config, UINT Index)
{
	m_pService = pService;
	m_Config = Config;
	m_Index = Index;
	if (m_Config.MsgCompressType == MSG_COMPRESS_LZO)
	{
		UINT CompressBufferSize = m_Config.SendBufferSize*NET_DATA_BLOCK_SIZE;
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
		UINT EncyptBufferSize = m_Config.RecvBufferSize*NET_DATA_BLOCK_SIZE;
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
	
	if (!m_ConnectionPool.Create(m_Config.ConnectionPoolSetting))
	{
		PrintDOSLog(_T("����(%u,%u,%u)��С�����ӳ�ʧ�ܣ�"),
			GetID(),
			m_Config.ConnectionPoolSetting.StartSize, m_Config.ConnectionPoolSetting.GrowSize, m_Config.ConnectionPoolSetting.GrowLimit);
		return false;
	}
	return true;
}

bool CDOSObjectProxyConnectionGroupNoBuff::AddConnection(CDOSObjectProxyConnectionNoBuff * pConnection)
{
	CAutoLock Lock(m_EasyCriticalSection);
		
	if (m_ConnectionPool.PushBack(pConnection))
	{
		pConnection->SetCompressBuffer(&m_CompressBuffer, m_LZOCompressWorkMemory);
		pConnection->SetEncyptBuffer(&m_EncyptBuffer);
		pConnection->SetGroup(this);		
		return true;
	}
	else
	{
		PrintDOSLog(_T("���ӳ�����(%u/%u)"), m_ConnectionPool.GetObjectCount(), m_ConnectionPool.GetBufferSize());
	}
	return false;
}

BOOL CDOSObjectProxyConnectionGroupNoBuff::OnStart()
{
	m_ThreadPerformanceCounter.Init(GetThreadHandle(), THREAD_CPU_COUNT_TIME);
	return TRUE;
}

BOOL CDOSObjectProxyConnectionGroupNoBuff::OnRun()
{

	m_ThreadPerformanceCounter.DoPerformanceCount();

	int ProcessCount = 0;

	LPVOID Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		LPVOID OldPos = Pos;
		CDOSObjectProxyConnectionNoBuff * pConnection = NULL;
		{
			CAutoLock Lock(m_EasyCriticalSection);

			pConnection = *m_ConnectionPool.GetNextObject(Pos);
		}
		ProcessCount += pConnection->Update();
		if (!pConnection->IsConnected())
		{
			CAutoLock Lock(m_EasyCriticalSection);
			m_ConnectionPool.DeleteObjectByPos(OldPos);
			m_pService->QueryDestoryConnection(pConnection);
		}
	}

	if (ProcessCount == 0)
	{
		DoSleep(DEFAULT_IDLE_SLEEP_TIME);
	}
	return TRUE;
}