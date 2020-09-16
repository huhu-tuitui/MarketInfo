// TabNavContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "TabNavContainer.h"
#include "MemDC.h"
#include "LightCanvas.hpp"

// CTabNavContainer

IMPLEMENT_DYNAMIC(CTabNavContainer, CContainer)

CTabNavContainer::CTabNavContainer()
{

}

CTabNavContainer::~CTabNavContainer()
{
}


BEGIN_MESSAGE_MAP(CTabNavContainer, CContainer)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTabNavContainer ��Ϣ�������

void CTabNavContainer::OnPaint()
{
	CPaintDC dc(this);
	CMyMemDC MemDC(&dc);
	CLightCanvas canvas(MemDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	// ����ɫ����
	canvas.DrawRect(rect, COLOR_BLACK_BG, COLOR_BLACK_BG);
	DrawBorder(&MemDC, BORDER_BOTTOM);
}
