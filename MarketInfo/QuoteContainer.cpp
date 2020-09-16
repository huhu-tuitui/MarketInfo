// QuoteContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "QuoteContainer.h"
#include "MemDC.h"
#include "LightCanvas.hpp"
#include "CommonFunc.h"
#include "MainDlg.h"
#include "CommUtil.h"


TCHAR QUOTE_DATA_TEXT[][20] = {
					_T("��ʮ"), _T("����"), _T("����"), _T("����"), _T("����"), 
					_T("����"), _T("����"), _T("����"), _T("����"), _T("��һ"), 
					_T("��һ"), _T("���"), _T("����"), _T("����"), _T("����"), 
					_T("����"), _T("����"), _T("���"), _T("���"), _T("��ʮ")};
#define QUOTE_DATA_COUNT _countof(QUOTE_DATA_TEXT)

// CQuoteContainer

IMPLEMENT_DYNAMIC(CQuoteContainer, CContainer)

CQuoteContainer::CQuoteContainer()
{
	m_pfCodeTitle = ::GetFont(_T("Aparajita"), 20, FW_BOLD);
	m_pfNameTitle = ::GetFont(_T("����"), 15, FW_BOLD);
	m_pfBuySell = ::GetFont(_T("����"), 11);

	for(int i = 0; i < QUOTE_DATA_COUNT; i++)
		m_vecClfSplash.push_back(COLOR_SPLASH_4);

	SetTimer(TIMERID_SPLASH, INTERVAL_SPLASH, NULL);
}

CQuoteContainer::~CQuoteContainer()
{
	FREE_P(m_pfCodeTitle)
	FREE_P(m_pfNameTitle)
	FREE_P(m_pfBuySell)
}


BEGIN_MESSAGE_MAP(CQuoteContainer, CContainer)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CQuoteContainer ��Ϣ�������

