// FenshiContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "FenshiContainer.h"
#include "MemDC.h"
#include "LightCanvas.hpp"

// CFenshiContainer

IMPLEMENT_DYNAMIC(CFenshiContainer, CContainer)

CFenshiContainer::CFenshiContainer()
{

}

CFenshiContainer::~CFenshiContainer()
{
}


BEGIN_MESSAGE_MAP(CFenshiContainer, CContainer)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CFenshiContainer ��Ϣ�������

void CFenshiContainer::OnPaint()
{
	CPaintDC dc(this);
	CMyMemDC MemDC(&dc);
	CLightCanvas canvas(MemDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	// ����ɫ����
	canvas.DrawRect(rect, COLOR_BLACK_BG, COLOR_BLACK_BG);
	DrawBorder(&MemDC, BORDER_RIGHT | BORDER_BOTTOM);

	DrawTitle(&MemDC, _T("��ʱ�ɽ�"));

	// �����ⱳ��ȱ��
	canvas.DrawLine(0, 0, COLOR_TITLE_BG, 1, 0, TITLE_HEIGHT);
	canvas.DrawLine(0, 0, COLOR_GRAY_BORDER, 1, 1, 1);
	canvas.DrawLine(0, TITLE_HEIGHT - 1, COLOR_GRAY_BORDER, 1, 1, TITLE_HEIGHT - 1);
}
