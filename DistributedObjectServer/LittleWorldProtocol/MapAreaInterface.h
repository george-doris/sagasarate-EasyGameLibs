#pragma once

//
class IMapArea:public CBaseMsgHandler
{
public:
	

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
	virtual int RegisterMapObject(UINT64 ObjectID ,OBJECT_ID ControllerID ,OBJECT_ID AreaEventReceiverID ,CSmartStruct& MoveStatus ,UINT Flag ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UnregisterMapObject
	��;:	
	����:
    ObjectID				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterMapObject(UINT64 ObjectID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UpdateMoveStatus
	��;:	
	����:
    ObjectID				
    MoveStatus				
	����ֵ:���ر�����
	*************************************************/
	virtual int UpdateMoveStatus(UINT64 ObjectID ,CSmartStruct& MoveStatus ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SendEventMsgPacket
	��;:	
	����:
    ObjectID				
    EventMsgPacket				
	����ֵ:���ر�����
	*************************************************/
	virtual int SendEventMsgPacket(UINT64 ObjectID ,CSmartStruct& EventMsgPacket ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	HideOnMap
	��;:	
	����:
    ObjectID				
    HideType				
	����ֵ:���ر�����
	*************************************************/
	virtual int HideOnMap(UINT64 ObjectID ,BYTE HideType ) {return COMMON_RESULT_FAILED;}
	


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
	virtual int TeleportInMap(UINT64 ObjectID ,float TargetPosX ,float TargetPosY ,float TargetHeight ,float TargetDir ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum REGISTERMAPOBJECT_MEMBER_IDS
	{
		REGISTERMAPOBJECT_MEMBER_OBJECTID=1,
		REGISTERMAPOBJECT_MEMBER_CONTROLLERID=2,
		REGISTERMAPOBJECT_MEMBER_AREAEVENTRECEIVERID=3,
		REGISTERMAPOBJECT_MEMBER_MOVESTATUS=7,
		REGISTERMAPOBJECT_MEMBER_FLAG=8,
	};

	enum UNREGISTERMAPOBJECT_MEMBER_IDS
	{
		UNREGISTERMAPOBJECT_MEMBER_OBJECTID=1,
	};

	enum UPDATEMOVESTATUS_MEMBER_IDS
	{
		UPDATEMOVESTATUS_MEMBER_OBJECTID=1,
		UPDATEMOVESTATUS_MEMBER_MOVESTATUS=11,
	};

	enum SENDEVENTMSGPACKET_MEMBER_IDS
	{
		SENDEVENTMSGPACKET_MEMBER_OBJECTID=1,
		SENDEVENTMSGPACKET_MEMBER_EVENTMSGPACKET=2,
	};

	enum HIDEONMAP_MEMBER_IDS
	{
		HIDEONMAP_MEMBER_OBJECTID=1,
		HIDEONMAP_MEMBER_HIDETYPE=2,
	};

	enum TELEPORTINMAP_MEMBER_IDS
	{
		TELEPORTINMAP_MEMBER_OBJECTID=1,
		TELEPORTINMAP_MEMBER_TARGETPOSX=2,
		TELEPORTINMAP_MEMBER_TARGETPOSY=3,
		TELEPORTINMAP_MEMBER_TARGETHEIGHT=5,
		TELEPORTINMAP_MEMBER_TARGETDIR=4,
	};


};
