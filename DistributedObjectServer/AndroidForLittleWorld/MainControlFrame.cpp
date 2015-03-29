// MainControlFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainControlFrame.h"



// CMainControlFrame

IMPLEMENT_DYNCREATE(CMainControlFrame, CMDIChildWnd)

CMainControlFrame::CMainControlFrame()
{
	m_pView=NULL;
}

CMainControlFrame::~CMainControlFrame()
{
}


BEGIN_MESSAGE_MAP(CMainControlFrame, CMDIChildWnd)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CMainControlFrame ��Ϣ�������

void CMainControlFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_MINIMIZE);
	//CMDIChildWnd::OnClose();
}

int CMainControlFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	CCreateContext CreateContext;

	CreateContext.m_pCurrentDoc=NULL;
	CreateContext.m_pCurrentFrame=this;
	CreateContext.m_pLastView=NULL;
	CreateContext.m_pNewDocTemplate=NULL;
	CreateContext.m_pNewViewClass=RUNTIME_CLASS(CMainControlView);

	m_pView=(CMainControlView *)CreateView(&CreateContext);

	if(m_pView==NULL)
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	return 0;
}

void CMainControlFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pView)
		m_pView->SetFocus();	
}

BOOL CMainControlFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// ����ͼ��һ�γ��Ը�����
	//if (m_View.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

