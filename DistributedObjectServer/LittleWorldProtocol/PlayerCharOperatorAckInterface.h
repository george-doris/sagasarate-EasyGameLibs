#pragma once

//
class IPlayerCharOperatorAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	GetCharListAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharListAck(int Result ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	CreateCharAck
	��;:	
	����:
    Result				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateCharAck(int Result ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	DeleteCharAck
	��;:	
	����:
    Result				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteCharAck(int Result ,UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SelectCharAck
	��;:	
	����:
    Result				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int SelectCharAck(int Result ,UINT64 CharID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum GETCHARLISTACK_MEMBER_IDS
	{
		GETCHARLISTACK_MEMBER_RESULT=1,
		GETCHARLISTACK_MEMBER_CHARDATA=2,
	};

	enum CREATECHARACK_MEMBER_IDS
	{
		CREATECHARACK_MEMBER_RESULT=1,
		CREATECHARACK_MEMBER_CHARDATA=2,
	};

	enum DELETECHARACK_MEMBER_IDS
	{
		DELETECHARACK_MEMBER_RESULT=1,
		DELETECHARACK_MEMBER_CHARID=2,
	};

	enum SELECTCHARACK_MEMBER_IDS
	{
		SELECTCHARACK_MEMBER_RESULT=1,
		SELECTCHARACK_MEMBER_CHARID=2,
		SELECTCHARACK_MEMBER_CHARDATA=3,
	};


};
