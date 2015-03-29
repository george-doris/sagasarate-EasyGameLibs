#pragma once

//
class IPlayerCharDB:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	AccountLogin
	��;:	
	����:
    szAccountName				
    szPassword				
    szClientIP				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogin(LPCTSTR szAccountName ,LPCTSTR szPassword ,LPCTSTR szClientIP ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	AccountLogout
	��;:	
	����:
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogout(UINT64 AccountID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	GetAccountChar
	��;:	
	����:
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetAccountChar(UINT64 AccountID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	CreateChar
	��;:	
	����:
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateChar(UINT64 AccountID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	DeleteChar
	��;:	
	����:
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteChar(UINT64 AccountID ,UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	GetCharData
	��;:	
	����:
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharData(UINT64 AccountID ,UINT64 CharID ) {return COMMON_RESULT_FAILED;}
	


	/*************************************************
	������:	SaveCharData
	��;:	
	����:
    AccountID				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int SaveCharData(UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum ACCOUNTLOGIN_MEMBER_IDS
	{
		ACCOUNTLOGIN_MEMBER_SZACCOUNTNAME=1,
		ACCOUNTLOGIN_MEMBER_SZPASSWORD=2,
		ACCOUNTLOGIN_MEMBER_SZCLIENTIP=3,
	};

	enum ACCOUNTLOGOUT_MEMBER_IDS
	{
		ACCOUNTLOGOUT_MEMBER_ACCOUNTID=1,
	};

	enum GETACCOUNTCHAR_MEMBER_IDS
	{
		GETACCOUNTCHAR_MEMBER_ACCOUNTID=1,
	};

	enum CREATECHAR_MEMBER_IDS
	{
		CREATECHAR_MEMBER_ACCOUNTID=1,
		CREATECHAR_MEMBER_CHARDATA=2,
	};

	enum DELETECHAR_MEMBER_IDS
	{
		DELETECHAR_MEMBER_ACCOUNTID=1,
		DELETECHAR_MEMBER_CHARID=2,
	};

	enum GETCHARDATA_MEMBER_IDS
	{
		GETCHARDATA_MEMBER_ACCOUNTID=1,
		GETCHARDATA_MEMBER_CHARID=2,
	};

	enum SAVECHARDATA_MEMBER_IDS
	{
		SAVECHARDATA_MEMBER_ACCOUNTID=1,
		SAVECHARDATA_MEMBER_CHARID=2,
		SAVECHARDATA_MEMBER_CHARDATA=3,
	};


};
