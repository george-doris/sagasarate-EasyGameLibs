#pragma once


class CPlayerManagerMsgCaller :
	public IPlayerManager
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CPlayerManagerMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CPlayerManagerMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CPlayerManagerMsgCaller(void);
	
	

	/*************************************************
	������:	PlayerLogin
	��;:	
	����:
    szAccountName				
    szPassword				
	����ֵ:���ر�����
	*************************************************/
	virtual int PlayerLogin(LPCTSTR szAccountName ,LPCTSTR szPassword );
	

	bool PackMsgPlayerLogin(CSmartStruct& MsgPacket,LPCTSTR szAccountName ,LPCTSTR szPassword );

};
