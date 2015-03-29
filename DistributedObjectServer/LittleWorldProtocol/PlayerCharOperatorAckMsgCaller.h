#pragma once


class CPlayerCharOperatorAckMsgCaller :
	public IPlayerCharOperatorAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CPlayerCharOperatorAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CPlayerCharOperatorAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CPlayerCharOperatorAckMsgCaller(void);
	
	

	/*************************************************
	������:	GetCharListAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharListAck(int Result ,CSmartStruct& CharData );
	


	/*************************************************
	������:	CreateCharAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateCharAck(int Result ,CSmartStruct& CharData );
	


	/*************************************************
	������:	DeleteCharAck
	��;:	
	����:
    Result				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteCharAck(int Result ,UINT64 CharID );
	


	/*************************************************
	������:	SelectCharAck
	��;:	
	����:
    Result				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int SelectCharAck(int Result ,UINT64 CharID ,CSmartStruct& CharData );
	

	bool PackMsgGetCharListAck(CSmartStruct& MsgPacket,int Result ,CSmartStruct& CharData );
	bool PackMsgCreateCharAck(CSmartStruct& MsgPacket,int Result ,CSmartStruct& CharData );
	bool PackMsgDeleteCharAck(CSmartStruct& MsgPacket,int Result ,UINT64 CharID );
	bool PackMsgSelectCharAck(CSmartStruct& MsgPacket,int Result ,UINT64 CharID ,CSmartStruct& CharData );

};
