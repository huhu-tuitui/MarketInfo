#include <stdafx.h>
#include <tchar.h>
#include "Logger.h"

void SetLoggerLevel(int nLevel)
{
	if (LEVEL_DEBUG < nLevel || nLevel < LEVEL_FATAL)
	{
		LOG_WARN(_T("������־����:%d ʧ�ܣ�Ĭ��INFO����"), nLevel);
		return;
	}
	nLoggerLevel = nLevel;
	LOG_INFO(_T("������־����:%s �ɹ���"), LOGGER_LEVEL_NAME[nLevel]);
}

void Log(INT nLevel, LPCSTR func, INT line, LPCTSTR fmt, ...)
{
	if (nLevel > nLoggerLevel)
		return;

	// ���õ��򣬲�Ȼfprintfд��������
	setlocale(0, "chs");

	FILE *fp = NULL;
	TCHAR szLogFile[MAX_PATH] = {0};
	va_list args;
	va_start(args, fmt);

	GetCurrentDirectory(MAX_PATH, szLogFile);
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	
	// ����LogĿ¼
	_stprintf_s(szLogFile, _T("%s\\logs\\"), szLogFile);
	::_tmkdir(szLogFile);
	_stprintf_s(szLogFile, _T("%s\\%4d%02d%02d.log"), szLogFile, sys.wYear, sys.wMonth, sys.wDay);
	
	fopen_s(&fp, CW2A(szLogFile), "a+");
	if (!fp) 
	{
		TCHAR szOutput[MAX_PATH] = {0};
		char errMsg[MAX_PATH] = { 0 };
		strerror_s(errMsg, MAX_PATH, errno);
		_stprintf_s(szOutput, _T("Open run log file[%s] error: %s\n"), szLogFile, ((CA2T)errMsg).m_szBuffer);
		OutputDebugString(szOutput);
		return;
	}
	fprintf(fp, "[%4d/%02d/%02d %02d:%02d:%02d.%03d][%ld:%ld][%s]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds,
		GetProcessId(NULL), GetThreadId(NULL), LOGGER_LEVEL_CODE[nLevel]);

	//fprintf( fp, "%s:Line%d ", func, line);
	fprintf(fp, "%s ", func);
	//_ftprintf(fp, fmt, args);
	_vftprintf_s(fp, fmt, args);
	fprintf( fp, "\n");

	fclose(fp);
	va_end(args);
}