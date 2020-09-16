
// LiveUpdateDlg.h : ͷ�ļ�
//

#pragma once
#include "BtnST\BtnST.h"
#include "ProgressBar\ProgressCtrlX.h"
#include "ComEngine.h"


// CLiveUpdateDlg �Ի���

// CLiveUpdateDlg �Ի���
class CLiveUpdateDlg : public CDialogEx
{
// ����
public:
	CLiveUpdateDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LIVEUPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	afx_msg LRESULT OnProgressSetRange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressSetPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressStepIt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadEnd(WPARAM wParam, LPARAM lParam);

private:
	void UpdateMessage(LPCTSTR lpszMessage, DWORD dwMilliseconds = 0);

private:
	CButtonST m_STBtnUpgrade;
	CButtonST m_STBtnLater;
	CProgressCtrlX m_PRG;

	HANDLE m_hCancel;
	CComEngine* m_pComEngine;


	HANDLE m_hCreatePackage;	// �򿪵İ�װ�����
};
