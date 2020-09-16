// Container.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "BaseContainer.h"
#include "CommonFunc.h"
#include "MemDC.h"
#include "LightCanvas.hpp"

// CContainer

IMPLEMENT_DYNAMIC(CContainer, CWnd)

CContainer::CContainer()
{
	// ע�ᴰ��
	CString strWndClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
		GetSysColorBrush(COLOR_WINDOW), NULL);

	// ��������
	CreateEx(NULL, strWndClass, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, AfxGetMainWnd()->m_hWnd, NULL);

	m_pfTitle = ::GetFont(_T("����"), 10);
	m_pfSystemUnderLine = ::GetFont(_T("system"), 9, FW_NORMAL, 1);
}

CContainer::~CContainer()
{
	FREE_P(m_pfTitle)
	FREE_P(m_pfSystemUnderLine)
}

BEGIN_MESSAGE_MAP(CContainer, CWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CContainer ��Ϣ�������
int CContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CContainer::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
}

void CContainer::DrawTitle( CDC *pDC, CString strTitle )
{
	CLightCanvas canvas(pDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	// ���ⱳ��
	CRect rectTitle;
	rectTitle.SetRect(0, 0, rect.Width(), TITLE_HEIGHT);
	canvas.DrawRect(rectTitle, COLOR_GRAY_BORDER, COLOR_TITLE_BG);

	// ����
	CSize titleSize = pDC->GetTextExtent(strTitle);
	CRect tmpRect = rect;
	tmpRect.DeflateRect( (rect.Width() - titleSize.cx) / 2, 6, 0, 0);
	canvas.SetTextColor(RGB(255, 255, 255));
	canvas.DrawText(strTitle, &tmpRect, DT_LEFT | DT_TOP | DT_SINGLELINE, m_pfTitle);
}

void CContainer::DrawBorder(CDC *pDC, DWORD dwBorderType)
{
	CLightCanvas canvas(pDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	if (dwBorderType & BORDER_TOP)
		canvas.DrawLine(0, 0, COLOR_GRAY_BORDER, 1, rect.right, 0);

	if (dwBorderType & BORDER_LEFT)
		canvas.DrawLine(0, 0, COLOR_GRAY_BORDER, 1, 0, rect.bottom);

	if (dwBorderType & BORDER_BOTTOM)
		canvas.DrawLine(0, rect.bottom - 1, COLOR_GRAY_BORDER, 1, rect.right, rect.bottom - 1);

	if (dwBorderType & BORDER_RIGHT)
		canvas.DrawLine(rect.right - 1, 0, COLOR_GRAY_BORDER, 1, rect.right - 1, rect.bottom);
}

void CContainer::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMERID_SPLASH:
		{
			for (int i = 0; i < (int)m_vecClfSplash.size(); i++)
			{
				if (m_vecClfSplash[i] == COLOR_SPLASH_4)
					m_vecClfSplash[i] = COLOR_SPLASH_3;
				else if (m_vecClfSplash[i] == COLOR_SPLASH_3)
					m_vecClfSplash[i] = COLOR_SPLASH_2;
				else if (m_vecClfSplash[i] == COLOR_SPLASH_2)
					m_vecClfSplash[i] = COLOR_SPLASH_1;
			}

			Invalidate();
		}
		break;

	default:
		break;
	}

	__super::OnTimer(nIDEvent);
}