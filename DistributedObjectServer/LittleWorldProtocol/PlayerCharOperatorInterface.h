#pragma once

//
class IPlayerCharOperator:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	GetCharList
	��;:	
	����:

	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharList() {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	CreateChar
	��;:	
	����:
    CharCreateData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateChar(CSmartStruct& CharCreateData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	DeleteChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteChar(UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SelectChar
	��;:	
	����:
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int SelectChar(UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum GETCHARLIST_MEMBER_IDS
	{
	};

	enum CREATECHAR_MEMBER_IDS
	{
		CREATECHAR_MEMBER_CHARCREATEDATA=1,
	};

	enum DELETECHAR_MEMBER_IDS
	{
		DELETECHAR_MEMBER_CHARID=1,
	};

	enum SELECTCHAR_MEMBER_IDS
	{
		SELECTCHAR_MEMBER_CHARID=1,
	};


};
