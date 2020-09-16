
// LiveUpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LiveUpdate.h"
#include "LiveUpdateDlg.h"
#include "afxdialogex.h"
#include "CommUtil.h"

#include "LogTracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DOWNLOAD_URL	_T("http://futures.tsingfun.com/")
#define SETUP_FILENAME	_T("Setup.exe")

#define DEFAULT_INSTALL_PATH	_T("C:\\MarketInfo")
#define DEFAULT_PROG_NAME		_T("MarketInfo.exe")
#define IDT_WAITFORINSTALL_COMPLETE				1			// �ȴ���װ���Timer
#define IDT_INSTALL_COMPLETE_FAKE				2			// ��װ��ɺ��Զ���ʾαTimer
#define TIMER_INTERVAL_WAITFORINSTALL_COMPLETE	1000
#define TIMER_INTERVAL_INSTALL_COMPLETE_FAKE	5000

// CLiveUpdateDlg �Ի���

CLiveUpdateDlg::CLiveUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLiveUpdateDlg::IDD, pParent), m_hCreatePackage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//::SetEvent(m_hCancel);ֹͣ����
	m_hCancel = ::CreateEvent(NULL, TRUE, FALSE, _T("_CancelEvent_"));
	if (m_hCancel == NULL)
	{
		LOG_ERROR(_T("����cancel��eventʧ�ܣ��˳�"));
		exit(-1);
		return;
	}

	m_pComEngine = CComEngine::GetComEngine();
	if (!m_pComEngine)
	{
		LOG_ERROR(_T("GetComEngineʧ�ܣ��˳�"));
		exit(-1);
		return;
	}
}

void CLiveUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRG_SPLASH, m_PRG);
	DDX_Control(pDX, IDOK, m_STBtnUpgrade);
	DDX_Control(pDX, IDCANCEL, m_STBtnLater);
}

BEGIN_MESSAGE_MAP(CLiveUpdateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLiveUpdateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLiveUpdateDlg::OnBnClickedCancel)

	ON_MESSAGE(WM_PROGRESS_SET_RANGE, OnProgressSetRange)
	ON_MESSAGE(WM_PROGRESS_SET_POS, OnProgressSetPos)
	ON_MESSAGE(WM_PROGRESS_STEP_IT, OnProgressStepIt)
	ON_MESSAGE(WM_PROGRESS_ERROR, OnProgressError)
	ON_MESSAGE(WM_DOWNLOAD_START, OnDownloadStart)
	ON_MESSAGE(WM_DOWNLOAD_END, OnDownloadEnd)
END_MESSAGE_MAP()


// CLiveUpdateDlg ��Ϣ�������

BOOL CLiveUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//Button
	// ------------------------------------------------------
	m_STBtnUpgrade.SetFlat(TRUE);
	m_STBtnUpgrade.DrawBorder(FALSE);
	// ǰ����ɫ
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255));
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
	// ����״̬&Focus
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(66, 131, 222));
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(66, 131, 222));
	// �������
	m_STBtnUpgrade.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(88, 146, 226));

	// ------------------------------------------------------
	m_STBtnLater.SetFlat(TRUE);
	m_STBtnLater.DrawBorder(FALSE);
	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255));
	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));

	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(148, 148, 148));
	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(148, 148, 148));

	m_STBtnLater.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(128, 128, 128));


	// ------------------------------------------------------
	m_PRG.SetGradientColors(RGB(111, 204, 159), RGB(111, 204, 159));
	m_PRG.SetBkColor(RGB(247, 243, 247));
	m_PRG.SetBorderColor(RGB(111, 204, 159), RGB(247, 243, 247));
	m_PRG.SetReverse(FALSE);

	// ------------------------------------------------------
	// �����ز��ֿؼ�
	GetDlgItem(IDC_SPLASH_INFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
	m_PRG.ShowWindow(SW_HIDE);

	// �汾����
	CString strTmp;
	strTmp.Format(_T("��������汾Ϊ��%s"), CLiveUpdateApp::GetLatestVersion());
	SetDlgItemText(IDC_STATIC_VERSION1, strTmp);

	strTmp.Format(_T("��ǰ����汾Ϊ��%s"), CLiveUpdateApp::GetCurrentVersion());
	SetDlgItemText(IDC_STATIC_VERSION2, strTmp);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLiveUpdateDlg::OnPaint()
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
HCURSOR CLiveUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��������
void CLiveUpdateDlg::OnBnClickedOk()
{
	LOG_TRACER();

	if (m_pComEngine->IsDownloading())
		return;

	GetDlgItem(IDC_STATIC_VERSION1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_VERSION2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_SPLASH_INFO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
	m_PRG.ShowWindow(SW_SHOW);
	m_STBtnLater.SetWindowText(_T("ȡ������(&C)"));

	LOG_INFO(_T("��ʼ�������߰�װ��..."));

	// ���ڿ�ʼ���߸��£����Ժ󡭡�
	UpdateMessage(_T("��ʼ�������߰�װ��..."));

	LOG_INFO(_T("��ʼ����%s"), SETUP_FILENAME);
	TCHAR szPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);

	TCHAR szFile[_MAX_PATH] = {0};
	_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), szPath, SETUP_FILENAME);
	// ��������ļ����ڣ���ɾ��
	if (-1 != _taccess(szFile,NULL))
		::DeleteFile(szFile);

	CString strErrMsg;
	unsigned long nFileSize = 0;
	unsigned int nUrlListLen = 0;
	TCHAR szServerIniUrl[_MAX_PATH] = {0};
	TCHAR szServerIniDir[1] = {0};
	BYTE bZipList[1] = {0};
	_stprintf_s(szServerIniUrl, _MAX_PATH, _T("%s/%s"), DOWNLOAD_URL, SETUP_FILENAME);
	nUrlListLen = lstrlen(szServerIniUrl) + 1;

	LOG_INFO(_T("m_pComEngine->BeginDownloadThread"));
	LOG_INFO(_T("szServerIniUrl = %s"), szServerIniUrl);
	//�������첽��ʽ����
	m_PRG.SetTextFormat(_T("%d%%"),PBS_SHOW_PERCENT);
	if (!m_pComEngine->BeginDownloadThread(GetSafeHwnd(), 1, 
		szServerIniUrl, nUrlListLen,
		szServerIniDir, 1, 
		bZipList, 1, 
		m_hCancel, szPath, NULL))
	{
		LOG_ERROR(_T("m_pComEngine->BeginDownloadThreadʧ��"));
		//�������
		OutputDebugString(_T("���������߳�ʧ�ܡ�"));
	}
}

