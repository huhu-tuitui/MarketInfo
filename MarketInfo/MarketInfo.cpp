
// MarketInfo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MarketInfo.h"
//#include "MarketInfoDlg.h"
#include "MainDlg.h"
#include "VerticalScrollbar.h"
#include "CommonFunc.h"
#include "MyBugTrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LIVE_UPDATE_FILENAME	_T("LiveUpdate.exe")
#define COMM_LIB_FILENAME		_T("commlib.dll")

// CMarketInfoApp

BEGIN_MESSAGE_MAP(CMarketInfoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMarketInfoApp ����

CMarketInfoApp::CMarketInfoApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMarketInfoApp ����

CMarketInfoApp theApp;


// CMarketInfoApp ��ʼ��

BOOL CMarketInfoApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// ������־����
#ifndef _DEBUG
	SetLoggerLevel(LEVEL_ERROR);
#endif

	// �������������֪
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	// ����һ���߳�������������
	DWORD dwThread;
	HANDLE hThread = ::CreateThread(NULL, 0, CheckUpdate, 0, 0, &dwThread);
	if (NULL == hThread)
		TRACE("�����µ��̳߳���\n");

	//CMarketInfoDlg dlg;
	CMainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

static DWORD dwLiveUpdateTrdID = 0;

int CMarketInfoApp::ExitInstance()
{
	FREE_P(CVerticalScrollbar::m_pUpArrow)
	FREE_P(CVerticalScrollbar::m_pDownArrow)
	FREE_P(CVerticalScrollbar::m_pSpan)
	FREE_P(CVerticalScrollbar::m_pThumb1)
	FREE_P(CVerticalScrollbar::m_pThumb2)

	// �ͷ�����apiָ��
	CMainDlg::m_pApi->Release();
	FREE_P(CMainDlg::m_pUserSpi);

	// �رո��¶Ի���
	::PostThreadMessage(dwLiveUpdateTrdID, WM_QUIT, 0, 0);

	return TRUE;
}

DWORD WINAPI CheckUpdate( LPVOID lparam )
{
	//::Sleep(1000);

	TCHAR szPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);

	TCHAR szLiveUpdateFile[_MAX_PATH] = {0};
	_stprintf_s(szLiveUpdateFile, _MAX_PATH, _T("%s\\%s"), szPath, LIVE_UPDATE_FILENAME);
	TCHAR szCommLibFile[_MAX_PATH] = {0};
	_stprintf_s(szCommLibFile, _MAX_PATH, _T("%s\\%s"), szPath, COMM_LIB_FILENAME);

	
	// ��ֹLiveUpdate.exe�޷�����������򣬽����򿽱���tmpĿ¼
	CString tmpPath;
	ExpandEnvironmentStrings(_T("%temp%"), tmpPath.GetBuffer(MAX_PATH), MAX_PATH);
	//CreateDirectory(tmpPath, NULL);

	CString tmpLiveUpdate, tmpCommLib;
	tmpLiveUpdate.Format(_T("%s\\%s"), tmpPath, LIVE_UPDATE_FILENAME);
	::CopyFile(szLiveUpdateFile, tmpLiveUpdate, FALSE);
	tmpCommLib.Format(_T("%s\\%s"), tmpPath, COMM_LIB_FILENAME);
	::CopyFile(szCommLibFile, tmpCommLib, FALSE);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset( &si, 0, sizeof( si ) );
	si.cb = sizeof( si );
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;

	BOOL fRet = CreateProcess( tmpLiveUpdate, NULL, NULL, FALSE, NULL, NULL, NULL, szPath, &si, &pi );

	if (!fRet)
		LOG_ERROR( _T("�������³���:%sʧ�ܣ�"), tmpLiveUpdate );

	dwLiveUpdateTrdID = pi.dwThreadId;

	return 0;
}