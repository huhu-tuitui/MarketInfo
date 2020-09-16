#pragma once
#include "BaseContainer.h"

typedef struct _OV_DATA {
	// ����
	TThostFtdcPriceType LastPrice;
	// ���¾���
	TThostFtdcPriceType AvgPrice;
	// �ǵ�
	TThostFtdcPriceType ZhangDie;
	// ������
	TThostFtdcPriceType HuanShouRate;
	// �Ƿ�
	TThostFtdcPriceType ZhangDieFu;
	// ��
	TThostFtdcPriceType OpenPrice;
	// ����
	TThostFtdcLargeVolumeType ZongShou;
	// ���
	TThostFtdcPriceType HighestPrice;
	// ����
	TThostFtdcVolumeType XianShou;
	// ���
	TThostFtdcPriceType LowestPrice;
	// �ܶ�
	TThostFtdcPriceType Amount;
	// ����
	TThostFtdcPriceType LiangBi;
	// ��ͣ
	TThostFtdcPriceType ZhangTing;
	// ��ͣ
	TThostFtdcPriceType DieTing;
	// ����
	TThostFtdcVolumeType TotalSell;
	// ��������
	TThostFtdcPriceType AvgSellPrice;
	// ����
	TThostFtdcVolumeType TotalBuy;
	// �������
	TThostFtdcPriceType AvgBuyPrice;

	void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	_OV_DATA()
	{
		Reset();
	}

} OV_DATA, *POV_DATA;

// COverviewContainer

class COverviewContainer : public CContainer
{
	DECLARE_DYNAMIC(COverviewContainer)

public:
	COverviewContainer();
	virtual ~COverviewContainer();

	virtual void OnRecvData(CThostFtdcDepthMarketDataField *pData);
	virtual void OnRecvData(CSecurityFtdcDepthMarketDataField *pData);

protected:

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	void CompareOvData(const OV_DATA &data);

private:
	OV_DATA	m_ovData;
	
	CFont *m_pfText;
};


