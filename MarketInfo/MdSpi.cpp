#include "StdAfx.h"
#include "MdSpi.h"
#include<afx.h>
#include"resource.h"
#include "afxdialogex.h"
#include "MainDlg.h"

using namespace std;
ostream& operator << (ostream& out, string& str)
{
	return out<<str.c_str();
}; 

//char FRONT_ADDR[] = "tcp://180.169.124.109:41213";		// ǰ�õ�ַ
//TThostFtdcBrokerIDType	BROKER_ID = "20000";			// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����
//char *ppInstrumentID[] = {"cu1501", "cu1503", "a1505", "ag1503","al1506"};		// ���鶩���б�
//char *ppInstrumentID[] = {"cu1503"};
//int iInstrumentID = _countof(ppInstrumentID);									// ���鶩������
int iRequestID = 0;


MdSpi::MdSpi(CThostFtdcMdApi *pUserApi, HWND hWnd) : m_pUserApi(pUserApi)
{
	m_hWnd = hWnd;
}

void MdSpi::BeginConnect()
{

}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void MdSpi::OnFrontConnected()
{
	TRACE("Step 1 Success: �ͻ����뽻�׺�̨���ӳɹ���\n");
	TRACE("$ ��Ϣ��Դ�� OnFrontConnected\n"); 
	TRACE("----------------------------------------------------------------------------\n");
	///�û���¼����
	TRACE("Step 2: �û������½......\n");
	ReqUserLogin();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
void MdSpi::OnFrontDisconnected(int nReason)
	{
		TRACE("$ ��Ϣ��Դ�� OnFrontDisconnected\n");
		TRACE("$ Reason = %d\n", nReason);
		TRACE("�ͻ����뽻�׺�̨���ӶϿ���\n");
		TRACE("==============================================================================\n");
	}

///����Ӧ��
void MdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE("$ ������Ϣ��Դ�� OnRspError\n");
	IsErrorRspInfo(pRspInfo);
}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void MdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	TRACE("$ ������Ϣ��Դ�� OnHeartBeatWarning\n");
	TRACE("$ nTimerLapse = %d", nTimeLapse);
}

///��¼������Ӧ
void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		TRACE("Step 2 Success: �û������½�ɹ���\n");
	}
	else
	{
		TRACE("Step 2 Failed: �û������½ʧ�ܣ�\n");
	}
	TRACE("$��Ϣ��Դ: OnRspUserLogin\n");
	TRACE("----------------------------------------------------------------------------\n");

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		TRACE("Step 3: ��ȡ��ǰ������......\n");
		TRACE("$ ��ȡ��ǰ������ = %s\n", m_pUserApi->GetTradingDay());
		TRACE("Step 3 Success: ��ȡ��ǰ�����ճɹ�\n");
		TRACE("----------------------------------------------------------------------------\n");

		TRACE("Step 4: �û����������飬���߻�������ͼ��\n");
		TRACE("\tdata: ���������飻\n");
		TRACE("\tdraw: ��������ͼ��\n");

		// ����������
		TRACE("�û�����������......\n");
		SubscribeMarketData();
	}
}

///��������Ӧ��
void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!pRspInfo->ErrorID)
	{
		TRACE("Step 4 Success: �û�����������ɹ���\n");
	}
	else
	{
		TRACE("Step 4 Failed: �û�����������ʧ�ܣ�\n");
	}
	TRACE("$��Ϣ��Դ: %s\n.", __FUNCTION__);
	TRACE("----------------------------------------------------------------------------\n");

	////Ū����������ݵľ���ṹ
	//string path=pSpecificInstrument->InstrumentID;
	//path.append("_").append(m_pUserApi->GetTradingDay()).append(".txt");
	//fstream out(path,ios::app);
	//out<<"��Լ����,\t��һ��,\t��һ��,\t��һ��,\t��һ��,\t������,\t��߼�,\t��ͼ�,\t���¼�,\t������,\t����޸�ʱ��"<<endl;
	//out.close();
}

///ȡ����������Ӧ��
void MdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	TRACE("$��Ϣ��Դ: %s\n.", __FUNCTION__);
}

