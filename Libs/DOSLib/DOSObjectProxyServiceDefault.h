﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectProxyService.h                                  */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CBaseDOSObjectProxyService
{
public:
	virtual void Destory() = 0;
	virtual void Release() = 0;
	virtual BYTE GetProxyType() = 0;
	virtual void SetID(UINT ID) = 0;
	virtual UINT GetID() = 0;

	virtual bool StartService() = 0;
	virtual void StopService() = 0;
	virtual bool PushMessage(OBJECT_ID ObjectID, CDOSMessagePacket * pPacket) = 0;

	virtual UINT GetConnectionCount(){ return 0; }
	virtual float GetCPUUsedRate(){ return 0; }
	virtual float GetCycleTime(){ return 0; }
	virtual UINT GetGroupCount(){ return 0; }
	virtual float GetGroupCPUUsedRate(UINT Index){ return 0; }
	virtual float GetGroupCycleTime(UINT Index){ return 0; }
};


class CDOSObjectProxyServiceDefault :
	public CBaseDOSObjectProxyService,
	public CNetService, 
	public CEasyThread
{
protected:
	CLIENT_PROXY_CONFIG											m_Config;
	CCycleQueue<CDOSMessagePacket *>							m_MsgQueue;

	CStaticMap<MSG_ID_TYPE, OBJECT_ID>							m_MessageMap;
	OBJECT_ID													m_UnhandleMsgReceiverID;
	CEasyCriticalSection										m_EasyCriticalSection;

	CThreadPerformanceCounter									m_ThreadPerformanceCounter;
	


	CIDStorage<CDOSObjectProxyConnectionDefault>				m_ConnectionPool;
	CThreadSafeIDStorage<CDOSObjectProxyConnectionDefault *>	m_DestoryConnectionList;
	CEasyArray<CDOSObjectProxyConnectionGroup>					m_ConnectionGroups;
	CEasyBuffer													m_CompressBuffer;
	char														m_LZOCompressWorkMemory[LZO1X_1_MEM_COMPRESS];

public:
	CDOSObjectProxyServiceDefault(void);
	virtual ~CDOSObjectProxyServiceDefault(void);
	
	virtual void Release();
	virtual void Destory();
	virtual BYTE GetProxyType();
	virtual void SetID(UINT ID);
	virtual UINT GetID();
	virtual bool StartService();
	virtual void StopService();
	virtual bool PushMessage(OBJECT_ID ObjectID, CDOSMessagePacket * pPacket);
	
	virtual UINT GetConnectionCount();
	virtual float GetCPUUsedRate();
	virtual float GetCycleTime();
	virtual UINT GetGroupCount();
	virtual float GetGroupCPUUsedRate(UINT Index);
	virtual float GetGroupCycleTime(UINT Index);



	bool Init(CDOSServer * pServer, CLIENT_PROXY_CONFIG& Config);
	


	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

	virtual void OnClose();
	virtual int Update(int ProcessPacketLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	virtual CBaseNetConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual bool DeleteConnection(CBaseNetConnection * pConnection);	
	CDOSObjectProxyConnectionDefault * GetConnection(UINT ID);

	void AcceptConnection(CDOSObjectProxyConnectionDefault * pConnection);
	void QueryDestoryConnection(CDOSObjectProxyConnectionDefault * pConnection);


	OBJECT_ID GetGlobalMsgMapObjectID(MSG_ID_TYPE MsgID);
	OBJECT_ID GetUnhandleMsgReceiverID();

	CLIENT_PROXY_CONFIG& GetConfig()
	{
		return m_Config;
	}

	CEasyBuffer& GetCompressBuffer()
	{
		return m_CompressBuffer;
	}

protected:
	void OnMsg(CDOSMessage * pMessage);
	void OnSystemMsg(CDOSMessage * pMessage);
	bool DoRegisterGlobalMsgMap(MSG_ID_TYPE MsgID, OBJECT_ID ObjectID);
	bool DoUnregisterGlobalMsgMap(MSG_ID_TYPE MsgID, OBJECT_ID ObjectID);
	void ClearMsgMapByRouterID(UINT RouterID);
	bool CheckEncryptConfig();
};



inline CDOSObjectProxyConnectionDefault * CDOSObjectProxyServiceDefault::GetConnection(UINT ID)
{
	return m_ConnectionPool.GetObject(ID);
}