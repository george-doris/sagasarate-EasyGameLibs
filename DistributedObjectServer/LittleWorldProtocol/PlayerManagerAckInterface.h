#pragma once

//
class IPlayerManagerAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	PlayerLoginAck
	��;:	
	����:
    Result				
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int PlayerLoginAck(int Result ,UINT64 AccountID ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum PLAYERLOGINACK_MEMBER_IDS
	{
		PLAYERLOGINACK_MEMBER_RESULT=1,
		PLAYERLOGINACK_MEMBER_ACCOUNTID=2,
	};


};
