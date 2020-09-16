#pragma once
#include <vector>
using namespace std;

#include "SecurityMdApi/SecurityFtdcMdApi.h"

// CContainer

// ��ģ�����߶�
static const int TITLE_HEIGHT = 24;

enum BORDER_TYPE{
	BORDER_TOP		= 1 << 1,
	BORDER_LEFT		= 1 << 2,
	BORDER_BOTTOM	= 1 << 3,
	BORDER_RIGHT	= 1 << 4
};

// ˢ���������
#define TIMERID_SPLASH		1
#define INTERVAL_SPLASH		500

class CContainer : public CWnd
{
	DECLARE_DYNAMIC(CContainer)

public:
	CContainer();
	virtual ~CContainer();

	virtual void OnRecvData(CThostFtdcDepthMarketDataField *pData){ };
	virtual void OnRecvData(CSecurityFtdcDepthMarketDataField *pStockData){ };
	virtual void ClearData(){ };

protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawTitle( CDC *pDC, CString strTitle );
	virtual void DrawBorder( CDC *pDC, DWORD dwBorderType);


	// �»�������
	CFont *m_pfSystemUnderLine;

	// �洢ÿ������ı�����ɫ��ˢ������
	vector<COLORREF> m_vecClfSplash;

private:
	CFont *m_pfTitle;
};


