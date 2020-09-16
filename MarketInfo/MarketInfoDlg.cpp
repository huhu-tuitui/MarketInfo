
// MarketInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MarketInfo.h"
#include "MarketInfoDlg.h"
#include "afxdialogex.h"

#include "MdSpi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMarketInfoDlg �Ի���




CMarketInfoDlg::CMarketInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMarketInfoDlg::IDD, pParent), m_pGridCtrl(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMarketInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMarketInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMarketInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMarketInfoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMarketInfoDlg ��Ϣ�������

BOOL CMarketInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new CGridCtrl;
		if (!m_pGridCtrl) return FALSE;

		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
		rect.bottom -= 50;
		m_pGridCtrl->Create(rect, this, 100);

		// fill it up with stuff
		m_pGridCtrl->SetEditable(TRUE);
		m_pGridCtrl->EnableDragAndDrop(TRUE);

		try {
			m_pGridCtrl->SetColumnCount(_countof(LIST_COLUMN_NAME) + 1);
			m_pGridCtrl->SetFixedRowCount(1);
			m_pGridCtrl->SetFixedColumnCount(1);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return FALSE;
		}

		// fill rows/cols with text
		for (int row = 0; row < m_pGridCtrl->GetRowCount(); row++)
			for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = row;
				Item.col = col;
				if (row < 1) {
					// �б���
					Item.nFormat = DT_LEFT|DT_WORDBREAK;
					Item.strText.Format(LIST_COLUMN_NAME[col - 1]);
					m_pGridCtrl->SetColumnWidth(col, 50);
				} else if (col < 1) {
					// �б���
					Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
					Item.strText.Format(_T("%d"),row);
				}
				m_pGridCtrl->SetItem(&Item);
		}

		//m_pGridCtrl->AutoSize();
	}

	//STOCK_DATA data = {_T("test"), 1,2,3,4,5,6,7,8,9,10,11};
	//AddRow2List(&data);
	//AddRow2List(&data);

	InitializeCriticalSection(&Critical);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMarketInfoDlg::AddOrUpdateRow2List(PSTOCK_DATA pData)
{
	if (!m_pGridCtrl)
		return;

	EnterCriticalSection(&Critical);

	int findRow = -1;
	for(int row = 1; row < m_pGridCtrl->GetRowCount() + 1; row++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT | GVIF_FORMAT;
		Item.row = row;
		Item.col = 1;
		if (m_pGridCtrl->GetItem(&Item) && Item.strText.Compare(CString(pData->Name)) == 0)
		{
			findRow = row;
			break;
		}
	}

	if (findRow > -1)
	{
		// ����һ��
		for (int col = 1; col < m_pGridCtrl->GetColumnCount(); col++)
			FillGridCtrl(pData, findRow, col);
	}
	else
	{
		// ���һ��
		m_pGridCtrl->SetRowCount(m_pGridCtrl->GetRowCount() + 1);
		for (int col = 0; col < m_pGridCtrl->GetColumnCount(); col++)
			FillGridCtrl(pData, m_pGridCtrl->GetRowCount() - 1, col);
	}

	LeaveCriticalSection(&Critical);
}

void CMarketInfoDlg::FillGridCtrl(PSTOCK_DATA pData, int row, int col)
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.row = row;
	Item.col = col;

	// ����
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;

	switch(col)
	{
	case 0:
		{
			Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
			Item.strText.Format(_T("%d"), m_pGridCtrl->GetRowCount() - 1);
		}
		break;
	case 1:
		Item.strText.Format(_T("%S"), pData->Name);
		break;
	case 2:
		if(pData->Buy5 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Buy5);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 3:
		if(pData->Buy4 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Buy4);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 4:
		if(pData->Buy3 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Buy3);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 5:
		if(pData->Buy2 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Buy2);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 6:
		if(pData->Buy1 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Buy1);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 7:
		Item.strText.Format(_T("%.1f"), pData->Price);
		break;
	case 8:
		if(pData->Sell1 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Sell1);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 9:
		if(pData->Sell2 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Sell2);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 10:
		if(pData->Sell3 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Sell3);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 11:
		if(pData->Sell4 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Sell4);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	case 12:
		if(pData->Sell5 != 0.0)
		{
			Item.strText.Format(_T("%.1f"), pData->Sell5);
		}
		else
		{
			Item.strText.Format(_T(" "));
		}
		break;
	}

	m_pGridCtrl->SetItem(&Item);
	m_pGridCtrl->Refresh();
	//m_pGridCtrl->AutoSize();
}


void CMarketInfoDlg::SetProgressMsg(CString msg)
{
	// to do
}


void CMarketInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMarketInfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMarketInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMarketInfoDlg::OnBnClickedOk()
{
	DWORD dwThread;
	HANDLE hThread = CreateThread(NULL, 0, CMarketInfoDlg::ThreadCallback, NULL, 0, &dwThread);
	if (NULL == hThread)
        TRACE("�����µ��̳߳���\n");
}

DWORD WINAPI CMarketInfoDlg::ThreadCallback(LPVOID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*char FRONT_ADDR[] = "tcp://180.169.124.109:41213";		// ǰ�õ�ַ
	TThostFtdcBrokerIDType	BROKER_ID = "20000";			// ���͹�˾����

	CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	MdSpi pUserSpi=MdSpi(pUserApi, AfxGetApp()->m_pMainWnd->m_hWnd);
	pUserApi->RegisterSpi(&pUserSpi);						// ע���¼���
	TRACE("��ǰĬ�Ϸ�����Ϊ����--����ʵ�̽��׷���ˣ�%s.\n",FRONT_ADDR);
	TRACE("��ǰĬ��Broker IDΪ��%s\n.", BROKER_ID);
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	
	pUserApi->Init();
	TRACE("Step 1: �ͻ���׼���뽻�׺�̨��������......\n.");
	pUserApi->Join();*/
	//CDialogEx::OnOK();

	return 0;
}

void CMarketInfoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pWnd = AfxGetApp()->GetMainWnd();
	pWnd->UpdateData(true);
}
