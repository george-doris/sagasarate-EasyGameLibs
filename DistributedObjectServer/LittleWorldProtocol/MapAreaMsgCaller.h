#pragma once


class CMapAreaMsgCaller :
	public IMapArea
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CMapAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CMapAreaMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CMapAreaMsgCaller(void);
	
	

	/*************************************************
	������:	RegisterMapObject
	��;:	
	����:
    ObjectID				
    ControllerID				
    AreaEventReceiverID				
    MoveStatus				
    Flag				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterMapObject(UINT64 ObjectID ,OBJECT_ID ControllerID ,OBJECT_ID AreaEventReceiverID ,CSmartStruct& MoveStatus ,UINT Flag );
	


	/*************************************************
	������:	UnregisterMapObject
	��;:	
	����:
    ObjectID				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterMapObject(UINT64 ObjectID );
	


	/*************************************************
	������:	UpdateMoveStatus
	��;:	
	����:
    ObjectID				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int UpdateMoveStatus(UINT64 ObjectID ,CSmartStruct& MoveStatus );
	


	/*************************************************
	������:	SendEventMsgPacket
	��;:	
	����:
    ObjectID				
    EventMsgPacket				
	����ֵ:���ر�����
	*************************************************/
	virtual int SendEventMsgPacket(UINT64 ObjectID ,CSmartStruct& EventMsgPacket );
	


	/*************************************************
	������:	HideOnMap
	��;:	
	����:
    ObjectID				
    HideType				
	����ֵ:���ر�����
	*************************************************/
	virtual int HideOnMap(UINT64 ObjectID ,BYTE HideType );
	


	/*************************************************
	������:	TeleportInMap
	��;:	
	����:
    ObjectID				
    TargetPosX				
    TargetPosY				
    TargetHeight				
    TargetDir				
	����ֵ:���ر�����
	*************************************************/
	virtual int TeleportInMap(UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir );
	

	bool PackMsgRegisterMapObject(CSmartStruct& MsgPacket,UINT64 ObjectID ,OBJECT_ID ControllerID ,OBJECT_ID AreaEventReceiverID ,CSmartStruct& MoveStatus ,UINT Flag );
	bool PackMsgUnregisterMapObject(CSmartStruct& MsgPacket,UINT64 ObjectID );
	bool PackMsgUpdateMoveStatus(CSmartStruct& MsgPacket,UINT64 ObjectID ,CSmartStruct& MoveStatus );
	bool PackMsgSendEventMsgPacket(CSmartStruct& MsgPacket,UINT64 ObjectID ,CSmartStruct& EventMsgPacket );
	bool PackMsgHideOnMap(CSmartStruct& MsgPacket,UINT64 ObjectID ,BYTE HideType );
	bool PackMsgTeleportInMap(CSmartStruct& MsgPacket,UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir );

};
