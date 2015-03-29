#pragma once


class CPlayerCharOperatorMsgCaller :
	public IPlayerCharOperator
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CPlayerCharOperatorMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CPlayerCharOperatorMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CPlayerCharOperatorMsgCaller(void);
	
	

	/*************************************************
	������:	GetCharList
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharList();
	


	/*************************************************
	������:	CreateChar
	��;:	
	����:
    CharCreateData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateChar(CSmartStruct& CharCreateData );
	


	/*************************************************
	������:	DeleteChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteChar(UINT64 CharID );
	


	/*************************************************
	������:	SelectChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int SelectChar(UINT64 CharID );
	

	bool PackMsgGetCharList(CSmartStruct& MsgPacket);
	bool PackMsgCreateChar(CSmartStruct& MsgPacket,CSmartStruct& CharCreateData );
	bool PackMsgDeleteChar(CSmartStruct& MsgPacket,UINT64 CharID );
	bool PackMsgSelectChar(CSmartStruct& MsgPacket,UINT64 CharID );

};
