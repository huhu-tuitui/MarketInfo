#pragma once
#include "BaseContainer.h"
#include "VerticalScrollbar.h"

#include <vector>
using namespace std;

// ���������
#define SCROLLBAR_WIDTH	12
// ������λ��
#define SCROLLBAR_OFFSET 24

// �б���ʾ��ĸ߶�
#define LIST_ITEM_HEIGHT 20
// ��ʳɽ�ʱ����
#define ITEM_TIME_WIDTH  60

typedef struct _ZHUBI_DATA {
	// ʱ��
	TThostFtdcTimeType Time;
	// �ɽ��۸�
	TThostFtdcPriceType Price;
	// �ɽ���
	TThostFtdcVolumeType Vol;
	// �ɽ���(����)
	TThostFtdcVolumeType Vol2;

	_ZHUBI_DATA(CString time, double price, int vol, int vol2)
	{
		sprintf_s(Time, "%S", time);
		Price = price;
		Vol = vol;
		Vol2 = vol2;
	}

	void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	_ZHUBI_DATA()
	{
		Reset();
	}

} ZHUBI_DATA, *PZHUBI_DATA;

// CZhubiContainer

class CZhubiContainer : public CContainer
{
	DECLARE_DYNAMIC(CZhubiContainer)

public:
	CZhubiContainer();
	virtual ~CZhubiContainer();

	virtual void OnRecvData(CThostFtdcDepthMarketDataField *pData);
	virtual void OnRecvData(CSecurityFtdcDepthMarketDataField *pData);

	virtual void ClearData();

protected:

	afx_msg void OnPaint();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnVScrollBarChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	void InitScrollBarPosition();
	BOOL InitScrollBar();

	//��Ӧ�������Ļص�
	//�Ϲ�һ��
	BOOL ScrollUp();
	//�Ϲ�һҳ
	BOOL PageUp();
	//�¹�һ��
	BOOL ScrollDown();
	//�¹�һҳ
	BOOL PageDown();
	//���þ�����
	BOOL ScrollRowTo(int nRow);

	void DrawList( CDC *pDC );

private:
	CVerticalScrollbar *m_pVerticleScrollbar;	//������ָ��
	bool m_bDrawScrollBar;
	CRect m_rScrollBar;

	int m_nMaxLine;		// �������
	int m_nTopIndex;	// ��������
	vector<ZHUBI_DATA>	m_vecDataList;

	bool m_bScrollBottom; // �����������Ƿ�����ײ�
};

