
// LiveUpdate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLiveUpdateApp:
// �йش����ʵ�֣������ LiveUpdate.cpp
//

class CLiveUpdateApp : public CWinApp
{
public:
	CLiveUpdateApp();

// ��д
public:
	virtual BOOL InitInstance();

	static CString GetCurrentVersion();
	static CString GetLatestVersion();

private:
	bool CheckUpdate();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLiveUpdateApp theApp;