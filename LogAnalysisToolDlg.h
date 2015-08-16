// LogAnalysisToolDlg.h : header file
//
#include <vector>
#include <map>
#include "Func.h"
using namespace std;

#if !defined(AFX_LOGANALYSISTOOLDLG_H__B89DBC28_A6F4_4008_A69E_C58A754CD408__INCLUDED_)
#define AFX_LOGANALYSISTOOLDLG_H__B89DBC28_A6F4_4008_A69E_C58A754CD408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma warning(disable:4786)
#define     ID_TIMER 1000    //定义宏名来标识定时器
#define     ProcessMaxValue 100 //进度条最大值
#define  BIF_NEWDIALOGSTYLE  0x0040  
/////////////////////////////////////////////////////////////////////////////
// CLogAnalysisToolDlg dialog

typedef struct	_NOTESDEF
{
	char	szCur[4];
	int		iDeno;
	int		iCounter;
	_NOTESDEF()
	{
		memset( szCur, 0, sizeof(szCur) );
		iDeno = 0;
		iCounter = 0;
	}
} _NotesDef;

//将此变量声明在窗口类中会出现错误，但其他工程中不会出现错误（可能工程设置错误，与CLogAnalysisToolDlg dlg; m_pMainWnd = &dlg;有关）
// vector<CString>     m_vecFilename;  //用CString数组存放文件的路径
// map<COleDateTime,CString> m_mapFileDateName;
// vector<CString>     m_vecGapRightFilename;//时间间隔右侧的文件名
// vector<CString>     m_vecGapLeftFilename;//时间间隔左侧的文件名
// CString				m_strSumPrompt;

class CLogAnalysisToolDlg : public CDialog
{
// Construction
public:
	CLogAnalysisToolDlg(CWnd* pParent = NULL);	// standard constructor
	
	vector<CString>     m_vecFilename;  //用CString数组存放文件的路径
	map<COleDateTime,CString> m_mapFileDateName;
	vector<CString>     m_vecGapRightFilename;//时间间隔右侧的文件名
	vector<CString>     m_vecGapLeftFilename;//时间间隔左侧的文件名
    CString				m_strSumPrompt;

	//初始化及复位
	void                listBoxInitial();//初始化列表框
	void				reset();//复位
	char		        m_szConfigFileName[256];//配置文件路径

	//选择文件
	int					GetLogDate( CString l_strFileName, COleDateTime& l_EndTime);
	bool                m_bFileSelect;//选择文件的标志变量
	void				GetFileNameFromFolder( CString strFolderPath);
	bool                m_bMoreMachineAnalysis;//多机分析

	//控件指针
	CListCtrl*          m_hWndListCtrol1;
	CListCtrl*			m_hWndListCtrol2;
	CListCtrl*			m_hWndListCtrol3;
	CListCtrl*			m_hWndListCtrol4;
	CListCtrl*			m_hWndListCtrol6;
	CProgressCtrl*      m_hWndProgressCtrl1; 


	//交易分析相关函数
	int	                GetOccurrences( char* l_szFileName, char* l_szKeyWord );//指定文件中搜索关键字频率
	void				TranscationAnalysisFun();//交易分析函数
	int					WriteSearchConfiFile();//根据选定的日志文件得到钱箱ID，币值，自动配置搜索文件
	bool                m_bFinishProcessing;

    //note分析相关函数
	CFunc				m_oFunc;//全局辅助函数类
	bool				m_bHiddenResultWhenZero;//隐藏为0的统计项
	void				StatCashDispensed();//统计配钞信息
	void				InsertDataToNotesList( char *szTitle, char *szDeno, char *szTimes, char *szCounter, char *szAmount );//插入钞票列表信息记录
	int					GetCounter( char *szFileName, char *szKeyWord, int iPacer, int *Times );//得到钞票次数及张数
	int					m_iListViewIndex;//钞票列表的全局索引
	void				StatCashRejected();//统计拒钞钞票
	void				StatCashACDeposited();//统计AC存款张数
	int					GetACDepositDetail( char* l_szFileName, char* l_szKeyWord );//得到AC存款次数
	void				ParseNoteInfo( char *szLogsMessage );//将数据放到结构体 
	void				AddToList( _NotesDef NotesDetail );//将钞票结构体添加到列表
	long		m_iACDepositedAmount;
	int					m_iACDepositedCounter;
	int					m_iACDepositedTimes;
	_NotesDef			m_tNotesInfo[99];//AC存款各币种信息
	void				StatCashDeposited();//得到存款信息

    //模式统计
	void				StatMode();//ATM模式统计
	int					GetLineTime( char* l_pstr, COleDateTime& CurTime);//得到当前记录时间
	int                 GetModeOccurrencesTime( char* l_szFileName, char* l_szKeyWord, int l_bFirstFile, int l_bFinalFile);//得到各模式频率及时间
	COleDateTime		m_LastFileSearchBeginTime;//上个文件最后一次搜索开始时间
	bool                m_bFindKeyWord;//找到模式关键字
	COleDateTimeSpan    m_sumTimeSpan;//模式时间
	int                 m_bModeStat;//是否进行模式分析

	int					GetCassetteState();

	//进度条控制
	int                 progressNumber;//当前进度
	bool                m_bTextFlash;

	//导出Excel
	CString            GetExcelDriver();
	BOOL		       MakeSurePathExists( CString &Path,
						bool FilenameIncluded);
	BOOL		       GetDefaultXlsFileName(CString& sExcelFile);
    void		       ExportListToExcel(CListCtrl* pList, CString strTitle);
  

// Dialog Data
	//{{AFX_DATA(CLogAnalysisToolDlg)
	enum { IDD = IDD_LOGANALYSISTOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogAnalysisToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLogAnalysisToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOpenFile();
	afx_msg void OnBtnTransactionAnalysis();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnExcelExport();
	afx_msg void OnBtnSelectFolder();
	afx_msg void OnBtnSearch();
	afx_msg void OnDblclkList6(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGANALYSISTOOLDLG_H__B89DBC28_A6F4_4008_A69E_C58A754CD408__INCLUDED_)
