// MapBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapBox.h"



// CMapBox

IMPLEMENT_DYNAMIC(CMapBox, CStatic)

CMapBox::CMapBox()
{
	m_pParent=NULL;
}

CMapBox::~CMapBox()
{
}


BEGIN_MESSAGE_MAP(CMapBox, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMapBox ��Ϣ�������



BOOL CMapBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect Rect;
	GetClientRect(&Rect);
	pDC->FillSolidRect(&Rect,RGB(0,0,0));
	return FALSE;
}

void CMapBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	if(m_pParent)
	{
		m_pParent->DrawMap(&dc);
	}
}

void CMapBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_pParent)
	{
		m_pParent->OnClickMap(point);
	}
	CStatic::OnLButtonUp(nFlags, point);
}
