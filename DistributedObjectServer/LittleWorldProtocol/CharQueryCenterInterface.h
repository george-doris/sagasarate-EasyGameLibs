#pragma once

//
class ICharQueryCenter:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	RegisterChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterChar(UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	UnregisterChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterChar(UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	QueryCharInfo
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int QueryCharInfo(UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum REGISTERCHAR_MEMBER_IDS
	{
		REGISTERCHAR_MEMBER_CHARID=1,
	};

	enum UNREGISTERCHAR_MEMBER_IDS
	{
		UNREGISTERCHAR_MEMBER_CHARID=1,
	};

	enum QUERYCHARINFO_MEMBER_IDS
	{
		QUERYCHARINFO_MEMBER_CHARID=1,
	};


};
