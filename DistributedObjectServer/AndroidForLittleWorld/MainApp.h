// AndroidForLittleWorld.h : AndroidForLittleWorld Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif




// CMainApp:
// �йش����ʵ�֣������ AndroidForLittleWorld.cpp
//

class CMainApp : public CWinApp
{
protected:
	HMENU							m_hMDIMenu;
	HACCEL							m_hMDIAccel;
	CEasyArray<CClientGroup *>		m_GroupList;
	CNetServer						m_Server;

	CMainControlFrame *				m_pMainControlFrame;

	CEasyArray<CEasyString>			m_CommandHistory;

public:
	CMainApp();

	static CMainApp * GetInstance();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	HMENU GetMenu()
	{
		return m_hMDIMenu;
	}
	HACCEL GetAccel()
	{
		return m_hMDIAccel;
	}

	CMainFrame * GetMainFrame()
	{
		return (CMainFrame *)m_pMainWnd;
	}
	CMainControlView * GetMainControlView()
	{
		return m_pMainControlFrame->GetView();
	}
	CClientGroup * GetClientGroup(UINT Index)
	{
		if(Index<m_GroupList.GetCount())
			return m_GroupList[Index];
		return NULL;
	}
	UINT AddCommandHistory(LPCTSTR szCommand);
	LPCTSTR GetCommandHistory(int& Index);

	void OnMainControlViewClose();
	

public:
	afx_msg void OnAppAbout();
	
	DECLARE_MESSAGE_MAP()

protected:
	bool LoadConfig(LPCTSTR szConfigName);
};

extern CMainApp theApp;