// �Ժ�����
void CLiveUpdateDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CLiveUpdateDlg::UpdateMessage(LPCTSTR lpszMessage, DWORD dwMilliseconds/*=0*/)
{
	LOG_TRACER();
	SetDlgItemText(IDC_SPLASH_INFO, lpszMessage);

	//if (dwMilliseconds > 0 && m_eOperationMode != MODE_HIDE)
	if (dwMilliseconds > 0)
	{
		// wait a moment for reading time
		::Sleep(dwMilliseconds);
	}
	OutputDebugString(lpszMessage);
	OutputDebugString(_T("\n"));
}

LRESULT CLiveUpdateDlg::OnProgressSetRange(WPARAM wParam, LPARAM lParam)
{
	LOG_TRACER();
	int nLower = 0;
	int nUpper = (DWORD)wParam;
	m_PRG.SetRange32(nLower, nUpper);

	CString strFile,strMsg;
	strFile = (LPCTSTR)lParam;
	strFile = strFile.Mid(strFile.ReverseFind('\\')+1);
	LOG_INFO(_T("strFile = %s"), strFile);
	m_PRG.SetTextFormat(_T("%d%%"), PBS_SHOW_PERCENT);

	CString strMessage = _T("���ذ�װ�������Ժ�...");
	// �ӷ�����������Ҫ���µ��ļ�[%s]����
	UpdateMessage(strMessage);
	LOG_INFO(strMessage);
	
	return 0;
}

LRESULT CLiveUpdateDlg::OnProgressSetPos(WPARAM wParam, LPARAM lParam)
{
	if (!m_pComEngine->IsDownloading())
	{
		return S_OK;
	}
	unsigned long nPos = (unsigned long)wParam;
	int nLower, nUpper;
	m_PRG.SetPos(nPos);
	m_PRG.GetRange(nLower, nUpper);
	CString strMsg;

	if (nUpper - nLower <= 0)
	{
		return 0;
	}

	// ---��������---
	float total = (float)(nUpper - nLower) / (1024 * 1024);
	float curr = (float)nPos / (1024 * 1024);
	strMsg.Format(_T("�����أ� %.2f M / %.2f M"), curr, total);

	CString strLastMsg;
	GetDlgItemText(IDC_STATIC_SPEED, strLastMsg);
	if (strLastMsg.Compare(strMsg) != 0)
		SetDlgItemText(IDC_STATIC_SPEED, strMsg);

	if (nPos < (unsigned long)(nUpper - nLower))
	{
	}
	else
	{
		m_PRG.SetTextFormat(_T(""), PBS_SHOW_PERCENT);
	}

	return 0;
}

LRESULT CLiveUpdateDlg::OnProgressStepIt(WPARAM wParam, LPARAM lParam)
{
	LOG_TRACER();
	CString strMessage, strFileName;
	int nFileCount = (int)lParam;

	m_PRG.StepIt();

	return 0;
}


LRESULT CLiveUpdateDlg::OnProgressError(WPARAM wParam, LPARAM lParam)
{
	LOG_TRACER();
	DWORD dwError = (DWORD)wParam;
	CString strMessage,strFile;
	strFile = (LPCTSTR)lParam;
	strFile = strFile.Mid(strFile.ReverseFind('\\')+1);
	// �ӷ�����������Ҫ���µ��ļ�[%s]�Ĺ����з�������������ֹ!
	strMessage = _T("���ظ����ļ����ִ��󣬸�����ֹ");
	UpdateMessage(strMessage, 1000);
	LOG_INFO(_T("strFile = %s"), strFile);
	LOG_INFO(strMessage);

	return 0;
}

