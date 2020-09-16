// OverviewContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "OverviewContainer.h"
#include "MemDC.h"
#include "LightCanvas.hpp"
#include "CommonFunc.h"
#include "CommUtil.h"

#include "MainDlg.h"

// ����
TCHAR OV_DATA_TEXT[][18] = {
	_T("����"), _T("����"), _T("�ǵ�"), _T("����"), _T("�Ƿ�"),
	_T("��"), _T("����"), _T("���"), _T("����"), _T("���"),
	_T("�ܶ�"), _T("����"), _T("��ͣ"), _T("��ͣ"), _T("����"),
	_T("����"), _T("����"), _T("����") };
#define OV_DATA_COUNT _countof(OV_DATA_TEXT)

// COverviewContainer

IMPLEMENT_DYNAMIC(COverviewContainer, CContainer)

COverviewContainer::COverviewContainer()
{
	m_pfText = ::GetFont(_T("����"), 11);

	for(int i = 0; i < OV_DATA_COUNT; i++)
		m_vecClfSplash.push_back(COLOR_SPLASH_4);

	SetTimer(TIMERID_SPLASH, INTERVAL_SPLASH, NULL);
}

COverviewContainer::~COverviewContainer()
{
	FREE_P(m_pfText)
}


BEGIN_MESSAGE_MAP(COverviewContainer, CContainer)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// COverviewContainer ��Ϣ�������

