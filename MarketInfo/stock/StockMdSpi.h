#ifndef _STOCK_MDSPI_H_
#define _STOCK_MDSPI_H_

#include "SecurityMdApi/SecurityFtdcMdApi.h"

class StockMdSpi : public CSecurityFtdcMdSpi
{
public:
	StockMdSpi(CSecurityFtdcMdApi* api):pUserApi(api){};
	///错误应答
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin,	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
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