LRESULT CLiveUpdateDlg::OnDownloadStart( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

LRESULT CLiveUpdateDlg::OnDownloadEnd( WPARAM wParam, LPARAM lParam )
{
	UpdateMessage(_T("��װ��������ɡ���ʼ��װ..."), 1000);

	TCHAR szCurPath[_MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurPath);

	TCHAR szFile[_MAX_PATH] = {0};
	_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), szCurPath, SETUP_FILENAME);

	CString szPath = szFile;
	CString szCmdline = _T("");
	CString szWorking;
	szWorking = szPath.Mid( 0, szPath.ReverseFind( '\\' ) );
	/*PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset( &si, 0, sizeof( si ) );
	si.cb = sizeof( si );
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;*/


	// Inno Pack & NSIS ��װ���ȿɿ�
	// Inno Pack
	//szCmdline = _T("/SP- /SILENT /NORESTART /DIR=\"");
	//szCmdline.Append( DEFAULT_INSTALL_PATH );
	//szCmdline.Append( _T("\"") );
	// NSIS
	szCmdline = _T("/NCRC /S /D=\"");
	szCmdline.Append( DEFAULT_INSTALL_PATH );
	szCmdline.Append( _T("\"") );

	//BOOL fRet = CreateProcess( szPath, szCmdline.GetBuffer(), NULL, FALSE, NULL, NULL, NULL, szWorking, &si, &pi );
	
	// ------����Ȩ��------
	// Initialize the structure.
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	// Ask for privileges elevation.
	sei.lpVerb = TEXT("runas");

	// Create a Command Prompt from which you will be able to start
	// other elevated applications.
	sei.lpFile = szFile;
	sei.lpParameters = szCmdline;
	sei.lpDirectory = szWorking;

	// Don't forget this parameter; otherwise, the window will be hidden.
	sei.nShow = SW_SHOWNORMAL;

	if (!ShellExecuteEx(&sei)) {
		DWORD dwStatus = GetLastError();

		if (dwStatus == ERROR_CANCELLED) {
			// The user refused to allow privileges elevation.
			UpdateMessage(_T("�û��ܾ���װ����װʧ�ܡ�"));
		}
		else if (dwStatus == ERROR_FILE_NOT_FOUND) {
			// The file defined by lpFile was not found and
			// an error message popped up.
			UpdateMessage(_T("��װ�������ڣ����飡"));
		}

		CString strMsg;
		::GetLastErrorString(strMsg);
		LOG_ERROR(_T("������װ����ʧ�ܣ�%s"), strMsg);

		return -1;
	}

	m_hCreatePackage = sei.hProcess;		// ���
	// �ȴ���װ���
	SetTimer( IDT_WAITFORINSTALL_COMPLETE, TIMER_INTERVAL_WAITFORINSTALL_COMPLETE, NULL );

	return 0;
}

void CLiveUpdateDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch ( nIDEvent )
	{
	case IDT_WAITFORINSTALL_COMPLETE:
		{
			DWORD dw = 0;
			if ( m_hCreatePackage )
			{
				dw = WaitForSingleObject( m_hCreatePackage, 0 );
			}
			switch ( dw )
			{
			case WAIT_OBJECT_0:
				{
					KillTimer( IDT_WAITFORINSTALL_COMPLETE );

					// ���򷵻�code
					DWORD dwExitCode;
					GetExitCodeProcess(m_hCreatePackage, &dwExitCode);

					m_STBtnLater.SetWindowText(_T("�˳�(&Q)"));
					if (dwExitCode == 0)
					{
						UpdateMessage( _T("���°汾�Ѱ�װ��ɡ�5 ���رնԻ�����������...") );						
						SetTimer( IDT_INSTALL_COMPLETE_FAKE, TIMER_INTERVAL_INSTALL_COMPLETE_FAKE, NULL );
					}
					else if (dwExitCode == 2)
					{
						UpdateMessage( _T("��װʧ�ܣ�������ȡ����װ��") );
					}
					else
					{
						CString errMsg;
						errMsg.Format(_T("��װʧ�ܣ���δ֪����:%d��"), dwExitCode);
						UpdateMessage(errMsg);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case IDT_INSTALL_COMPLETE_FAKE:
		{
			KillTimer( IDT_INSTALL_COMPLETE_FAKE );

			// ��װ�����������
			TCHAR szFile[_MAX_PATH] = {0};
			_stprintf_s(szFile, _MAX_PATH, _T("%s\\%s"), DEFAULT_INSTALL_PATH, DEFAULT_PROG_NAME);

			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			memset( &si, 0, sizeof( si ) );
			si.cb = sizeof( si );
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW;

			BOOL fRet = CreateProcess( szFile, NULL, NULL, FALSE, NULL, NULL, NULL, DEFAULT_INSTALL_PATH, &si, &pi );

			if (fRet)
			{
				OnOK();
			}
			else
			{
				UpdateMessage( _T("��Ǹ����������ʧ�ܣ����˳����ֶ���������") );
			}
		}
		break;

	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}