void COverviewContainer::OnPaint()
{
	CPaintDC dc(this);
	
	// �ų����̱��Ի��򣬲�ˢ��
	CJpbDlg *pJpbDlg = dynamic_cast<CMainDlg *>(AfxGetApp()->GetMainWnd())->m_pJpbDlg;
	ASSERT(pJpbDlg);
	if (pJpbDlg->m_bShow)
	{
		CRect jpbRect;
		pJpbDlg->GetClientRect(jpbRect);
		ExcludeClipRect(dc, jpbRect.left, jpbRect.top, jpbRect.right + 30, jpbRect.bottom - 2);
	}

	CMyMemDC MemDC(&dc);
	CLightCanvas canvas(MemDC);
	canvas.SetBkMode();

	CRect rect;
	GetClientRect(&rect);

	// ����ɫ����
	canvas.DrawRect(rect, COLOR_BLACK_BG, COLOR_BLACK_BG);
	DrawBorder(&MemDC, BORDER_LEFT | BORDER_RIGHT);

	TThostFtdcPriceType data[OV_DATA_COUNT] = {
		m_ovData.LastPrice, m_ovData.AvgPrice, m_ovData.ZhangDie, m_ovData.HuanShouRate, m_ovData.ZhangDieFu, m_ovData.OpenPrice,
		m_ovData.ZongShou, m_ovData.HighestPrice, m_ovData.XianShou, m_ovData.LowestPrice, m_ovData.Amount, m_ovData.LiangBi, m_ovData.ZhangTing, m_ovData.DieTing,
		m_ovData.TotalSell, m_ovData.AvgSellPrice, m_ovData.TotalBuy, m_ovData.AvgBuyPrice
	};

	// ����֮�������
	int w_sep = 10;
	int w = (rect.Width() - w_sep) / 2;
	int h = 20;

	for (int i = 0; i < OV_DATA_COUNT; i++)
	{
		int nRow = (i / 2);
		CRect leftRect (3,         h * nRow + 1, w,                 h * (nRow + 1) + 1);
		CRect rightRect(w + w_sep, h * nRow + 1, w * 2 + w_sep - 3, h * (nRow + 1) + 1);
		CRect curRect = (i % 2 == 0) ? leftRect : rightRect;

		// ��������
		CString strTime(OV_DATA_TEXT[i]);
		canvas.SetTextColor(COLOR_WHITE_TEXT);
		canvas.DrawText(strTime, &curRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_pfText);

		// ����
		canvas.SetTextColor(COLOR_RED_TEXT);
		CString strData = _T("---");
		if (i == 6 || i == 8 || i == 14 || i == 16)	// Vol����
		{
			TThostFtdcVolumeType val = (TThostFtdcVolumeType)data[i];
			if (val != 0)
			{
				CString strDisp;
				GetKXZSString(data[i], strDisp);
				strData.Format(_T("%s"), strDisp);
				//strData.Format(_T("%d"), val);
			}
		}
		else if (i == 4 || i == 2)	// �ǵ������ǵ�
		{
			if (!ZERO_DOUBLE(data[i]) && DBL_MAX != data[i])
			{
				canvas.SetTextColor((data[i] > 0) ? COLOR_RED_TEXT : COLOR_GREEN_TEXT);
				CString percentFormat;
				percentFormat.Format(_T("%s%%%%"), (i == 4) ? _T("%.2f") : GetFloatFormat(pJpbDlg->m_curXiaoshu));
				strData.Format((i == 4) ? percentFormat : GetFloatFormat(pJpbDlg->m_curXiaoshu), data[i]);
			}
		}
		else if (i == 10)	// �ܶ�
		{
			canvas.SetTextColor(COLOR_CYAN_TEXT);
			if (!ZERO_DOUBLE(data[i]) && DBL_MAX != data[i])
			{
				CString strDisp;
				GetKXZSString(data[i], strDisp);
				strData.Format(_T("%s"), strDisp);
			}
		}
		else
		{
			// ��ͣ����ͣ���ɫ
			if (i == 9 || i == 13)
				canvas.SetTextColor(COLOR_GREEN_TEXT);

			if (i == 0 && data[2] < 0) //����
				canvas.SetTextColor(COLOR_GREEN_TEXT);
			
			if (!ZERO_DOUBLE(data[i]) && DBL_MAX != data[i])
				strData.Format(GetFloatFormat(pJpbDlg->m_curXiaoshu), data[i]);
		}

		// ��������ɫ����
		if (strData.Compare(_T("---")))
		{
			curRect.DeflateRect(32, 0, 0, 0);
			canvas.DrawRect(curRect, m_vecClfSplash[i], m_vecClfSplash[i]);
		}

		canvas.DrawText(strData, &curRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
}

void COverviewContainer::OnRecvData(CThostFtdcDepthMarketDataField *pData)
{
	OV_DATA data;

	if (!ZERO_DOUBLE(pData->PreSettlementPrice) && DBL_MAX != pData->PreSettlementPrice)
	{
		data.ZhangDie = pData->LastPrice - pData->PreSettlementPrice;  //��ǰ���¼ۼ�ȥ��һ�����¼�Ϊ�ǵ�
		data.ZhangDieFu = data.ZhangDie * 100 / pData->PreSettlementPrice;  //�ǵ��İٷֱ�
	}

	data.LastPrice = pData->LastPrice;
	data.AvgPrice =	pData->AveragePrice;
	
	data.HuanShouRate = pData->BidPrice3;
	
	data.OpenPrice = pData->OpenPrice;
	data.ZongShou = pData->OpenInterest;  ////
	data.HighestPrice = pData->HighestPrice;
	data.XianShou = pData->Volume;   ////
	data.LowestPrice = pData->LowestPrice;
	data.Amount = pData->Turnover;
	//data.LiangBi = pData->;
	data.ZhangTing = pData->UpperLimitPrice;
	data.DieTing = pData->LowerLimitPrice;
	//data.TotalSell = pData->;
	//data.AvgSellPrice = pData->;
	//data.TotalBuy = pData->;
	//data.AvgBuyPrice = pData->;

	// �ҳ������仯������
	CompareOvData(data);

	m_ovData = data;

	// �ػ�
	Invalidate();
}

// ��Ʊ����
void COverviewContainer::OnRecvData(CSecurityFtdcDepthMarketDataField *pData)
{
	OV_DATA data;

	if (!ZERO_DOUBLE(pData->PreClosePrice) && DBL_MAX != pData->PreClosePrice)
	{
		data.ZhangDie = pData->LastPrice - pData->PreClosePrice;  //��ǰ���¼ۼ�ȥ��һ�����¼�Ϊ�ǵ�
		data.ZhangDieFu = data.ZhangDie * 100 / pData->PreClosePrice;  //�ǵ��İٷֱ�
	}

	data.LastPrice = pData->LastPrice;
	data.AvgPrice =	pData->AveragePrice;
	
	data.HuanShouRate = pData->BidPrice3;
	
	data.OpenPrice = pData->OpenPrice;
	data.ZongShou = pData->OpenInterest;  ////
	data.HighestPrice = pData->HighestPrice;
	data.XianShou = pData->Volume;   ////
	data.LowestPrice = pData->LowestPrice;
	data.Amount = pData->Turnover;
	//data.LiangBi = pData->;
	data.ZhangTing = pData->UpperLimitPrice;
	data.DieTing = pData->LowerLimitPrice;
	//data.TotalSell = pData->;
	//data.AvgSellPrice = pData->;
	//data.TotalBuy = pData->;
	//data.AvgBuyPrice = pData->;

	// �ҳ������仯������
	CompareOvData(data);

	m_ovData = data;

	// �ػ�
	Invalidate();
}

void COverviewContainer::CompareOvData(const OV_DATA &data)
{
	if (m_ovData.LastPrice != data.LastPrice)
		m_vecClfSplash[0] = COLOR_SPLASH_4;
	if (m_ovData.AvgPrice != data.AvgPrice)
		m_vecClfSplash[1] = COLOR_SPLASH_4;
	if (m_ovData.ZhangDie != data.ZhangDie)
		m_vecClfSplash[2] = COLOR_SPLASH_4;
	if (m_ovData.HuanShouRate != data.HuanShouRate)
		m_vecClfSplash[3] = COLOR_SPLASH_4;
	if (m_ovData.ZhangDieFu != data.ZhangDieFu)
		m_vecClfSplash[4] = COLOR_SPLASH_4;
	if (m_ovData.OpenPrice != data.OpenPrice)
		m_vecClfSplash[5] = COLOR_SPLASH_4;
	if (m_ovData.ZongShou != data.ZongShou)
		m_vecClfSplash[6] = COLOR_SPLASH_4;
	if (m_ovData.HighestPrice != data.HighestPrice)
		m_vecClfSplash[7] = COLOR_SPLASH_4;
	if (m_ovData.XianShou != data.XianShou)
		m_vecClfSplash[8] = COLOR_SPLASH_4;
	if (m_ovData.LowestPrice != data.LowestPrice)
		m_vecClfSplash[9] = COLOR_SPLASH_4;
	if (m_ovData.Amount != data.Amount)
		m_vecClfSplash[10] = COLOR_SPLASH_4;
	if (m_ovData.LiangBi != data.LiangBi)
		m_vecClfSplash[11] = COLOR_SPLASH_4;
	if (m_ovData.ZhangTing != data.ZhangTing)
		m_vecClfSplash[12] = COLOR_SPLASH_4;
	if (m_ovData.DieTing != data.DieTing)
		m_vecClfSplash[13] = COLOR_SPLASH_4;
	if (m_ovData.TotalSell != data.TotalSell)
		m_vecClfSplash[14] = COLOR_SPLASH_4;
	if (m_ovData.AvgSellPrice != data.AvgSellPrice)
		m_vecClfSplash[15] = COLOR_SPLASH_4;
	if (m_ovData.TotalBuy != data.TotalBuy)
		m_vecClfSplash[16] = COLOR_SPLASH_4;
	if (m_ovData.AvgBuyPrice != data.AvgBuyPrice)
		m_vecClfSplash[17] = COLOR_SPLASH_4;
}
