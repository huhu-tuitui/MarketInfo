#include "stdafx.h"
#include <iostream>
#include <vector>

#include "StockTraderSpi.h"
#include "windows.h"

using namespace std;
#pragma warning(disable :4996)

TSecurityFtdcBrokerIDType appId;		// Ӧ�õ�Ԫ
TSecurityFtdcUserIDType userId;		// Ͷ���ߴ���


extern int requestId;

// �Ự����
int	 frontId;	//ǰ�ñ��
int	 sessionId;	//�Ự���
char orderRef[13];

vector<CSecurityFtdcOrderField*> orderList;
vector<CSecurityFtdcTradeField*> tradeList;

char MapDirection(char src, bool toOrig);
char MapOffset(char src, bool toOrig);



void StockTraderSpi::OnFrontConnected()
{
	cerr<<" ���ӽ���ǰ��...�ɹ�"<<endl;
	//SetEvent(g_hEvent);
}

void StockTraderSpi::ReqUserLogin(TSecurityFtdcBrokerIDType	vAppId,
	TSecurityFtdcUserIDType	vUserId,	TSecurityFtdcPasswordType	vPasswd)
{

	CSecurityFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, vAppId); strcpy(appId, vAppId); 
	strcpy(req.UserID, vUserId);  strcpy(userId, vUserId); 
	strcpy(req.Password, vPasswd);
	int ret = pUserApi->ReqUserLogin(&req, ++requestId);
	cerr<<" ���� | ���͵�¼..."<<((ret == 0) ? "�ɹ�" :"ʧ��") << endl;	
}

void StockTraderSpi::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin,
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo(pRspInfo) && pRspUserLogin ) {  
		// ����Ự����	
		frontId = pRspUserLogin->FrontID;
		sessionId = pRspUserLogin->SessionID;
		int nextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		sprintf(orderRef, "%d", ++nextOrderRef);
		cerr<<" ��Ӧ | ��¼�ɹ�...��ǰ������:"
			<<pRspUserLogin->TradingDay<<endl;     
	}
	//if(bIsLast) 
	//	SetEvent(g_hEvent);
}

void StockTraderSpi::ReqQryInstrument(TSecurityFtdcInstrumentIDType instId)
{
	CSecurityFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, instId);//Ϊ�ձ�ʾ��ѯ���к�Լ
	int ret = pUserApi->ReqQryInstrument(&req, ++requestId);
	cerr<<" ���� | ���ͺ�Լ��ѯ..."<<((ret == 0)?"�ɹ�":"ʧ��")<<endl;
}

void StockTraderSpi::OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, 
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 	
	if ( !IsErrorRspInfo(pRspInfo) &&  pInstrument){
		cerr<<" ��Ӧ | ��Լ:"<<pInstrument->InstrumentID
			<<" ����������:"<<pInstrument->ExchangeID<<endl; 
	}
	//if(bIsLast) SetEvent(g_hEvent);
}

void StockTraderSpi::ReqQryTradingAccount()
{
	CSecurityFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);
	strcpy(req.InvestorID, userId);
	int ret = pUserApi->ReqQryTradingAccount(&req, ++requestId);
	cerr<<" ���� | �����ʽ��ѯ..."<<((ret == 0)?"�ɹ�":"ʧ��")<<endl;

}

void StockTraderSpi::OnRspQryTradingAccount(
	CSecurityFtdcTradingAccountField *pTradingAccount, 
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 
	if (!IsErrorRspInfo(pRspInfo) &&  pTradingAccount){
		cerr<<" ��Ӧ | Ȩ��:"<<pTradingAccount->Balance
			<<" ����:"<<pTradingAccount->Available   
			<<" ��֤��:"<<pTradingAccount->CurrMargin
			<<" ������:"<<pTradingAccount->Commission
			<<" ���ᱣ֤��:"<<pTradingAccount->FrozenMargin
			//<<" ����������:"<<pTradingAccount->FrozenCommission 
			<< endl;    
	}
	//if(bIsLast) SetEvent(g_hEvent);
}

void StockTraderSpi::ReqQryInvestorPosition(TSecurityFtdcInstrumentIDType instId)
{
	CSecurityFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);
	strcpy(req.InvestorID, userId);
	strcpy(req.InstrumentID, instId);	
	int ret = pUserApi->ReqQryInvestorPosition(&req, ++requestId);
	cerr<<" ���� | ���ͳֲֲ�ѯ..."<<((ret == 0)?"�ɹ�":"ʧ��")<<endl;
}

