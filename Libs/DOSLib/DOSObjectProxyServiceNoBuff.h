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


class CDOSObjectProxyServiceNoBuff :
	public CBaseDOSObjectProxyService,
	public CNetService, 
	public CEasyThread
{
protected:
	struct MSG_MAP_INFO
	{
		int						MsgMapType;
		CEasyArray<OBJECT_ID>	ObjectIDList;
		MSG_MAP_INFO()
			:ObjectIDList(_T("MSG_MAP_INFO"))
		{
			ObjectIDList.SetTag(_T("CDOSObjectProxyServiceNoBuff"));
			ObjectIDList.Create(16, 16);
			MsgMapType = GLOBAL_MSG_MAP_TYPE_OVERLAP;
		}
	};

	struct IP_INFO
	{
		UINT		ExpireTime;
		CIPAddress	IP;
	};

	CLIENT_PROXY_CONFIG													m_Config;
	CCycleQueue<CDOSMessagePacket *>									m_MsgQueue;

	CStaticMap<MSG_ID_TYPE, MSG_MAP_INFO>								m_MessageMap;
	OBJECT_ID															m_UnhandleMsgReceiverID;
	CEasyCriticalSection												m_EasyCriticalSection;

	CThreadPerformanceCounter											m_ThreadPerformanceCounter;
	CGuardThread														m_GuardThread;


	CIDStorage<CDOSObjectProxyConnectionNoBuff, EDSM_NEW_ONCE>			m_ConnectionPool;
	UINT																m_FreeObjectCheckPtr;
	CThreadSafeIDStorage<CDOSObjectProxyConnectionNoBuff *>				m_DestoryConnectionList;
	CEasyArray<CDOSObjectProxyConnectionGroupNoBuff>					m_ConnectionGroups;
	CEasyBuffer															m_CompressBuffer;
	CEasyBuffer															m_EncyptBuffer;
	char																m_LZOCompressWorkMemory[LZO1X_1_MEM_COMPRESS];

	CHashMap<CIPAddress, IP_INFO>										m_IPBlackList;
	CHashMap<CIPAddress, IP_INFO>										m_RecvProtectedIPList;
	CEasyArray<IP_INFO>													m_PrepareIPBlackList;
	CEasyCriticalSection												m_BlackListCriticalSection;
	CEasyTimer															m_BlackListUpdateTimer;

	CStaticMap<UINT64, CEasyArray<CDOSObjectProxyConnectionDefault *> >	m_GroupBroadcastMap;

public:
	CDOSObjectProxyServiceNoBuff(void);
	virtual ~CDOSObjectProxyServiceNoBuff(void);
	
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
	CDOSObjectProxyConnectionNoBuff * GetConnection(UINT ID);

	void AcceptConnection(CDOSObjectProxyConnectionNoBuff * pConnection);
	void QueryDestoryConnection(CDOSObjectProxyConnectionNoBuff * pConnection);


	bool HaveGlobalMsgMap(MSG_ID_TYPE MsgID);
	bool SendGlobalMapMessage(CDOSMessagePacket * pPacket);

	CLIENT_PROXY_CONFIG& GetConfig()
	{
		return m_Config;
	}

	CEasyBuffer& GetCompressBuffer()
	{
		return m_CompressBuffer;
	}
	CDOSServer * GetServer();

	bool AddBlackList(CIPAddress IP, UINT Duration);
	bool OnRecvProtected(CIPAddress IP);
protected:
	void OnMsg(CDOSMessage * pMessage);
	void OnSystemMsg(CDOSMessagePacket * pPacket);
	bool DoRegisterGlobalMsgMap(MSG_ID_TYPE MsgID, int MapType, OBJECT_ID ObjectID);
	bool DoUnregisterGlobalMsgMap(MSG_ID_TYPE MsgID, OBJECT_ID ObjectID);
	void ClearMsgMapByRouterID(UINT RouterID);
	bool CheckEncryptConfig();
	int CheckFreeObject();
	bool AddConnectionToBroadcastGroup(CDOSObjectProxyConnectionDefault * pConnection, UINT64 GroupID);
	bool RemoveConnectionFromBroadcastGroup(CDOSObjectProxyConnectionDefault * pConnection);
};



inline CDOSObjectProxyConnectionNoBuff * CDOSObjectProxyServiceNoBuff::GetConnection(UINT ID)
{
	return m_ConnectionPool.GetObject(ID);
}