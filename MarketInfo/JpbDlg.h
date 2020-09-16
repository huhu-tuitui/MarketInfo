#pragma once
#include <map>
#include <vector>
using namespace std;

typedef struct _JPB_DATA {
	// �ڻ�����
	string code;
	// �ڻ�����
	string name;
	// ���Ƽ�ƴ
	string name_jp;
	// �г�����
	string marketcode;
	// �г�����
	string market;
	// ������λС��
	int xiaoshu;

	_JPB_DATA(string c, string n, string m, int x)
	{
		code = c;
		name = n;
		name_jp = "";//TODO
		marketcode = "";
		market = m;
		xiaoshu = x;
	}
	_JPB_DATA(string c, string n, string m, string mc, int x)
	{
		code = c;
		name = n;
		name_jp = "";//TODO
		marketcode = "";
		market = m;
		marketcode = mc;
		xiaoshu = x;
	}

	void Reset()
	{
		code = "";
		name = "";
		name_jp = "";
		marketcode = "";
		market = "";
		xiaoshu = 2;
	}

	_JPB_DATA()
	{
		Reset();
	}

} JPB_DATA, *PJPB_DATA;


// �б���߶�
#define JPB_ITEM_HEIGHT 22
// �б�����ʾ����
#define JPB_ITEM_NUM	8

// CJpbDlg

class CJpbDlg : public CWnd
{
	DECLARE_DYNAMIC(CJpbDlg)

public:
	CJpbDlg();
	virtual ~CJpbDlg();	

	// ��ʾ/���ؼ��̱��Ի���
	void Show(BOOL bShow);
	BOOL	m_bShow;

	void DoSearch(CString strSearch);

	// Ʒ������
	map<string, JPB_DATA> m_mapData;
	
	int m_curXiaoshu;

protected:

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	LRESULT OnJpbKeyDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnJpbKeyMouseWheel(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	// ����Ƿ�ѡ��ĳTab�������Index�����򷵻�-1
	int HitTest(CPoint point);

	void SelectedItemUp();
	void SelectedItemDown();

	static DWORD WINAPI ThreadCallback(LPVOID);

	// bHasMarketCode:�ڻ�û���г����룬��Ʊ��ETF����֤����֤֮��
	void LoadConfig(LPCTSTR dat, int nXiaoshu, bool bHasMarketCode);

private:
	CFont *m_pfMarket, *m_pfName;

	// ѡ��������
	int m_nTotalCount;
	int	m_nSelectedIndex;
	int m_nTopIndex;
	string m_curCode;
	CString m_strSearch;	// ���̱���������

	vector<JPB_DATA> m_vecDisp;
};


