#pragma once
#include "BaseContainer.h"
#include "JpbDlg.h"
#include "resource.h"

#include "MdSpi.h"
#include "stock\StockMdSpi.h"
#include "stock\StockTraderSpi.h"
#include "stock\StockDefine.h"

// CMainDlg �Ի���

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_MAINDLG };

	// ģ��
	CContainer	*m_pZhubiContainer;
	CContainer	*m_pFenshiContainer;
	CContainer	*m_pQuoteContainer;
	CContainer	*m_pOverviewContainer;
	CContainer	*m_pTabNavContainer;

	// ���̱�����
	CJpbDlg	*m_pJpbDlg;

	// ����API
	static CThostFtdcMdApi *m_pApi;
	static MdSpi		   *m_pUserSpi;

	// ��Ʊ����API
	static CSecurityFtdcMdApi *m_pStockApi;
	static StockMdSpi		  *m_pStockUserSpi;

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg LRESULT OnUserSetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSearchEditCommand();
	afx_msg LRESULT OnJpbExChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	// ����ͼ�갴ť��OnPaint�����
	void OnDrawBmpBtn( CDC* pMemDC );

	// ����Ƿ�ѡ��ĳTab�������Index�����򷵻�-1
	int HitTest( CPoint point );

	// ͼ�갴ť��������
	void OnBmpBtnClicked(int nIndex);

private:

	static DWORD WINAPI ThreadCallback(LPVOID);
	static DWORD WINAPI ThreadCallback_Stock(LPVOID);

	// Toolbar���
	enum AllBmpBtn
	{
		BMP_BTN_CLOSE,		// �ر�		
		BMP_BTN_MAX,		// ���
		BMP_BTN_MIN,		// ��С��		
		TOTAL_BMP_BTN_COUNT
	};

	Gdiplus::Bitmap* m_pAllNormalBmpBtn[TOTAL_BMP_BTN_COUNT];
	Gdiplus::Bitmap* m_pAllHoverBmpBtn[TOTAL_BMP_BTN_COUNT];
	CBitmap *m_pCaptionLogo;

	CRect   m_rectToolbar;
	CFont	*m_pfTitle;
	int		m_nHoverIndex;
	BOOL	m_bMouseDown;

	// ������������
	CRect	m_rectTabNav;	// ��ർ����
	CRect	m_rectOverview;	// ���±���һ��
	CRect	m_rectZhubi;	// ��ʳɽ�
	CRect	m_rectQuote10;	// 10������
	CRect	m_rectFenshi;	// ��ʱ�ɽ�

	// ������
	HWND	m_wndEdit;

	// �ϴ�Ʒ�ִ���
	string	m_strLastCode;
};
