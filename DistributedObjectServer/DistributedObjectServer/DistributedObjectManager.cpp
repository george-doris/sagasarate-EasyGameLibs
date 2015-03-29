#include "StdAfx.h"


CDistributedObjectManager::CDistributedObjectManager(void)
{
	FUNCTION_BEGIN;
	m_pDOSObjectManager=NULL;
	FUNCTION_END;
}

CDistributedObjectManager::~CDistributedObjectManager(void)
{
}

BOOL CDistributedObjectManager::Init(CDOSObjectManager * pDOSObjectManager,UINT MaxObjectCount)
{
	FUNCTION_BEGIN;
	m_pDOSObjectManager=pDOSObjectManager;
	if(!m_ObjectPool.Create(MaxObjectCount))
	{
		Log("�޷�����%u��С�Ĳ�������",MaxObjectCount);
		return FALSE;
	}

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDistributedObjectManager::Destory()
{
	FUNCTION_BEGIN;
	m_pDOSObjectManager=NULL;
	m_ObjectPool.Destory();
	FUNCTION_END;
}

BOOL CDistributedObjectManager::RegisterObject(DOS_OBJECT_REGISTER_INFO_EX& ObjectRegisterInfo)
{
	FUNCTION_BEGIN;
	if(m_pDOSObjectManager&&ObjectRegisterInfo.pObject)
	{
		CDistributedObjectOperator * pObjectOperator=CreateObjectOperator(ObjectRegisterInfo.pObject,ObjectRegisterInfo.ObjectID);
		if(pObjectOperator)
		{
			DOS_OBJECT_REGISTER_INFO RegisterInfo;

			RegisterInfo.ObjectID=ObjectRegisterInfo.ObjectID;
			RegisterInfo.pObject=pObjectOperator;
			RegisterInfo.Weight=ObjectRegisterInfo.Weight;
			RegisterInfo.ObjectGroupIndex=ObjectRegisterInfo.ObjectGroupIndex;
			RegisterInfo.MsgQueueSize=ObjectRegisterInfo.MsgQueueSize;
			RegisterInfo.MsgProcessLimit=ObjectRegisterInfo.MsgProcessLimit;
			RegisterInfo.Param=ObjectRegisterInfo.Param;
			
			if(m_pDOSObjectManager->RegisterObject(RegisterInfo))
			{
				//LogDebug("CDistributedObjectManager::RegisterObject:�ɹ�ע�����0x%llX",
				//	pObjectOperator->GetObjectID().ID);
				return TRUE;
			}
			else
			{
				Log("CDistributedObjectManager::RegisterObject:ע�����0x%llXʧ��",
					ObjectRegisterInfo.ObjectID.ID);
			}
		}
		
	}
	
	FUNCTION_END;
	return FALSE;
}

BOOL CDistributedObjectManager::UnregisterObject(CDistributedObjectOperator * pObjectOperator)
{
	FUNCTION_BEGIN;
	if(m_pDOSObjectManager&&pObjectOperator)
	{
		
		if(m_pDOSObjectManager->UnregisterObject(pObjectOperator->GetObjectID()))
		{
			//LogDebug("CDistributedObjectManager::UnregisterObject:�ɹ�ע������0x%llX",
			//	pObjectOperator->GetObjectID().ID);
			return TRUE;
		}
		else
		{
			Log("CDistributedObjectManager::UnregisterObject:ע������0x%llXʧ��",
				pObjectOperator->GetObjectID().ID);
		}
	}
	FUNCTION_END;
	return FALSE;
}

CDistributedObjectOperator * CDistributedObjectManager::CreateObjectOperator(IDistributedObject * pObject,OBJECT_ID ObjectID)
{
	FUNCTION_BEGIN;
	CDistributedObjectOperator * pObjectOperator=NULL;
	UINT ID=m_ObjectPool.NewObject(&pObjectOperator);
	if(pObjectOperator)
	{
		if(pObjectOperator->Init(this,ID,pObject))
		{
			//LogDebug("CDistributedObjectManager::CreateObjectOperator:�ɹ��½�����%u[%llX],���ж���%u",ID,ObjectID.ID,m_ObjectPool.GetObjectCount());
			return pObjectOperator;
		}
		else
		{
			m_ObjectPool.DeleteObject(ID);
		}
	}
	LogDebug("CDistributedObjectManager::CreateObjectOperator:�½�����ʧ��,���ж���%u",m_ObjectPool.GetObjectCount());
	FUNCTION_END;	
	return NULL;

}

BOOL CDistributedObjectManager::DeleteObjectOperator(CDistributedObjectOperator * pObjectOperator)
{
	FUNCTION_BEGIN;	
	UINT ID=pObjectOperator->GetPoolID();
	
	if(m_ObjectPool.DeleteObject(ID))
	{
		//LogDebug("CDistributedObjectManager::CreateObjectOperator:ɾ������%u,���ж���%u",ID,m_ObjectPool.GetObjectCount());
		return TRUE;
	}
	else
	{
		LogDebug("CDistributedObjectManager::CreateObjectOperator:ɾ������%uʧ��,���ж���%u",ID,m_ObjectPool.GetObjectCount());
	}
	FUNCTION_END;
	return FALSE;
}

void CDistributedObjectManager::PrintObjectCount()
{
	Log("һ����%u���������",
		m_ObjectPool.GetObjectCount());
}