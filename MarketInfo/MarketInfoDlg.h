
// MarketInfoDlg.h : ͷ�ļ�
//

#pragma once
#include "../GridCtrl_src/GridCtrl.h"

// CMarketInfoDlg �Ի���
class CMarketInfoDlg : public CDialogEx
{
// ����
public:
	CMarketInfoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MARKETINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

public:
	void AddOrUpdateRow2List(PSTOCK_DATA pData);
	void SetProgressMsg(CString msg);

private:
	static DWORD WINAPI ThreadCallback(LPVOID);

	void FillGridCtrl(PSTOCK_DATA pData, int row, int col);

private:
	CGridCtrl* m_pGridCtrl;
	CRITICAL_SECTION Critical;
};
