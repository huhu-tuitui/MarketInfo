#ifndef _UTILCOM_H_
#define _UTILCOM_H_

#include "macro.h"
#include "stdafx.h"

// get last error message
COMMON_DLL_EXPORT void GetLastErrorString(CString& strErrMsg);

enum NUM_UNIT_TYPE			// ���־���
{
	NUM_UNIT_0  = 0,		// ��λ��
	NUM_UNIT_WAN,			// ��
	NUM_UNIT_YI,			// ��
	NUM_UNIT_WANYI,			// ����
};

NUM_UNIT_TYPE GetEffectivdNum( double dbNum, TCHAR *pSzChar, int nSize );

// ��ѧ��ʾ����
COMMON_DLL_EXPORT void GetKXZSString( double dbTmp, CString &strText );

// С������ָ��λ��ת��Ϊ�ַ���(bClearZero:ĩβ�Ƿ�ȥ�㣬��С�����ؿ�)
COMMON_DLL_EXPORT void Xiaoshu2String(double d, int n, CString& outStr, bool bClearZero = false);



// ��������
#define POSITIVE_LIMIT				0.00001
// ��������
#define NEGATIVE_LIMIT				-0.00001

// ������
static inline bool IsPositive( double fNum )
{
	return fNum >= POSITIVE_LIMIT;
}

// �Ǹ���
static inline bool IsNegative( double fNum )
{
	return fNum <= NEGATIVE_LIMIT;
}

// �Ƿ�Ϊ��
static inline bool IsZeroValue( double fNum)
{
	return NEGATIVE_LIMIT < fNum && fNum < POSITIVE_LIMIT;
}

// �����������뷨������������
COMMON_DLL_EXPORT double SiSheWuRuFloatValue(double fNum, int nDecimal);

// �����������뷨������������
COMMON_DLL_EXPORT double SiSheWuRuInt64FloatValue(double fNum, int nDecimal);

#endif // _UTILCOM_H_