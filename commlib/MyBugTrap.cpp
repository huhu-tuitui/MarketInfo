#include "stdafx.h"
#include "BugTrap\BugTrap.h"
#include "MyBugTrap.h"

static void SetupExceptionHandler()
{
	LOG_TRACER();
	
	BT_InstallSehFilter();

	// ������Ϣ
    BT_SetAppName(_T("MarketInfo"));
	BT_SetDialogMessage(BTDM_INTRO1, _T("We're so Sorry, program crashed because of our mistakes !"));
	BT_SetDialogMessage(BTDM_INTRO2, _T("������᳢���ռ�����˴��쳣������ص����ݣ��뽫�˱��淢�͸������Ա�������ǿ���������õĲ�Ʒ��������ɵĲ���֮�������½⣡"));
    BT_SetSupportEMail(_T("futures_bugreport@tsingfun.com"));
	// BTF_DETAILEDMODE������ʱ��¼dump�ļ��������õĻ�������û��dump�ļ�
	// BTF_ATTACHREPORT���㡱���͵�������ͨ�����������ʼ����ͱ���
	// BTF_SCREENCAPTURE�����󱨸��и���һ�ű���ʱ��Ļ��ͼ

	// BTF_EDITMAIL���㡱���͵������Է����ʼ���ָ����ַ(û�и��������ɸ���Ŀ�ĵ�ַ)
	// BTF_LISTPROCESSES���г�����ʱ���еĽ�����Ϣ(�ٶȽ���)
	// BTF_SHOWADVANCEDUI��������Ĭ����ʾ��ϸ�Ի���(�����õĻ�������ʾ�򵥶Ի����в鿴��ϸ�İ�ť)	
	// BTF_DESCRIBEERROR�����ͱ���֮ǰ�������������Ի������û��������������Ϣ
	BT_SetFlags(BTF_DETAILEDMODE | BTF_ATTACHREPORT | BTF_SCREENCAPTURE);
    BT_SetSupportServer(_T("futures.tsingfun.com"), 9999);
    BT_SetSupportURL(_T("http://futures.tsingfun.com"));

	// Add custom log file using default name
	/*INT_PTR iLogHandle = BT_OpenLogFile(NULL, BTLF_TEXT);
	BT_SetLogSizeInEntries(iLogHandle, 100);
	BT_SetLogFlags(iLogHandle, BTLF_SHOWTIMESTAMP);
	BT_SetLogEchoMode(iLogHandle, BTLE_STDERR | BTLE_DBGOUT);

	LPCTSTR lpszFileName = BT_GetLogFileName(iLogHandle);
	BT_AddLogFile(lpszFileName);	
	
	//BT_InsLogEntry(iLogHandle, BTLL_INFO, _T("custom log message"));
	//BT_InsLogEntryF(iLogHandle, BTLL_WARNING, _T("numeric output: %d"), 123);
	
	BT_CloseLogFile(iLogHandle);*/

	// ���µ�Log�ļ�����
	TCHAR szLogFile[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szLogFile);
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	_stprintf_s(szLogFile, _T("%s\\logs\\%4d%02d%02d.log"), szLogFile, sys.wYear, sys.wMonth, sys.wDay);
	BT_AddLogFile(szLogFile);
	
	//BTTrace trace(_T("c:\\test.log"), BTLF_TEXT);
	//trace.Append(BTLL_INFO, _T("custom log message trace"));
}

// Windows�¸�֪��������ķ�����3�����ĵĺ������ֱ����£�
//	SetUnhandledExceptionFilter   (HandleException)ȷ������û�п��Ƶ��쳣����ʱ����HandleException.
//	_set_invalid_parameter_handler(HandleInvalidParameter)ȷ��������Ч�������÷���ʱ����HandleInvalidParameter.
//	_set_purecall_handler         (HandlePureVirtualCall)ȷ�����麯�����÷���ʱ����HandlePureVirtualCall.

// Example: SetUnhandledExceptionFilter(TFUnhandledExceptionFilter); *((int*)0x0) = 0;

LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionPointers)
{
	LOG_TRACER();

	SetupExceptionHandler();
	return EXCEPTION_CONTINUE_EXECUTION;
}