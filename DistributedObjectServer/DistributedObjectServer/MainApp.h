// DBProxy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once



// CMainApp:
// �йش����ʵ�֣������ DBProxy.cpp
//

class CMainApp : public CServerApp
{
protected:
	bool		m_IsConfigLoaded;
public:
	CMainApp();

// ��д
	public:
	//virtual BOOL InitInstance();
	//virtual int ExitInstance();

	virtual BOOL OnStartUp();
	virtual void OnShutDown();

// ʵ��
	virtual void OnSetServiceName();
	
};

extern CMainApp theApp;
