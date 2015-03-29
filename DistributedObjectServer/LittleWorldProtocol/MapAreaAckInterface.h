#pragma once

//
class IMapAreaAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	RegisterMapObjectAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterMapObjectAck(UINT64 ObjectID ,int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UnregisterMapObjectAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterMapObjectAck(UINT64 ObjectID ,int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UpdateMoveStatusAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UpdateMoveStatusAck(UINT64 ObjectID ,int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SendEventMsgPacketAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int SendEventMsgPacketAck(UINT64 ObjectID ,int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	NewMapObject
	��;:	
	����:
    ObjectID				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int NewMapObject(UINT64 ObjectID ,CSmartStruct& MoveStatus ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	DeleteMapObject
	��;:	
	����:
    ObjectID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteMapObject(UINT64 ObjectID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	MoveStatusUpdate
	��;:	
	����:
    ObjectID				
    IsForce				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int MoveStatusUpdate(UINT64 ObjectID ,BYTE IsForce ,CSmartStruct& MoveStatus ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	EventMsgPacket
	��;:	
	����:
    EventMsg				
	����ֵ:���ر�����
	*************************************************/
	virtual int EventMsgPacket(CSmartStruct& EventMsg ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	HideOnMapAck
	��;:	
	����:
    ObjectID				
    HideType				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int HideOnMapAck(UINT64 ObjectID ,BYTE HideType ,int Result ) {return COMMON_RESULT_FAILED;}
	


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
	virtual int TeleportInMapAck(UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir ,int Result ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum REGISTERMAPOBJECTACK_MEMBER_IDS
	{
		REGISTERMAPOBJECTACK_MEMBER_OBJECTID=1,
		REGISTERMAPOBJECTACK_MEMBER_RESULT=2,
	};

	enum UNREGISTERMAPOBJECTACK_MEMBER_IDS
	{
		UNREGISTERMAPOBJECTACK_MEMBER_OBJECTID=1,
		UNREGISTERMAPOBJECTACK_MEMBER_RESULT=2,
	};

	enum UPDATEMOVESTATUSACK_MEMBER_IDS
	{
		UPDATEMOVESTATUSACK_MEMBER_OBJECTID=1,
		UPDATEMOVESTATUSACK_MEMBER_RESULT=2,
	};

	enum SENDEVENTMSGPACKETACK_MEMBER_IDS
	{
		SENDEVENTMSGPACKETACK_MEMBER_OBJECTID=1,
		SENDEVENTMSGPACKETACK_MEMBER_RESULT=2,
	};

	enum NEWMAPOBJECT_MEMBER_IDS
	{
		NEWMAPOBJECT_MEMBER_OBJECTID=1,
		NEWMAPOBJECT_MEMBER_MOVESTATUS=2,
	};

	enum DELETEMAPOBJECT_MEMBER_IDS
	{
		DELETEMAPOBJECT_MEMBER_OBJECTID=1,
	};

	enum MOVESTATUSUPDATE_MEMBER_IDS
	{
		MOVESTATUSUPDATE_MEMBER_OBJECTID=1,
		MOVESTATUSUPDATE_MEMBER_ISFORCE=2,
		MOVESTATUSUPDATE_MEMBER_MOVESTATUS=3,
	};

	enum EVENTMSGPACKET_MEMBER_IDS
	{
		EVENTMSGPACKET_MEMBER_EVENTMSG=1,
	};

	enum HIDEONMAPACK_MEMBER_IDS
	{
		HIDEONMAPACK_MEMBER_OBJECTID=1,
		HIDEONMAPACK_MEMBER_HIDETYPE=2,
		HIDEONMAPACK_MEMBER_RESULT=3,
	};

	enum TELEPORTINMAPACK_MEMBER_IDS
	{
		TELEPORTINMAPACK_MEMBER_OBJECTID=1,
		TELEPORTINMAPACK_MEMBER_TARGETPOSX=2,
		TELEPORTINMAPACK_MEMBER_TARGETPOSY=3,
		TELEPORTINMAPACK_MEMBER_TARGETHEIGHT=6,
		TELEPORTINMAPACK_MEMBER_TARGETDIR=4,
		TELEPORTINMAPACK_MEMBER_RESULT=5,
	};


};
