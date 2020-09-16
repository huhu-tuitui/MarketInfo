
// LiveUpdate.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "LiveUpdate.h"
#include "LiveUpdateDlg.h"
#include "Logger.h"
#include "FileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DOWNLOAD_URL		_T("http://futures.tsingfun.com/update.ini")
#define INI_FILENAME		_T("update.ini")
#define MAIN_EXE_FILENAME	_T("MarketInfo.exe")

// CLiveUpdateApp

BEGIN_MESSAGE_MAP(CLiveUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLiveUpdateApp ����

CLiveUpdateApp::CLiveUpdateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLiveUpdateApp ����

CLiveUpdateApp theApp;


// CLiveUpdateApp ��ʼ��

BOOL CLiveUpdateApp::InitInstance()
{
	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

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

	// ����Ƿ���Ҫ����
	if (!CheckUpdate())
		return FALSE;

	CLiveUpdateDlg dlg;
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
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
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

bool CLiveUpdateApp::CheckUpdate()
{
	LOG_INFO(_T("��ʼ����%s"), INI_FILENAME);
	TCHAR szPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);

	TCHAR szFile[_MAX_PATH] = {0};
	_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), szPath, INI_FILENAME);
	// ��������ļ����ڣ���ɾ��
	if (-1 != _taccess(szFile,NULL))
		::DeleteFile(szFile);

	CString strErrMsg;
	unsigned long nFileSize = 0;
	
	CComEngine *pComEngine = CComEngine::GetComEngine();
	if (!pComEngine)
	{
		LOG_ERROR(_T("GetComEngineʧ�ܣ��˳�"));
		return false;
	}

	// ��ͬ����ʽ����
	if (!pComEngine->Download(NULL, NULL, DOWNLOAD_URL, 
		INI_FILENAME, strErrMsg, nFileSize))
	{
		LOG_ERROR(strErrMsg);
		// �������
		OutputDebugString(strErrMsg);

		return false;
	}

	// ������ɣ��жϰ汾��
	if (GetLatestVersion().Compare(GetCurrentVersion()) > 0)
		return true;
	else 
		return false;

	return true;
}

CString CLiveUpdateApp::GetCurrentVersion()
{
	CString version;
	CFileVersion fversion;
	TCHAR szPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);
	TCHAR szFile[_MAX_PATH] = {0};
	_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), szPath, MAIN_EXE_FILENAME);
	if (fversion.Open(szFile))
		version = fversion.GetFileVersion();
	else
		LOG_ERROR(_T("��ȡ������%s �汾��ʧ�ܣ�"), szFile);

	return version;
}

CString CLiveUpdateApp::GetLatestVersion()
{
	TCHAR szPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);
	TCHAR szFile[_MAX_PATH] = {0};
	_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), szPath, INI_FILENAME);

	CString version;
	::GetPrivateProfileString(_T("update"), _T("version"), NULL, version.GetBuffer(MAX_PATH), MAX_PATH, szFile);

	return version;
}