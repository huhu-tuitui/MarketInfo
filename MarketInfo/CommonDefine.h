#ifndef __COMMON_DEFINE_H_
#define __COMMON_DEFINE_H_

// Ĭ��չʾ�ڻ�����
#define DEFAULT_CODE	"IF1503"

// ǰ�õ�ַ
#define FRONT_ADDR		"tcp://180.169.124.109:41213"
// ���͹�˾����
#define BROKER_ID		"20000"

// caption_logo width
#define CAPTION_LOGO_WIDTH 90

// ����
const TCHAR LIST_COLUMN_NAME[][10] = {_T("����"), _T("����"), _T("����"), _T("����"), _T("���"), _T("��һ"), 
									  _T("�ּ�"), _T("��һ"), _T("����"), _T("����"), _T("����"), _T("����")};

typedef struct _STOCK_DATA {
	// ����
	TThostFtdcInstrumentIDType Name;
	// �ּ�
	TThostFtdcPriceType Price;
	// ��һ~��
	TThostFtdcPriceType Buy1;
	TThostFtdcPriceType Buy2;
	TThostFtdcPriceType Buy3;
	TThostFtdcPriceType Buy4;
	TThostFtdcPriceType Buy5;
	// ��һ~��
	TThostFtdcPriceType Sell1;
	TThostFtdcPriceType Sell2;
	TThostFtdcPriceType Sell3;
	TThostFtdcPriceType Sell4;
	TThostFtdcPriceType Sell5;
} STOCK_DATA, *PSTOCK_DATA;


// ��ɫ������
#define COLOR_TOOLBAR_BG	RGB(45, 50, 55)
#define COLOR_BLACK_BG		RGB(0, 0, 0)
#define COLOR_GRAY_BORDER	RGB(60, 60, 60)
#define COLOR_SEP_QUOTE		RGB(120, 120, 120)
#define COLOR_TITLE_BG		RGB(25, 25, 25)

// ������ɫ
#define COLOR_YELLOW_TEXT	RGB(255, 255, 0)
#define COLOR_GREEN_TEXT	RGB(82, 255, 82)
#define COLOR_RED_TEXT		RGB(255, 82, 82)
#define COLOR_WHITE_TEXT	RGB(192, 192, 192)
#define COLOR_CYAN_TEXT		RGB(82, 255, 255)

// ���̱��Ի�����ɫ
#define COLOR_JPB_BORDER	RGB(227, 227, 227)
#define COLOR_JPB_IN_BORDER	RGB(240, 240, 240)
#define COLOR_JPB_FILL		RGB(255, 255, 255)
#define COLOR_JPB_MARKET	RGB(107, 107, 107)
#define COLOR_JPB_NAME		RGB(0, 0, 0)

// ������ʾ���䱳��ɫ
// ��ɫ���
//#define COLOR_SPLASH_4		RGB(24, 35, 69)
//#define COLOR_SPLASH_3		RGB(18, 26, 47)
//#define COLOR_SPLASH_2		RGB(12, 17, 35)
//#define COLOR_SPLASH_1		RGB(0, 0, 0)

// �Ե������
#define COLOR_SPLASH_4		RGB(0, 0, 106)
#define COLOR_SPLASH_3		RGB(0, 0, 74)
#define COLOR_SPLASH_2		RGB(0, 0, 45)
#define COLOR_SPLASH_1		RGB(0, 0, 0)

// ��ɫ���
//#define COLOR_SPLASH_4		RGB(0, 0, 156)
//#define COLOR_SPLASH_3		RGB(0, 0, 106)
//#define COLOR_SPLASH_2		RGB(0, 0, 74)
//#define COLOR_SPLASH_1		RGB(0, 0, 0)




// ��¼����λ��
#define SECTION_DROPLOCATION        _T("DropLocation")
#define INI_KEY_MAIN_X              _T("MainX")
#define INI_KEY_MAIN_Y              _T("MainY")

#endif __COMMON_DEFINE_H_