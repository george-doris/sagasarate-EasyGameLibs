#pragma once


class CCharacterMsgCaller :
	public ICharacter
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CCharacterMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CCharacterMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CCharacterMsgCaller(void);
	
	

	/*************************************************
	������:	QueryCharInfo
	��;:	
	����:
    RequesterID				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryCharInfo(OBJECT_ID RequesterID );
	


	/*************************************************
	������:	QueryMove
	��;:	
	����:
    MoveStatus				
    StartPosX				
    StartPosY				
    StartHeight				
    TargetPosX				
    TargetPosY				
    TargetHeight				
    RotateStatus				
    Direction				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryMove(BYTE MoveStatus ,float StartPosX ,float StartPosY ,float StartHeight ,float TargetPosX ,float TargetPosY ,float TargetHeight ,BYTE RotateStatus ,float Direction );
	


	/*************************************************
	������:	AliveTest
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int AliveTest();
	


	/*************************************************
	������:	QueryFly
	��;:	
	����:
    MapID				
    PosX				
    PosY				
    Height				
    Direction				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryFly(UINT MapID ,float PosX ,float PosY ,float Height ,float Direction );
	


	/*************************************************
	������:	MapChangeConfirm
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int MapChangeConfirm(int Result );
	


	/*************************************************
	������:	QueryJump
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int QueryJump();
	

	bool PackMsgQueryCharInfo(CSmartStruct& MsgPacket,OBJECT_ID RequesterID );
	bool PackMsgQueryMove(CSmartStruct& MsgPacket,BYTE MoveStatus ,float StartPosX ,float StartPosY ,float StartHeight ,float TargetPosX ,float TargetPosY ,float TargetHeight ,BYTE RotateStatus ,float Direction );
	bool PackMsgAliveTest(CSmartStruct& MsgPacket);
	bool PackMsgQueryFly(CSmartStruct& MsgPacket,UINT MapID ,float PosX ,float PosY ,float Height ,float Direction );
	bool PackMsgMapChangeConfirm(CSmartStruct& MsgPacket,int Result );
	bool PackMsgQueryJump(CSmartStruct& MsgPacket);

};
