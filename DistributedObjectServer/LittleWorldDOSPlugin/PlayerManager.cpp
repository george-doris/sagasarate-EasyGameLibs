#include "StdAfx.h"



//static char g_SendBuff[MAX_MESSAGE_PACKET_SIZE];

START_DOS_MSG_MAP(CPlayerManager)
DOS_MSG_MAP(CPlayerManagerMsgHandler)
DOS_MSG_MAP(CMapAreaAckMsgHandler)
DOS_MSG_MAP(CPlayerCharDBAckMsgHandler)
END_DOS_MSG_MAP(CPlayerManager)


CPlayerManager::CPlayerManager(void)
{
	FUNCTION_BEGIN;
	INIT_DOS_MSG_MAP;
	m_pOperator=NULL;
	m_CurMapID.ID=0;
	m_PlayerCharDBObjectID.ID=0;
	FUNCTION_END;
}

CPlayerManager::~CPlayerManager(void)
{
	FUNCTION_BEGIN;
	//if(m_pOperator)
	//	m_pOperator->Release();
	FUNCTION_END;
}

BOOL CPlayerManager::Initialize(IDistributedObjectOperator * pOperator)
{
	FUNCTION_BEGIN;
	m_pOperator=pOperator;

	if(!m_PlayerPool.Create(CMainConfig::GetInstance()->GetPlayerManagerConfig().PlayerPool.StartSize,
		CMainConfig::GetInstance()->GetPlayerManagerConfig().PlayerPool.GrowSize,
		CMainConfig::GetInstance()->GetPlayerManagerConfig().PlayerPool.GrowLimit))
	{
		return FALSE;
	}

	
	MSG_ID_TYPE MsgIDs[]={
		MAKE_MSG_ID(INTERFACE_LW_PLAYER_MANAGER,METHOD_PM_QUERY_CHAR_INFO,false),
		MAKE_MSG_ID(INTERFACE_LW_PLAYER_MANAGER,METHOD_PM_PLAYER_REGISTER,false)};
	m_pOperator->RegisterGlobalMsgMap(BROAD_CAST_ROUTER_ID, BROAD_CAST_PROXY_TYPE, MsgIDs, sizeof(MsgIDs) / sizeof(MSG_ID_TYPE));

	m_FindTimer.SaveTime();

	Log("��ҽ�ɫ�������[%llX]��ע��",
		m_pOperator->GetObjectID().ID);

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CPlayerManager::Destory()
{
	FUNCTION_BEGIN;	

	MSG_ID_TYPE MsgIDs[]={METHOD_PM_QUERY_CHAR_INFO,METHOD_PM_PLAYER_REGISTER};
	m_pOperator->UnregisterGlobalMsgMap(BROAD_CAST_ROUTER_ID, BROAD_CAST_PROXY_TYPE, MsgIDs, sizeof(MsgIDs) / sizeof(MSG_ID_TYPE));

	void * Pos=m_PlayerPool.GetFirstObjectPos();
	while(Pos)
	{
		CCharacter * pChar=m_PlayerPool.GetNext(Pos);
		pChar->ForceRelease();
	}

	//m_CharPool.Destory();

	m_pOperator=NULL;
	FUNCTION_END;
}

UINT CPlayerManager::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CPlayerManager::Release()
{
	CNameObject::Release();
}

BOOL CPlayerManager::CreatePlayer(OBJECT_ID ClientProxyID,LPCTSTR szAccountName,LPCTSTR szPassword)
{
	CPlayer * pPlayer=NULL;
	UINT ID=m_PlayerPool.NewObject(&pPlayer);
	if(pPlayer)
	{		
		
		if(pPlayer->Init(this,m_pOperator->GetObjectID(),ID,m_CurMapID,m_CharQueryCenterID,m_PlayerCharDBObjectID,ClientProxyID,szAccountName,szPassword))
		{
			DOS_OBJECT_REGISTER_INFO_EX RegisterInfo;
			RegisterInfo.ObjectID.ObjectTypeID=LWOT_PLAYER;
			RegisterInfo.pObject=pPlayer;
			if(m_pOperator->RegisterObject(RegisterInfo))
			{
				//Log("�����˽�ɫ����[%llX][%llu]%s",CharObjectID,CharID,CharName);
				return TRUE;
			}
		}
		DeletePlayer(ID);
	}
	Log("������Ҷ���ʧ��%s",szAccountName);
	return FALSE;
}
BOOL CPlayerManager::DeletePlayer(UINT PoolID)
{
	//Log("ɾ����ɫ����[%llu]",PoolID);
	return m_PlayerPool.DeleteObject(PoolID);
}

void CPlayerManager::OnConcernedObjectLost(OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

void CPlayerManager::OnObjectReport(OBJECT_ID ObjectID,const CSmartStruct& ObjectInfo)
{
	FUNCTION_BEGIN;
	if(ObjectID.ObjectTypeID==LWOT_MAP_AREA)
	{
		m_CurMapID=ObjectID;
		Log("PlayerManager�ҵ���ͼ����[%llX]��",m_CurMapID.ID);
	}
	if(ObjectID.ObjectTypeID==LWOT_CHAR_QUERY_CENTER)
	{
		m_CharQueryCenterID=ObjectID;
		Log("PlayerManager�ҵ�CharQueryCenter[%llX]��",m_CharQueryCenterID.ID);
	}
	if(ObjectID.ObjectTypeID==LWOT_PLATER_CHAR_DATA_DB_ADAPTER)
	{
		m_PlayerCharDBObjectID=ObjectID;
		Log("PlayerManager�ҵ�PlayerCharDB[%llX]��",m_PlayerCharDBObjectID.ID);
	}
	FUNCTION_END;
}

int CPlayerManager::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	
	if(m_FindTimer.IsTimeOut(OBJECT_FIND_TIME))
	{
		m_FindTimer.SaveTime();

		if(m_CurMapID.ID==0)
		{
			m_pOperator->FindObject(LWOT_MAP_AREA);			
		}

		if(m_CharQueryCenterID.ID==0)
		{
			m_pOperator->FindObject(LWOT_CHAR_QUERY_CENTER);			
		}

		if(m_PlayerCharDBObjectID.ID==0)
		{
			m_pOperator->FindObject(LWOT_PLATER_CHAR_DATA_DB_ADAPTER);			
		}

		
	}
	FUNCTION_END;
	return 0;
}


int CPlayerManager::PlayerLogin(LPCTSTR szAccountName ,LPCTSTR szPassword )
{
	FUNCTION_BEGIN;

	int Result=PLRT_FAILED;
	
	if(m_CurMapID.ID!=0)
	{

		if(CreatePlayer(GetCurMsgSenderID(),szAccountName,szPassword))
		{
			Log("���(%s)�ɹ���½��",szAccountName);
			Result=PLRT_OK;
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			Log("���(%s)��½ʱ��������ʧ��",szAccountName);
		}
	}
	else
	{		
		Log("���(%s)��½ʱ��������δ׼��������",szAccountName);
	}
	CPlayerManagerAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),GetCurMsgSenderID());

	MsgCaller.PlayerLoginAck(Result,0);
	return COMMON_RESULT_SUCCEED;
	FUNCTION_END;
	return COMMON_RESULT_FAILED;
}








