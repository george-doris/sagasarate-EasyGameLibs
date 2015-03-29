#pragma once


class CMapAreaAckMsgCaller :
	public IMapAreaAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CMapAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CMapAreaAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CMapAreaAckMsgCaller(void);
	
	

	/*************************************************
	������:	RegisterMapObjectAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterMapObjectAck(UINT64 ObjectID ,int Result );
	


	/*************************************************
	������:	UnregisterMapObjectAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterMapObjectAck(UINT64 ObjectID ,int Result );
	


	/*************************************************
	������:	UpdateMoveStatusAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UpdateMoveStatusAck(UINT64 ObjectID ,int Result );
	


	/*************************************************
	������:	SendEventMsgPacketAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int SendEventMsgPacketAck(UINT64 ObjectID ,int Result );
	


	/*************************************************
	������:	NewMapObject
	��;:	
	����:
    ObjectID				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int NewMapObject(UINT64 ObjectID ,CSmartStruct& MoveStatus );
	


	/*************************************************
	������:	DeleteMapObject
	��;:	
	����:
    ObjectID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteMapObject(UINT64 ObjectID );
	


	/*************************************************
	������:	MoveStatusUpdate
	��;:	
	����:
    ObjectID				
    IsForce				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int MoveStatusUpdate(UINT64 ObjectID ,BYTE IsForce ,CSmartStruct& MoveStatus );
	


	/*************************************************
	������:	EventMsgPacket
	��;:	
	����:
    EventMsg				
	����ֵ:���ر�����
	*************************************************/
	virtual int EventMsgPacket(CSmartStruct& EventMsg );
	


	/*************************************************
	������:	HideOnMapAck
	��;:	
	����:
    ObjectID				
    HideType				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int HideOnMapAck(UINT64 ObjectID ,BYTE HideType ,int Result );
	


	/*************************************************
	������:	TeleportInMapAck
	��;:	
	����:
    ObjectID				
    TargetPosX				
    TargetPosY				
    TargetHeight				
    TargetDir				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int TeleportInMapAck(UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir ,int Result );
	

	bool PackMsgRegisterMapObjectAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,int Result );
	bool PackMsgUnregisterMapObjectAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,int Result );
	bool PackMsgUpdateMoveStatusAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,int Result );
	bool PackMsgSendEventMsgPacketAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,int Result );
	bool PackMsgNewMapObject(CSmartStruct& MsgPacket,UINT64 ObjectID ,CSmartStruct& MoveStatus );
	bool PackMsgDeleteMapObject(CSmartStruct& MsgPacket,UINT64 ObjectID );
	bool PackMsgMoveStatusUpdate(CSmartStruct& MsgPacket,UINT64 ObjectID ,BYTE IsForce ,CSmartStruct& MoveStatus );
	bool PackMsgEventMsgPacket(CSmartStruct& MsgPacket,CSmartStruct& EventMsg );
	bool PackMsgHideOnMapAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,BYTE HideType ,int Result );
	bool PackMsgTeleportInMapAck(CSmartStruct& MsgPacket,UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir ,int Result );

};
