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
#define     ID_TIMER 1000    //�����������ʶ��ʱ��
#define     ProcessMaxValue 100 //���������ֵ
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

//���˱��������ڴ������л���ִ��󣬵����������в�����ִ��󣨿��ܹ������ô�����CLogAnalysisToolDlg dlg; m_pMainWnd = &dlg;�йأ�
// vector<CString>     m_vecFilename;  //��CString�������ļ���·��
// map<COleDateTime,CString> m_mapFileDateName;
// vector<CString>     m_vecGapRightFilename;//ʱ�����Ҳ���ļ���
// vector<CString>     m_vecGapLeftFilename;//ʱ���������ļ���
// CString				m_strSumPrompt;

class CLogAnalysisToolDlg : public CDialog
{
// Construction
public:
	CLogAnalysisToolDlg(CWnd* pParent = NULL);	// standard constructor
	
	vector<CString>     m_vecFilename;  //��CString�������ļ���·��
	map<COleDateTime,CString> m_mapFileDateName;
	vector<CString>     m_vecGapRightFilename;//ʱ�����Ҳ���ļ���
	vector<CString>     m_vecGapLeftFilename;//ʱ���������ļ���
    CString				m_strSumPrompt;

	//��ʼ������λ
	void                listBoxInitial();//��ʼ���б��
	void				reset();//��λ
	char		        m_szConfigFileName[256];//�����ļ�·��

	//ѡ���ļ�
	int					GetLogDate( CString l_strFileName, COleDateTime& l_EndTime);
	bool                m_bFileSelect;//ѡ���ļ��ı�־����
	void				GetFileNameFromFolder( CString strFolderPath);
	bool                m_bMoreMachineAnalysis;//�������

	//�ؼ�ָ��
	CListCtrl*          m_hWndListCtrol1;
	CListCtrl*			m_hWndListCtrol2;
	CListCtrl*			m_hWndListCtrol3;
	CListCtrl*			m_hWndListCtrol4;
	CListCtrl*			m_hWndListCtrol6;
	CProgressCtrl*      m_hWndProgressCtrl1; 


	//���׷�����غ���
	int	                GetOccurrences( char* l_szFileName, char* l_szKeyWord );//ָ���ļ��������ؼ���Ƶ��
	void				TranscationAnalysisFun();//���׷�������
	int					WriteSearchConfiFile();//����ѡ������־�ļ��õ�Ǯ��ID����ֵ���Զ����������ļ�
	bool                m_bFinishProcessing;

    //note������غ���
	CFunc				m_oFunc;//ȫ�ָ���������
	bool				m_bHiddenResultWhenZero;//����Ϊ0��ͳ����
	void				StatCashDispensed();//ͳ���䳮��Ϣ
	void				InsertDataToNotesList( char *szTitle, char *szDeno, char *szTimes, char *szCounter, char *szAmount );//���볮Ʊ�б���Ϣ��¼
	int					GetCounter( char *szFileName, char *szKeyWord, int iPacer, int *Times );//�õ���Ʊ����������
	int					m_iListViewIndex;//��Ʊ�б��ȫ������
	void				StatCashRejected();//ͳ�ƾܳ���Ʊ
	void				StatCashACDeposited();//ͳ��AC�������
	int					GetACDepositDetail( char* l_szFileName, char* l_szKeyWord );//�õ�AC������
	void				ParseNoteInfo( char *szLogsMessage );//�����ݷŵ��ṹ�� 
	void				AddToList( _NotesDef NotesDetail );//����Ʊ�ṹ����ӵ��б�
	long		m_iACDepositedAmount;
	int					m_iACDepositedCounter;
	int					m_iACDepositedTimes;
	_NotesDef			m_tNotesInfo[99];//AC����������Ϣ
	void				StatCashDeposited();//�õ������Ϣ

    //ģʽͳ��
	void				StatMode();//ATMģʽͳ��
	int					GetLineTime( char* l_pstr, COleDateTime& CurTime);//�õ���ǰ��¼ʱ��
	int                 GetModeOccurrencesTime( char* l_szFileName, char* l_szKeyWord, int l_bFirstFile, int l_bFinalFile);//�õ���ģʽƵ�ʼ�ʱ��
	COleDateTime		m_LastFileSearchBeginTime;//�ϸ��ļ����һ��������ʼʱ��
	bool                m_bFindKeyWord;//�ҵ�ģʽ�ؼ���
	COleDateTimeSpan    m_sumTimeSpan;//ģʽʱ��
	int                 m_bModeStat;//�Ƿ����ģʽ����

	int					GetCassetteState();

	//����������
	int                 progressNumber;//��ǰ����
	bool                m_bTextFlash;

	//����Excel
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
