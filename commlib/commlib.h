// commlib.h : commlib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcommlibApp
// �йش���ʵ�ֵ���Ϣ������� commlib.cpp
//

class CcommlibApp : public CWinApp
{
public:
	CcommlibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
