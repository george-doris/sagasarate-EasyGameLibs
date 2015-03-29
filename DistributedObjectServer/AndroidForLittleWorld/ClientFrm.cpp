// ChildFrm.cpp : CClientFrame ���ʵ��
//
#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientFrame

IMPLEMENT_DYNCREATE(CClientFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CClientFrame, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, &CClientFrame::OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CClientFrame ����/����

CClientFrame::CClientFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_pView=NULL;
}

CClientFrame::~CClientFrame()
{
}


BOOL CClientFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CClientFrame ���

#ifdef _DEBUG
void CClientFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CClientFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CClientFrame ��Ϣ�������
void CClientFrame::OnFileClose() 
{
	// ��Ҫ�رտ�ܣ�ֻ�跢�� WM_CLOSE��
	// ���൱�ڴ�ϵͳ�˵���ѡ��رա�
	SendMessage(WM_CLOSE);
}

int CClientFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CCreateContext CreateContext;

	CreateContext.m_pCurrentDoc=NULL;
	CreateContext.m_pCurrentFrame=this;
	CreateContext.m_pLastView=NULL;
	CreateContext.m_pNewDocTemplate=NULL;
	CreateContext.m_pNewViewClass=RUNTIME_CLASS(CClientInfoView);

	m_pView=(CClientInfoView *)CreateView(&CreateContext);

	if(m_pView==NULL)
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	return 0;
}

void CClientFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_pView->SetFocus();
}

BOOL CClientFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// ����ͼ��һ�γ��Ը�����
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;
	
	// ����ִ��Ĭ�ϴ���
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
