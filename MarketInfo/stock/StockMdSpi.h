#ifndef _STOCK_MDSPI_H_
#define _STOCK_MDSPI_H_

#include "SecurityMdApi/SecurityFtdcMdApi.h"

class StockMdSpi : public CSecurityFtdcMdSpi
{
public:
	StockMdSpi(CSecurityFtdcMdApi* api):pUserApi(api){};
	///����Ӧ��
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin,	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);

public:
	void ReqUserLogin(TSecurityFtdcBrokerIDType	appId,
		TSecurityFtdcUserIDType	userId,	TSecurityFtdcPasswordType	passwd);
	void SubscribeMarketData(char* instIdList, char* exchangeID);
	void UnSubscribeMarketData(char* instIdList, char* exchangeID);
	bool IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo);
private:
	CSecurityFtdcMdApi* pUserApi;
};

#endif	// _STOCK_MDSPI_H_