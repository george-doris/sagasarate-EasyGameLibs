#pragma once

//
class IPlayerManager:public CBaseMsgHandler
{
public:
	

	/*************************************************
	������:	PlayerLogin
	��;:	
	����:
    szAccountName				
    szPassword				
	����ֵ:���ر�����
	*************************************************/
	virtual int PlayerLogin(LPCTSTR szAccountName ,LPCTSTR szPassword ) {return COMMON_RESULT_FAILED;}
	

	
protected:	
	enum PLAYERLOGIN_MEMBER_IDS
	{
		PLAYERLOGIN_MEMBER_SZACCOUNTNAME=1,
		PLAYERLOGIN_MEMBER_SZPASSWORD=5,
	};


};