void StockTraderSpi::OnRspQryInvestorPosition(
	CSecurityFtdcInvestorPositionField *pInvestorPosition, 
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 
	if( !IsErrorRspInfo(pRspInfo) &&  pInvestorPosition ){
		cerr<<" ��Ӧ | ��Լ:"<<pInvestorPosition->InstrumentID
			<<" ����:"<<MapDirection(pInvestorPosition->PosiDirection-2,false)
			<<" �ֲܳ�:"<<pInvestorPosition->Position
			<<" ���:"<<pInvestorPosition->YdPosition 
			<<" ���:"<<pInvestorPosition->TodayPosition<<endl;
	}
	//if(bIsLast) 
	//	SetEvent(g_hEvent);	
}

void StockTraderSpi::ReqOrderInsert(TSecurityFtdcInstrumentIDType instId,
	TSecurityFtdcDirectionType dir, TSecurityFtdcCombOffsetFlagType kpp,
	TSecurityFtdcPriceType price,   TSecurityFtdcVolumeType vol, 
	TSecurityFtdcExchangeIDType exchangID)
{
	CSecurityFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));	
	strcpy(req.BrokerID, appId);  //Ӧ�õ�Ԫ����	
	strcpy(req.InvestorID, userId); //Ͷ���ߴ���	
	strcpy(req.ExchangeID, exchangID);//����������
	strcpy(req.InstrumentID, instId); //��Լ����	
	strcpy(req.OrderRef, orderRef);  //��������
	int nextOrderRef = atoi(orderRef);
	sprintf(orderRef, "%d", ++nextOrderRef);

	TSecurityFtdcStockPriceType stockPrice;
	sprintf(stockPrice, "%f", price);
	lstrcpynA(req.LimitPrice, stockPrice, sizeof(req.LimitPrice));	//�۸�

	req.OrderPriceType = SECURITY_FTDC_OPT_LimitPrice;//�۸�����=�޼�	
	req.TimeCondition = SECURITY_FTDC_TC_GFD;  //��Ч������:������Ч
	
	req.Direction = MapDirection(dir,true);  //��������	
	req.CombOffsetFlag[0] = MapOffset(kpp[0],true); //��Ͽ�ƽ��־:����
	req.CombHedgeFlag[0] = SECURITY_FTDC_HF_Speculation;	  //���Ͷ���ױ���־	
	req.VolumeTotalOriginal = vol;	///����		
	req.VolumeCondition = SECURITY_FTDC_VC_AV; //�ɽ�������:�κ�����
	req.MinVolume = 1;	//��С�ɽ���:1	
	req.ContingentCondition = SECURITY_FTDC_CC_Immediately;  //��������:����

	//TThostFtdcPriceType	StopPrice;  //ֹ���
	req.ForceCloseReason =SECURITY_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	req.UserForceClose = 0;   //�û�ǿ����־:��

	int ret = pUserApi->ReqOrderInsert(&req, ++requestId);
	cerr<<" ���� | ���ͱ���..."<<((ret == 0)?"�ɹ�":"ʧ��")<< endl;
}

void StockTraderSpi::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, 
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !IsErrorRspInfo(pRspInfo) && pInputOrder ){
		cerr<<"��Ӧ | �����ύ�ɹ�...��������:"<<pInputOrder->OrderRef<<endl;  
	}
	//if(bIsLast) SetEvent(g_hEvent);	
}

void StockTraderSpi::ReqOrderAction(TSecurityFtdcSequenceNoType orderSeq)
{
	CSecurityFtdcOrderField* pOrder;
	bool found=false; unsigned int i=0;
	for(i=0;i<orderList.size();i++){
		if(orderList[i]->BrokerOrderSeq == orderSeq)
		{ 
			pOrder = orderList[i];
			found = true; 
			break;
		}
	}
	if(!found){cerr<<" ���� | ����������."<<endl; return;} 

	CSecurityFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, appId);   //���͹�˾����	
	strcpy(req.InvestorID, userId); //Ͷ���ߴ���
	strcpy(req.OrderRef, pOrder->OrderRef); //��������	
	req.FrontID = pOrder->FrontID;           //ǰ�ñ��	
	req.SessionID = pOrder->SessionID;       //�Ự���
	strcpy(req.ExchangeID, orderList[i]->ExchangeID);
	strcpy(req.InstrumentID, orderList[i]->InstrumentID);
	req.ActionFlag = SECURITY_FTDC_AF_Delete;  //������־ 

	int ret = pUserApi->ReqOrderAction(&req, ++requestId);
	cerr<< " ���� | ���ͳ���..." <<((ret == 0)?"�ɹ�":"ʧ��") << endl;
}

void StockTraderSpi::OnRspOrderAction(
	CSecurityFtdcInputOrderActionField *pInputOrderAction, 
	CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	if (!IsErrorRspInfo(pRspInfo) && pInputOrderAction){
		cerr<< " ��Ӧ | �����ɹ�..."
			<< "������:"<<pInputOrderAction->ExchangeID
			<<" ��Լ����:"<<pInputOrderAction->InstrumentID<<endl;
	}
	//if(bIsLast) SetEvent(g_hEvent);	
}

