#pragma once


// CMainControlFrame ���

class CMainControlFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMainControlFrame)
public:
	CMainControlFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMainControlFrame();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CMainControlView *		m_pView;
public:
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	CMainControlView * GetView()
	{
		return m_pView;
	}
};