void CQuoteContainer::OnPaint()
{
	CPaintDC dc(this);
	CMyMemDC MemDC(&dc);
	CLightCanvas canvas(MemDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	// ����ɫ����
	canvas.DrawRect(rect, COLOR_BLACK_BG, COLOR_BLACK_BG);
	DrawBorder(&MemDC, BORDER_RIGHT);

	// ��Ʒ��
	CRect rectPz = rect;
	rectPz.DeflateRect(3, 0, 0, 0);
	canvas.SetTextColor(COLOR_CYAN_TEXT);
	CString strCode = CString(m_quoteData.Name);
	if (strCode.IsEmpty())
		strCode = ZERO_MARK;
	canvas.DrawText(strCode, &rectPz, DT_LEFT | DT_TOP | DT_SINGLELINE, m_pfCodeTitle);

	int nCodeWidth = (int)(MemDC->GetTextExtent(strCode).cx * 1.4);
	rectPz.DeflateRect(nCodeWidth + 8, 3, 0, 0);
	canvas.SetTextColor(RGB(247, 251, 164));

	// ȡƷ������
	CJpbDlg *pJpbDlg = dynamic_cast<CMainDlg *>(AfxGetApp()->GetMainWnd())->m_pJpbDlg;
	ASSERT(pJpbDlg);
	CString strName( pJpbDlg->m_mapData[string(m_quoteData.Name)].name.data() );
	canvas.DrawText(strName, &rectPz, DT_LEFT | DT_TOP | DT_SINGLELINE, m_pfNameTitle);

	// �ָ���
	canvas.DrawLine(1, 25, COLOR_GRAY_BORDER, 1, rect.Width() - 2, 25);

	// ��ʮ������
	TThostFtdcPriceType PriceArr[QUOTE_DATA_COUNT] = {0, 0, 0, 0, 0, m_quoteData.Sell5, m_quoteData.Sell4, m_quoteData.Sell3, 
		m_quoteData.Sell2, m_quoteData.Sell1, m_quoteData.Buy1, m_quoteData.Buy2, m_quoteData.Buy3, 
		m_quoteData.Buy4, m_quoteData.Buy5, 0, 0, 0, 0, 0};

	TThostFtdcVolumeType VolArr[QUOTE_DATA_COUNT] = { 0, 0, 0, 0, 0, m_quoteData.SellVol5, m_quoteData.SellVol4, m_quoteData.SellVol3,
		m_quoteData.SellVol2, m_quoteData.SellVol1, m_quoteData.BuyVol1, m_quoteData.BuyVol2, m_quoteData.BuyVol3, 
		m_quoteData.BuyVol4, m_quoteData.BuyVol5, 0, 0, 0, 0, 0};

	for(int i = 0; i < QUOTE_DATA_COUNT; i++)
	{
		int w = (rect.Width() - QUOTE_NAME_WIDTH) / 2 - 3;

		int top = 28 + (int)(BUY_SELL_ITEM_HEIGHT * i) + (i > 9 ? 3 : 0);
		int bottom = top + (int)BUY_SELL_ITEM_HEIGHT;

		if (i == 9)
		{
			// �ָ���
			canvas.DrawLine(1, bottom + 1, COLOR_SEP_QUOTE, 1, rect.Width() - 2, bottom + 1);
		}
	
		CRect buySellRect(2, top, QUOTE_NAME_WIDTH, bottom);
		CRect secondRect (QUOTE_NAME_WIDTH + 3, top, QUOTE_NAME_WIDTH + w, bottom);
		CRect thirdRect  (QUOTE_NAME_WIDTH + w, top, QUOTE_NAME_WIDTH + w * 2, bottom);

		// ������ʮ - ��ʮ
		CString strTime(QUOTE_DATA_TEXT[i]);
		canvas.SetTextColor(COLOR_WHITE_TEXT);
		canvas.DrawText(strTime, &buySellRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_pfBuySell);

		// ���Ƴɽ���
		canvas.SetTextColor(COLOR_GREEN_TEXT);
		if (ZERO_DOUBLE(PriceArr[i]) || DBL_MAX == PriceArr[i])
		{
			canvas.DrawText(ZERO_MARK, &secondRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			// ��������ɫ����
			if (i >= 5 || i <= 14)
				canvas.DrawRect(secondRect, m_vecClfSplash[i - 5], m_vecClfSplash[i - 5]);

			CString zhengshu, xiaoshu;
			zhengshu.Format(_T("%d"), (int)PriceArr[i]);
			Xiaoshu2String(PriceArr[i], pJpbDlg->m_mapData[string(m_quoteData.Name)].xiaoshu, xiaoshu);
			
			// �Ȼ��»���С������
			canvas.DrawText(xiaoshu, &secondRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE, m_pfSystemUnderLine);
			// ����������
			CRect tmpRect = secondRect;
			CSize xsSize = MemDC->GetTextExtent(xiaoshu);
			tmpRect.DeflateRect(0, 0, xsSize.cx, 0);

			canvas.DrawText(zhengshu, &tmpRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}

		// ���Ƴɽ���
		canvas.SetTextColor(COLOR_YELLOW_TEXT);
		if (VolArr[i] == 0)
		{
			canvas.DrawText(ZERO_MARK, &thirdRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else
		{
			CString strVol;
			strVol.Format(_T("%d"), VolArr[i]);
			// ��������ɫ����
			if (i >= 5 || i <= 14)
				canvas.DrawRect(thirdRect, m_vecClfSplash[i + 5], m_vecClfSplash[i + 5]);

			canvas.DrawText(strVol, &thirdRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void CQuoteContainer::OnRecvData(CThostFtdcDepthMarketDataField *pData)
{
	TThostFtdcPriceType openPrice = pData->OpenPrice;
	TThostFtdcPriceType maxPrize = openPrice * 1.1;
	TThostFtdcPriceType minPrize = openPrice * 0.9;
	
	QUOTE_DATA data;
	memcpy(data.Name, pData->InstrumentID, 30);
	data.Price = pData->LastPrice;
	data.Buy1 =	pData->BidPrice1;
	data.Buy2 = pData->BidPrice2;
	data.Buy3 = pData->BidPrice3;
	data.Buy4 = pData->BidPrice4;
	data.Buy5 = pData->BidPrice5;
	data.Sell1 = pData->AskPrice1;
	data.Sell2 = pData->AskPrice2;
	data.Sell3 = pData->AskPrice3;
	data.Sell4 = pData->AskPrice4;
	data.Sell5 = pData->AskPrice5;

	data.BuyVol1 =	pData->BidVolume1;
	data.BuyVol2 = pData->BidVolume2;
	data.BuyVol3 = pData->BidVolume3;
	data.BuyVol4 = pData->BidVolume4;
	data.BuyVol5 = pData->BidVolume5;
	data.SellVol1 = pData->AskVolume1;
	data.SellVol2 = pData->AskVolume2;
	data.SellVol3 = pData->AskVolume3;
	data.SellVol4 = pData->AskVolume4;
	data.SellVol5 = pData->AskVolume5;

	if(data.Buy1 < minPrize || data.Buy1 > maxPrize)
	{
		data.Buy1 =	0;
	}
	if(data.Buy2 < minPrize || data.Buy2 > maxPrize)
	{
		data.Buy2 =	0;
	}
	if(data.Buy3 < minPrize || data.Buy3 > maxPrize)
	{
		data.Buy3 =	0;
	}
	if(data.Buy4 < minPrize || data.Buy4 > maxPrize)
	{
		data.Buy4 =	0;
	}
	if(data.Buy5 < minPrize || data.Buy5 > maxPrize)
	{
		data.Buy5 =	0;
	}
	if(data.Sell1 < minPrize || data.Sell1 > maxPrize)
	{
		data.Sell1 = 0;
	}
	if(data.Sell2 < minPrize || data.Sell2 > maxPrize)
	{
		data.Sell2 = 0;
	}
	if(data.Sell3 < minPrize || data.Sell3 > maxPrize)
	{
		data.Sell3 = 0;
	}
	if(data.Sell4 < minPrize || data.Sell4 > maxPrize)
	{
		data.Sell4 = 0;
	}
	if(data.Sell5 < minPrize || data.Sell5 > maxPrize)
	{
		data.Sell5 = 0;
	}

	// �ҳ������仯������
	CompareQuoteData(data);

	m_quoteData = data;

	// �ػ�
	Invalidate();
}

// ��Ʊ����
void CQuoteContainer::OnRecvData(CSecurityFtdcDepthMarketDataField *pData)
{
	TThostFtdcPriceType openPrice = pData->OpenPrice;
	TThostFtdcPriceType maxPrize = openPrice * 1.1;
	TThostFtdcPriceType minPrize = openPrice * 0.9;
	
	QUOTE_DATA data;
	memcpy(data.Name, pData->InstrumentID, 30);
	data.Price = pData->LastPrice;
	data.Buy1 =	pData->BidPrice1;
	data.Buy2 = pData->BidPrice2;
	data.Buy3 = pData->BidPrice3;
	data.Buy4 = pData->BidPrice4;
	data.Buy5 = pData->BidPrice5;
	data.Sell1 = pData->AskPrice1;
	data.Sell2 = pData->AskPrice2;
	data.Sell3 = pData->AskPrice3;
	data.Sell4 = pData->AskPrice4;
	data.Sell5 = pData->AskPrice5;

	data.BuyVol1 =	pData->BidVolume1;
	data.BuyVol2 = pData->BidVolume2;
	data.BuyVol3 = pData->BidVolume3;
	data.BuyVol4 = pData->BidVolume4;
	data.BuyVol5 = pData->BidVolume5;
	data.SellVol1 = pData->AskVolume1;
	data.SellVol2 = pData->AskVolume2;
	data.SellVol3 = pData->AskVolume3;
	data.SellVol4 = pData->AskVolume4;
	data.SellVol5 = pData->AskVolume5;

	if(data.Buy1 < minPrize || data.Buy1 > maxPrize)
	{
		data.Buy1 =	0;
	}
	if(data.Buy2 < minPrize || data.Buy2 > maxPrize)
	{
		data.Buy2 =	0;
	}
	if(data.Buy3 < minPrize || data.Buy3 > maxPrize)
	{
		data.Buy3 =	0;
	}
	if(data.Buy4 < minPrize || data.Buy4 > maxPrize)
	{
		data.Buy4 =	0;
	}
	if(data.Buy5 < minPrize || data.Buy5 > maxPrize)
	{
		data.Buy5 =	0;
	}
	if(data.Sell1 < minPrize || data.Sell1 > maxPrize)
	{
		data.Sell1 = 0;
	}
	if(data.Sell2 < minPrize || data.Sell2 > maxPrize)
	{
		data.Sell2 = 0;
	}
	if(data.Sell3 < minPrize || data.Sell3 > maxPrize)
	{
		data.Sell3 = 0;
	}
	if(data.Sell4 < minPrize || data.Sell4 > maxPrize)
	{
		data.Sell4 = 0;
	}
	if(data.Sell5 < minPrize || data.Sell5 > maxPrize)
	{
		data.Sell5 = 0;
	}

	// �ҳ������仯������
	CompareQuoteData(data);

	m_quoteData = data;

	// �ػ�
	Invalidate();
}

void CQuoteContainer::CompareQuoteData(const QUOTE_DATA &data)
{
	if (m_quoteData.Sell5 != data.Sell5)
		m_vecClfSplash[0] = COLOR_SPLASH_4;
	if (m_quoteData.Sell4 != data.Sell4)
		m_vecClfSplash[1] = COLOR_SPLASH_4;
	if (m_quoteData.Sell3 != data.Sell3)
		m_vecClfSplash[2] = COLOR_SPLASH_4;
	if (m_quoteData.Sell2 != data.Sell2)
		m_vecClfSplash[3] = COLOR_SPLASH_4;
	if (m_quoteData.Sell1 != data.Sell1)
		m_vecClfSplash[4] = COLOR_SPLASH_4;

	if (m_quoteData.Buy1 != data.Buy1)
		m_vecClfSplash[5] = COLOR_SPLASH_4;
	if (m_quoteData.Buy2 != data.Buy2)
		m_vecClfSplash[6] = COLOR_SPLASH_4;
	if (m_quoteData.Buy3 != data.Buy3)
		m_vecClfSplash[7] = COLOR_SPLASH_4;
	if (m_quoteData.Buy4 != data.Buy4)
		m_vecClfSplash[8] = COLOR_SPLASH_4;
	if (m_quoteData.Buy5 != data.Buy5)
		m_vecClfSplash[9] = COLOR_SPLASH_4;

	if (m_quoteData.SellVol5 != data.SellVol5)
		m_vecClfSplash[10] = COLOR_SPLASH_4;
	if (m_quoteData.SellVol4 != data.SellVol4)
		m_vecClfSplash[11] = COLOR_SPLASH_4;
	if (m_quoteData.SellVol3 != data.SellVol3)
		m_vecClfSplash[12] = COLOR_SPLASH_4;
	if (m_quoteData.SellVol2 != data.SellVol2)
		m_vecClfSplash[13] = COLOR_SPLASH_4;
	if (m_quoteData.SellVol1 != data.SellVol1)
		m_vecClfSplash[14] = COLOR_SPLASH_4;

	if (m_quoteData.BuyVol1 != data.BuyVol1)
		m_vecClfSplash[15] = COLOR_SPLASH_4;
	if (m_quoteData.BuyVol2 != data.BuyVol2)
		m_vecClfSplash[16] = COLOR_SPLASH_4;
	if (m_quoteData.BuyVol3 != data.BuyVol3)
		m_vecClfSplash[17] = COLOR_SPLASH_4;
	if (m_quoteData.BuyVol4 != data.BuyVol4)
		m_vecClfSplash[18] = COLOR_SPLASH_4;
	if (m_quoteData.BuyVol5 != data.BuyVol5)
		m_vecClfSplash[19] = COLOR_SPLASH_4;
}