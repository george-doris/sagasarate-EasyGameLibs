#pragma once


class CCharacterAckMsgCaller :
	public ICharacterAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CCharacterAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CCharacterAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CCharacterAckMsgCaller(void);
	
	

	/*************************************************
	������:	QueryCharInfoAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryCharInfoAck(int Result ,CSmartStruct& CharData );
	


	/*************************************************
	������:	QueryMoveAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryMoveAck(int Result );
	


	/*************************************************
	������:	AliveTestAck
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int AliveTestAck();
	


	/*************************************************
	������:	QueryFlyAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryFlyAck(int Result );
	


	/*************************************************
	������:	MapChange
	��;:	
	����:
    MapID				
    PosX				
    PosY				
    Direction				
	����ֵ:���ر�����
	*************************************************/
	virtual int MapChange(UINT MapID ,float PosX ,float PosY ,float Direction );
	


	/*************************************************
	������:	QueryJumpAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryJumpAck(UINT64 ObjectID ,int Result );
	

	bool PackMsgQueryCharInfoAck(CSmartStruct& MsgPacket,int Result ,CSmartStruct& CharData );
	bool PackMsgQueryMoveAck(CSmartStruct& MsgPacket,int Result );
	bool PackMsgAliveTestAck(CSmartStruct& MsgPacket);
	bool PackMsgQueryFlyAck(CSmartStruct& MsgPacket,int Result );
	bool PackMsgMapChange(CSmartStruct& MsgPacket,UINT MapID ,float PosX ,float PosY ,float Direction );
	bool PackMsgQueryJumpAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,int Result );

};
