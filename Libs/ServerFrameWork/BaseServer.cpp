#include "stdafx.h"

CBaseServer::CBaseServer()
{
	m_LogBuffer.Create(DEFAULT_CONSOLE_LOG_BUFF_LEN);
	m_LogBuffer.SetLockMode(false, true);
	m_ConsoleLogLevel = 0;
	m_CommandPool.Create(DEFAULT_CONSOLE_CMD_COUNT);
	m_ServerStatus.Create(SERVER_STATUS_BLOCK_SIZE);
}

CBaseServer::~CBaseServer()
{

}

bool CBaseServer::PrintConsoleLog(int Level, LPCTSTR szLogMsg)
{
	if (m_ConsoleLogLevel&Level)
	{
#ifdef WIN32
		m_LogBuffer.PushBack(szLogMsg, strlen(szLogMsg));
		m_LogBuffer.PushBack("\r\n", 2);
#else
		printf("%s\n", szLogMsg);
#endif
	}
	return true;
}

LPCTSTR CBaseServer::GetConsoleLog(UINT& MsgLen)
{
	MsgLen = m_LogBuffer.GetSmoothUsedSize();
	if (MsgLen)
	{
		return (LPCTSTR)m_LogBuffer.GetUsedBuffer();
	}
	return NULL;
}

void CBaseServer::PopupConsoleLog(UINT MsgLen)
{
	m_LogBuffer.PopFront((LPVOID)NULL, MsgLen);
}

bool CBaseServer::PushConsoleCmd(LPCTSTR szCommand)
{
	SERVER_COMMAND * pCmd;
	UINT ID = m_CommandPool.NewObject(&pCmd);
	if (ID)
	{
		pCmd->ID = ID;
		strncpy_0(pCmd->Command, MAX_SERVER_CMD_LEN, szCommand, MAX_SERVER_CMD_LEN);
		return true;
	}
	return false;
}


bool CBaseServer::SetServerStatus(WORD StatusID, const CSmartValue& Value)
{
	if (m_ServerStatus.IDToIndex(StatusID) == CSmartStruct::INVALID_MEMBER_ID)
	{
		return m_ServerStatus.AddMember(StatusID, Value);
	}
	else
	{
		m_ServerStatus.GetMember(StatusID).SetValue(Value);
		return true;
	}
}
CSmartValue CBaseServer::GetServerStatus(WORD StatusID)
{
	return m_ServerStatus.GetMember(StatusID);
}
CSmartStruct& CBaseServer::GetAllServerStatus()
{
	return m_ServerStatus;
}

UINT CBaseServer::GetAllServerStatus(BYTE * pBuff, UINT BuffLen)
{
	CAutoLock Lock(m_ServerStatusCriticalSection);
	if (BuffLen >= m_ServerStatus.GetDataLen())
	{
		memcpy(pBuff, m_ServerStatus.GetData(), m_ServerStatus.GetDataLen());
		return m_ServerStatus.GetDataLen();
	}
	return 0;
}
void CBaseServer::SetServerStatusFormat(WORD StatusID, LPCTSTR szStatusName, int FormatType)
{
	CAutoLock Lock(m_ServerStatusCriticalSection);

	for (UINT i = 0; i < m_ServerStatusFormatInfoList.GetCount(); i++)
	{
		if (m_ServerStatusFormatInfoList[i].StatusID == StatusID)
		{
			m_ServerStatusFormatInfoList[i].FormatType = FormatType;
			strncpy(m_ServerStatusFormatInfoList[i].szName, szStatusName, MAX_SERVER_STATUS_NAME_LEN);
			m_ServerStatusFormatInfoList[i].szName[MAX_SERVER_STATUS_NAME_LEN] = 0;
			return;
		}
	}

	SERVER_STATUS_FORMAT_INFO FormatInfo;
	FormatInfo.StatusID = StatusID;
	FormatInfo.FormatType = FormatType;
	strncpy(FormatInfo.szName, szStatusName, MAX_SERVER_STATUS_NAME_LEN);
	FormatInfo.szName[MAX_SERVER_STATUS_NAME_LEN] = 0;
	m_ServerStatusFormatInfoList.Add(FormatInfo);
}
bool CBaseServer::GetServerStatusFormat(WORD StatusID, SERVER_STATUS_FORMAT_INFO& FormatInfo)
{
	CAutoLock Lock(m_ServerStatusCriticalSection);

	for (UINT i = 0; i < m_ServerStatusFormatInfoList.GetCount(); i++)
	{
		if (m_ServerStatusFormatInfoList[i].StatusID == StatusID)
		{
			FormatInfo = m_ServerStatusFormatInfoList[i];
			return true;
		}
	}
	return false;
}
void CBaseServer::GetAllServerStatusFormat(CEasyArray<SERVER_STATUS_FORMAT_INFO>& FormatInfoList)
{
	CAutoLock Lock(m_ServerStatusCriticalSection);

	FormatInfoList = m_ServerStatusFormatInfoList;
}