#ifndef __REC_STATISTIC_DATA_H_
#define __REC_STATISTIC_DATA_H_

#include "macro.h"
#include <Windows.h>

// ��¼ͳ�����
#define STATISTICS_TYPE_DLGPOS	1	// ����λ��

COMMON_DLL_EXPORT void RecStatisticsData(int nType, LPCTSTR lpszData);
COMMON_DLL_EXPORT void SendStatisticsData();

#endif	// __REC_STATISTIC_DATA_H_