#pragma once

//
class ICharQueryCenterAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	RegisterCharAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterCharAck(int Result ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UnregisterCharAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterCharAck(int Result ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum REGISTERCHARACK_MEMBER_IDS
	{
		REGISTERCHARACK_MEMBER_RESULT=1,
	};

	enum UNREGISTERCHARACK_MEMBER_IDS
	{
		UNREGISTERCHARACK_MEMBER_RESULT=1,
	};


};
