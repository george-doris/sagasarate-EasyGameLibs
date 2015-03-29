#pragma once

//
class IPlayerCharDBAck:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	AccountLoginAck
	��;:	
	����:
    Result				
    AccountID				
    AccountData				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLoginAck(int Result ,UINT64 AccountID ,CSmartStruct& AccountData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	AccountLogoutAck
	��;:	
	����:
    Result				
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogoutAck(int Result ,UINT64 AccountID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	GetAccountCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetAccountCharAck(int Result ,UINT64 AccountID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	CreateCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateCharAck(int Result ,UINT64 AccountID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	DeleteCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteCharAck(int Result ,UINT64 AccountID ,UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	GetCharDataAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharDataAck(int Result ,UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SaveCharDataAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int SaveCharDataAck(int Result ,UINT64 AccountID ,UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum ACCOUNTLOGINACK_MEMBER_IDS
	{
		ACCOUNTLOGINACK_MEMBER_RESULT=1,
		ACCOUNTLOGINACK_MEMBER_ACCOUNTID=2,
		ACCOUNTLOGINACK_MEMBER_ACCOUNTDATA=3,
	};

	enum ACCOUNTLOGOUTACK_MEMBER_IDS
	{
		ACCOUNTLOGOUTACK_MEMBER_RESULT=1,
		ACCOUNTLOGOUTACK_MEMBER_ACCOUNTID=2,
	};

	enum GETACCOUNTCHARACK_MEMBER_IDS
	{
		GETACCOUNTCHARACK_MEMBER_RESULT=1,
		GETACCOUNTCHARACK_MEMBER_ACCOUNTID=2,
		GETACCOUNTCHARACK_MEMBER_CHARDATA=3,
	};

	enum CREATECHARACK_MEMBER_IDS
	{
		CREATECHARACK_MEMBER_RESULT=1,
		CREATECHARACK_MEMBER_ACCOUNTID=2,
		CREATECHARACK_MEMBER_CHARDATA=3,
	};

	enum DELETECHARACK_MEMBER_IDS
	{
		DELETECHARACK_MEMBER_RESULT=1,
		DELETECHARACK_MEMBER_ACCOUNTID=2,
		DELETECHARACK_MEMBER_CHARID=3,
	};

	enum GETCHARDATAACK_MEMBER_IDS
	{
		GETCHARDATAACK_MEMBER_RESULT=1,
		GETCHARDATAACK_MEMBER_ACCOUNTID=2,
		GETCHARDATAACK_MEMBER_CHARID=3,
		GETCHARDATAACK_MEMBER_CHARDATA=4,
	};

	enum SAVECHARDATAACK_MEMBER_IDS
	{
		SAVECHARDATAACK_MEMBER_RESULT=1,
		SAVECHARDATAACK_MEMBER_ACCOUNTID=2,
		SAVECHARDATAACK_MEMBER_CHARID=3,
	};


};