///�����ر�
void StockTraderSpi::OnRtnOrder(CSecurityFtdcOrderField *pOrder)
{	
	CSecurityFtdcOrderField* order = new CSecurityFtdcOrderField();
	memcpy(order,  pOrder, sizeof(CSecurityFtdcOrderField));
	bool founded=false;    unsigned int i=0;
	for(i=0; i<orderList.size(); i++){
		if(orderList[i]->BrokerOrderSeq == order->BrokerOrderSeq) {
			founded=true;    break;
		}
	}
	if(founded) orderList[i]= order;   
	else  orderList.push_back(order);
	cerr<<" �ر� | �������ύ...���:"<<order->BrokerOrderSeq
		<<"OrderStatus:"<<order->OrderStatus
		<<"CombHedgeFlag:"<<order->CombHedgeFlag
		<<"CombOffsetFlag:"<<order->CombOffsetFlag
		<<"Direction:"<<order->Direction
		<<"InstrumentID:"<<order->InstrumentID
		<<"LimitPrice:"<<order->LimitPrice
		<<"MinVolume:"<<order->MinVolume
		<<"OrderPriceType:"<<order->OrderPriceType
		<<"StatusMsg:"<<order->StatusMsg
		<<endl;
	
	//SetEvent(g_hEvent);	
}

///�ɽ�֪ͨ
void StockTraderSpi::OnRtnTrade(CSecurityFtdcTradeField *pTrade)
{
	CSecurityFtdcTradeField* trade = new CSecurityFtdcTradeField();
	memcpy(trade,  pTrade, sizeof(CSecurityFtdcTradeField));
	bool founded=false;     unsigned int i=0;
	for(i=0; i<tradeList.size(); i++){
		if(tradeList[i]->TradeID == trade->TradeID) {
			founded=true;   break;
		}
	}
	if(founded) tradeList[i] = trade;   
	else  tradeList.push_back(trade);
	cerr<<" �ر� | �����ѳɽ�...�ɽ����:"<<trade->TradeID<<endl;
	//SetEvent(g_hEvent);
}

void StockTraderSpi::OnFrontDisconnected(int nReason)
{
	cerr<<" ��Ӧ | �����ж�..." 
		<< " reason=" << nReason << endl;
}

void StockTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr<<" ��Ӧ | ������ʱ����..." 
		<< " TimerLapse = " << nTimeLapse << endl;
}

void StockTraderSpi::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}

bool StockTraderSpi::IsErrorRspInfo(CSecurityFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (ret){
		cerr<<" ��Ӧ | "<<pRspInfo->ErrorMsg<<endl;
	}
	return ret;
}

void StockTraderSpi::PrintOrders(){
	CSecurityFtdcOrderField* pOrder; 
	for(unsigned int i=0; i<orderList.size(); i++){
		pOrder = orderList[i];
		cerr<<" ���� | ��Լ:"<<pOrder->InstrumentID
			<<" ����:"<<MapDirection(pOrder->Direction,false)
			<<" ��ƽ:"<<MapOffset(pOrder->CombOffsetFlag[0],false)
			<<" �۸�:"<<pOrder->LimitPrice
			<<" ����:"<<pOrder->VolumeTotalOriginal
			<<" ���:"<<pOrder->BrokerOrderSeq 
			<<" �������:"<<pOrder->OrderSysID
			<<" ״̬:"<<pOrder->StatusMsg<<endl;
	}
	//SetEvent(g_hEvent);
}
void StockTraderSpi::PrintTrades(){
	CSecurityFtdcTradeField* pTrade;
	for(unsigned int i=0; i<tradeList.size(); i++){
		pTrade = tradeList[i];
		cerr<<" �ɽ� | ��Լ:"<< pTrade->InstrumentID 
			<<" ����:"<<MapDirection(pTrade->Direction,false)
			<<" ��ƽ:"<<MapOffset(pTrade->OffsetFlag,false) 
			<<" �۸�:"<<pTrade->Price
			<<" ����:"<<pTrade->Volume
			<<" �������:"<<pTrade->OrderSysID
			<<" �ɽ����:"<<pTrade->TradeID<<endl;
	}
	//SetEvent(g_hEvent);
}
char MapDirection(char src, bool toOrig=true){
	if(toOrig){
		if('b'==src||'B'==src){src='0';}else if('s'==src||'S'==src){src='1';}
	}else{
		if('0'==src){src='B';}else if('1'==src){src='S';}
	}
	return src;
}
char MapOffset(char src, bool toOrig=true){
	if(toOrig){
		if('o'==src||'O'==src){src='0';}
		else if('c'==src||'C'==src){src='1';}
		else if('j'==src||'J'==src){src='3';}
	}else{
		if('0'==src){src='O';}
		else if('1'==src){src='C';}
		else if('3'==src){src='J';}
	}
	return src;
}