///�������֪ͨ
void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	TRACE("$�������: %s\n.", __FUNCTION__);
	//SetEvent(g_hEvent);

	// ���ݷַ�
	CMainDlg *dlg = dynamic_cast<CMainDlg *>(AfxGetApp()->GetMainWnd());
	if (dlg)
	{
		dlg->m_pZhubiContainer->OnRecvData(pDepthMarketData);
		dlg->m_pFenshiContainer->OnRecvData(pDepthMarketData);
		dlg->m_pQuoteContainer->OnRecvData(pDepthMarketData);
		dlg->m_pOverviewContainer->OnRecvData(pDepthMarketData);
		dlg->m_pTabNavContainer->OnRecvData(pDepthMarketData);
	}

	/*CThostFtdcDepthMarketDataField dtField;
	memset(&dtField,0,sizeof(dtField));
	dtField = *pDepthMarketData;

	TThostFtdcPriceType openPrice = dtField.OpenPrice;
	TThostFtdcPriceType maxPrize = openPrice * 1.1;
	TThostFtdcPriceType minPrize = openPrice * 0.9;
	
	STOCK_DATA data;
	memcpy(data.Name, dtField.InstrumentID, 30);
	data.Price = dtField.LastPrice;
	data.Buy1 =	dtField.BidPrice1;
	data.Buy2 = dtField.BidPrice2;
	data.Buy3 = dtField.BidPrice3;
	data.Buy4 = dtField.BidPrice4;
	data.Buy5 = dtField.BidPrice5;
	data.Sell1 = dtField.AskPrice1;
	data.Sell2 = dtField.AskPrice2;
	data.Sell3 = dtField.AskPrice3;
	data.Sell4 = dtField.AskPrice4;
	data.Sell5 = dtField.AskPrice5;
	if(data.Buy1 < minPrize || data.Buy1 > maxPrize)
	{
		data.Buy1 =	0;
	}
	if(data.Buy2 < minPrize || data.Buy2 > maxPrize)
	{
		data.Buy2 =	0;
	}
	if(data.Buy3 < minPrize || data.Buy3 > maxPrize)
	{
		data.Buy3 =	0;
	}
	if(data.Buy4 < minPrize || data.Buy4 > maxPrize)
	{
		data.Buy4 =	0;
	}
	if(data.Buy5 < minPrize || data.Buy5 > maxPrize)
	{
		data.Buy5 =	0;
	}
	if(data.Sell1 < minPrize || data.Sell1 > maxPrize)
	{
		data.Sell1 = 0;
	}
	if(data.Sell2 < minPrize || data.Sell2 > maxPrize)
	{
		data.Sell2 = 0;
	}
	if(data.Sell3 < minPrize || data.Sell3 > maxPrize)
	{
		data.Sell3 = 0;
	}
	if(data.Sell4 < minPrize || data.Sell4 > maxPrize)
	{
		data.Sell4 = 0;
	}
	if(data.Sell5 < minPrize || data.Sell5 > maxPrize)
	{
		data.Sell5 = 0;
	}

	CMarketInfoDlg *dlg = dynamic_cast<CMarketInfoDlg *>(AfxGetApp()->GetMainWnd());
	dlg->AddOrUpdateRow2List(&data);*/

	//string path="";
	//path.append(pDepthMarketData->InstrumentID).append("_").append(pDepthMarketData->TradingDay).append(".txt");
	//fstream out;
	//out.open(path,ofstream::app);
	//out<<"��Լ����,\t��һ��\t��һ��\t��һ��\t��һ��\t������\t��߼�\t��ͼ�\t���¼�"<<endl;
	//out<<dtField.InstrumentID<<",\t";
	//out<<dtField.BidPrice1<<",\t"<<dtField.BidVolume1<<",\t";
	//out<<dtField.AskPrice1<<",\t"<<dtField.AskVolume1<<",\t";
	//out<<dtField.Volume<<",\t";
	//out<<dtField.HighestPrice<<","<<dtField.LowestPrice<<",\t"<<dtField.LastPrice<<",\t";
	//out<<dtField.TradingDay<<",\t"<<dtField.UpdateTime<<endl;
	//out.close();
	//cout<<dtField.UpdateMillisec<<endl;

	//string key=dtField.InstrumentID;
	//stringstream ss;
	//ss<<key<<"\t"<<dtField.AskPrice5<<"\t"<<dtField.AskPrice4<<"\t"<<dtField.AskPrice3<<"\t"<<dtField.AskPrice2<<"\t"<<dtField.AskPrice1<<"\t";
	//ss<<dtField.LastPrice<<"\t";
	//ss<<dtField.AskPrice1<<"\t"<<dtField.AskPrice2<<"\t"<<dtField.AskPrice3<<"\t"<<dtField.AskPrice4<<"\t"<<dtField.AskPrice5<<"\r\n";
	//string str1;
	//ss>>str1;
	//ss.clear();
	//
	//ss<<"\t"<<dtField.AskVolume5<<"\t"<<dtField.AskVolume4<<"\t"<<dtField.AskVolume3<<"\t"<<dtField.AskVolume2<<"\t"<<dtField.AskVolume1<<"\t";
	//ss<<dtField.Volume<<"\t";
	//ss<<dtField.BidVolume1<<"\t"<<dtField.BidVolume2<<"\t"<<dtField.BidVolume3<<"\t"<<dtField.BidVolume4<<"\t"<<dtField.BidVolume5<<"\r\n";
	//string str2;
	//ss>>str2;
	//ss.clear();

	//CWnd* pWnd = AfxGetApp()->GetMainWnd();
	//pWnd->SetDlgItemText(IDC_EDIT1 ,(CString)str1.c_str());
	//pWnd->SetDlgItemText(IDC_EDIT2 ,(CString)str2.c_str());
	////CMarketInfoDlg* pWnd = (CMarketInfoDlg*)CWnd::FromHandle(m_hWnd);
	//if(key.compare(ppInstrumentID[0])==0)
	//{
	//	pWnd->GetDlgItem(IDC_EDIT1)->SetWindowText((CString)str1.c_str());  
	//	pWnd->GetDlgItem(IDC_EDIT2)->SetWindowText((CString)str2.c_str());  
	//	pWnd->UpdateData(false);
	//}
	////to do
	//
	////UpdateData(FALSE);
}

void MdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, BROKER_ID);
	strcpy_s(req.UserID, INVESTOR_ID);
	strcpy_s(req.Password, PASSWORD);
	int iResult = m_pUserApi->ReqUserLogin(&req, ++iRequestID);
	TRACE("$ �����û���¼����: %s\n.",((iResult == 0) ? "�ɹ�" : "ʧ��"));
}

void MdSpi::SubscribeMarketData()
{
	int iResult = m_pUserApi->SubscribeMarketData(m_ppInstrumentID, m_nInstrumentIDNum);
	TRACE("$ �������鶩������: \n.", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
}

void MdSpi::UnSubscribeMarketData()
{
	int iResult = m_pUserApi->UnSubscribeMarketData(m_ppInstrumentID, m_nInstrumentIDNum);
	TRACE("$ ����ȡ�����鶩������: \n.", ((iResult == 0) ? "�ɹ�" : "ʧ��"));
}
	 
bool MdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		TRACE("$ ErrorID= %d , ErrorMsg= %s\n.", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}
