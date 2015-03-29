#pragma once

//
class ICharacterAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	QueryCharInfoAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryCharInfoAck(int Result ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	QueryMoveAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryMoveAck(int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	AliveTestAck
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int AliveTestAck() {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	QueryFlyAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryFlyAck(int Result ) {return COMMON_RESULT_FAILED;}
	


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
	virtual int MapChange(UINT MapID ,float PosX ,float PosY ,float Direction ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	QueryJumpAck
	��;:	
	����:
    ObjectID				
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryJumpAck(UINT64 ObjectID ,int Result ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum QUERYCHARINFOACK_MEMBER_IDS
	{
		QUERYCHARINFOACK_MEMBER_RESULT=1,
		QUERYCHARINFOACK_MEMBER_CHARDATA=2,
	};

	enum QUERYMOVEACK_MEMBER_IDS
	{
		QUERYMOVEACK_MEMBER_RESULT=1,
	};

	enum ALIVETESTACK_MEMBER_IDS
	{
	};

	enum QUERYFLYACK_MEMBER_IDS
	{
		QUERYFLYACK_MEMBER_RESULT=1,
	};

	enum MAPCHANGE_MEMBER_IDS
	{
		MAPCHANGE_MEMBER_MAPID=1,
		MAPCHANGE_MEMBER_POSX=2,
		MAPCHANGE_MEMBER_POSY=3,
		MAPCHANGE_MEMBER_DIRECTION=4,
	};

	enum QUERYJUMPACK_MEMBER_IDS
	{
		QUERYJUMPACK_MEMBER_OBJECTID=1,
		QUERYJUMPACK_MEMBER_RESULT=2,
	};


};
