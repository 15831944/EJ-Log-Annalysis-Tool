// LogAnalysisToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LogAnalysisTool.h"
#include "LogAnalysisToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogAnalysisToolDlg dialog

CLogAnalysisToolDlg::CLogAnalysisToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogAnalysisToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogAnalysisToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogAnalysisToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogAnalysisToolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogAnalysisToolDlg, CDialog)
	//{{AFX_MSG_MAP(CLogAnalysisToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BtnOpenFile, OnBtnOpenFile)
	ON_BN_CLICKED(BtnTransactionAnalysis, OnBtnTransactionAnalysis)
	ON_WM_TIMER()
	ON_BN_CLICKED(BtnExcelExport, OnBtnExcelExport)
	ON_BN_CLICKED(BtnSelectFolder, OnBtnSelectFolder)
	ON_BN_CLICKED(IDC_BtnSearch, OnBtnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST6, OnDblclkList6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogAnalysisToolDlg message handlers

BOOL CLogAnalysisToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//initialValue();

	//控件变量初始化
	m_hWndListCtrol1= (CListCtrl*)GetDlgItem(IDC_LIST1);
	m_hWndListCtrol2= (CListCtrl*)GetDlgItem(IDC_LIST2);
	m_hWndListCtrol3= (CListCtrl*)GetDlgItem(IDC_LIST3);
	m_hWndListCtrol4= (CListCtrl*)GetDlgItem(IDC_LIST4);
	m_hWndListCtrol6= (CListCtrl*)GetDlgItem(IDC_LIST6);
	m_hWndProgressCtrl1= (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);

	//进度条初始化
	progressNumber = 0;
	m_hWndProgressCtrl1->SetRange32(0,ProcessMaxValue);

	//得到配置文件路径
	CString		strPath;
	GetModuleFileName( NULL, strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer();
    int pos = strPath.ReverseFind('\\');
    strPath = strPath.Left(pos);
	memset( m_szConfigFileName, 0, sizeof(m_szConfigFileName) );
	sprintf( m_szConfigFileName, "%s\\Item.cfg", strPath );

	//全局变量初始化
	m_bFileSelect = false;
	m_bHiddenResultWhenZero = false;
	m_iListViewIndex = 0;
	m_iACDepositedAmount = 0;
	m_iACDepositedCounter = 0;
	m_iACDepositedTimes = 0;
	m_bFinishProcessing = true;
	m_bMoreMachineAnalysis = false;

	m_bFindKeyWord = false;

	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDYES)->ShowWindow(SW_SHOW);
	m_hWndListCtrol4->ShowWindow(SW_SHOW);

	listBoxInitial();//列表框初始化
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//复位函数
void CLogAnalysisToolDlg::reset()
{
	//全局变量初始化
	m_bFileSelect = false;//正确选中文件
	m_bHiddenResultWhenZero = false;//隐藏为零项
	m_iListViewIndex = 0;
	m_iACDepositedAmount = 0;
	m_iACDepositedCounter = 0;
	m_iACDepositedTimes = 0;
	m_bFinishProcessing = true;
	m_bMoreMachineAnalysis = false;

	m_strSumPrompt.Empty();//提示字符串清空
	m_vecFilename.clear();//选择的文件名清空
	memset( m_tNotesInfo, 0, sizeof(m_tNotesInfo) );//币种信息结构体数组清空
	
	//模式统计用的时间变量
	memset( &m_LastFileSearchBeginTime, 0, sizeof(m_LastFileSearchBeginTime) );
	memset( &m_sumTimeSpan, 0, sizeof(m_sumTimeSpan) );//模式总时间
	m_bFindKeyWord = false;//是否找到模式关键字
	
	m_mapFileDateName.clear();
	m_vecGapLeftFilename.clear();
 	m_vecGapRightFilename.clear();

	//设定闪烁计时器
	m_bTextFlash = true;

	//控件复位
	m_hWndListCtrol1->DeleteAllItems();
	m_hWndListCtrol2->DeleteAllItems();
	m_hWndListCtrol3->DeleteAllItems();
	m_hWndListCtrol4->DeleteAllItems();
	m_hWndListCtrol6->DeleteAllItems();
	//GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDYES)->ShowWindow(SW_SHOW);
    //m_hWndListCtrol3->ShowWindow(SW_HIDE);

	progressNumber = 0;
	m_hWndProgressCtrl1->SetPos(progressNumber);
}

//初始化列表框
void CLogAnalysisToolDlg::listBoxInitial()
{
	m_hWndListCtrol1->InsertColumn(0,_T(" "),LVCFMT_LEFT,0);//第一列不能居中，应隐藏
	m_hWndListCtrol1->InsertColumn(1,_T("                StatItems"),LVCFMT_LEFT,260);
    m_hWndListCtrol1->InsertColumn(2,_T("Frequency"),LVCFMT_CENTER,80);
    m_hWndListCtrol1->InsertColumn(3,_T("                          Comments"),LVCFMT_LEFT,390);

	m_hWndListCtrol2->InsertColumn(0,_T(" "),LVCFMT_CENTER,0);//第一列不能居中，应隐藏
	m_hWndListCtrol2->InsertColumn(1,_T("CassetteID"),LVCFMT_CENTER,110);
    m_hWndListCtrol2->InsertColumn(2,_T("Denomination"),LVCFMT_CENTER,85);
    m_hWndListCtrol2->InsertColumn(3,_T("Frequency"),LVCFMT_CENTER,70);
	m_hWndListCtrol2->InsertColumn(4,_T("Count"),LVCFMT_CENTER,70);
	m_hWndListCtrol2->InsertColumn(5,_T("Amount"),LVCFMT_CENTER,140);

	m_hWndListCtrol3->InsertColumn(0,_T(" "),LVCFMT_CENTER,0);//第一列不能居中，应隐藏
	m_hWndListCtrol3->InsertColumn(1,_T("Mode"),LVCFMT_CENTER,80);
    m_hWndListCtrol3->InsertColumn(2,_T("PowerUp"),LVCFMT_CENTER,80);
    m_hWndListCtrol3->InsertColumn(3,_T("OffLine"),LVCFMT_CENTER,100);
	m_hWndListCtrol3->InsertColumn(4,_T("Maintenance"),LVCFMT_CENTER,100);
	m_hWndListCtrol3->InsertColumn(5,_T("OutOfService"),LVCFMT_CENTER,100);
	m_hWndListCtrol3->InsertColumn(6,_T("InService"),LVCFMT_CENTER,100);

	m_hWndListCtrol4->InsertColumn(0,_T(" "),LVCFMT_CENTER,0);//第一列不能居中，应隐藏
	m_hWndListCtrol4->InsertColumn(1,_T("CassetteID"),LVCFMT_CENTER,150);
    m_hWndListCtrol4->InsertColumn(2,_T("FullFrequency"),LVCFMT_CENTER,150);
	m_hWndListCtrol4->InsertColumn(3,_T("EmptyFrequency"),LVCFMT_CENTER,150);

	m_hWndListCtrol6->SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_hWndListCtrol6->InsertColumn(0,_T(" "),LVCFMT_CENTER,0);//第一列不能居中，应隐藏
	m_hWndListCtrol6->InsertColumn(1,_T("No"),LVCFMT_CENTER,100);
    m_hWndListCtrol6->InsertColumn(2,_T("FilePath"),LVCFMT_CENTER,620);
}

void CLogAnalysisToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLogAnalysisToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLogAnalysisToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//打开EJ日志(可选择多个)
void CLogAnalysisToolDlg::OnBtnOpenFile() 
{
	// TODO: Add your control notification handler code here

	if (!m_bFinishProcessing)
	{
		return;
	}

	reset();//初始化变量

	CString pathName,fileName,fileTitle;  
    char* filters = _T("log(*.log)|*.log|All Files (*.*)|*.*||");  
	
    //创建一个可以选择多个文件的CFileDialog  
    CFileDialog fileDlg(true,NULL,"*.log",OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY,filters);  
	
    //最多可以打开500个文件  
    fileDlg.m_ofn.nMaxFile = 500 * MAX_PATH;  
	CString szPath = ".\\EJLogFile\\";
    fileDlg.m_ofn.lpstrInitialDir = szPath;
	
    char* l_chTemp = new TCHAR[fileDlg.m_ofn.nMaxFile];  
    fileDlg.m_ofn.lpstrFile = l_chTemp;  
	
    //对内存块清零  
    ZeroMemory(fileDlg.m_ofn.lpstrFile,sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);  

	CString l_strSelectFileName = "File Name: ";
	int     iMaxCount = 0;
	
    //显示文件对话框，获得文件名集合  
    if(fileDlg.DoModal() == IDOK)
	{  
        //获取第一个文件的位置  
        POSITION pos_file;  
        pos_file = fileDlg.GetStartPosition();   
		m_mapFileDateName.clear();
        //循环读出每个路径并存放在数组中  
        while(pos_file != NULL)
		{  
            //将文件路径存放在数组中  
            pathName = fileDlg.GetNextPathName(pos_file);      
			COleDateTime l_EndTime;
			int c = GetLogDate(pathName, l_EndTime);
			char* szfile = pathName.GetBuffer(pathName.GetLength());
			m_mapFileDateName.insert(pair<COleDateTime,CString>(l_EndTime, pathName)); //当时间相同，不会插入记录
        }      

		COleDateTime l_preLogDate = m_mapFileDateName.begin()->first;
		COleDateTime l_curLogDate;
		CString      l_strPreFileName;
		CString      l_strCurFileName;
		map <COleDateTime,CString>::iterator map_Iter;
		int aa = m_mapFileDateName.size();

		for(map_Iter=m_mapFileDateName.begin();map_Iter!=m_mapFileDateName.end();map_Iter++)
		{
			CString l_strFilePath = map_Iter->second;
			m_vecFilename.push_back(l_strFilePath);//保存按时间排序的路径名
			l_curLogDate = map_Iter->first;//得到该日志时间

			//获取文件名
			int length = l_strFilePath.GetLength();        
			for(int i = length -1; i>0;i--)  
			{  
				if('\\' == l_strFilePath. GetAt(i))  
				{
					l_strCurFileName = l_strFilePath.Right(length - i -1);  
					break;//跳出循环  
				}  
			}

			COleDateTimeSpan timeSpan = l_curLogDate - l_preLogDate;
			if (timeSpan.GetDays() >= 2)//两个文件之间间隔大于两天
			{
				CString strPrompt = l_strPreFileName + " and " + l_strCurFileName;
				m_strSumPrompt += strPrompt + "\n"; 
				m_vecGapLeftFilename.push_back(l_strPreFileName);//记录间隔左右文件名
				m_vecGapRightFilename.push_back(l_strCurFileName);
			}

			l_preLogDate = l_curLogDate;
			l_strPreFileName = l_strCurFileName;

			if (0 == iMaxCount)
			{
				l_strSelectFileName += l_strCurFileName;
				iMaxCount++;
			} 
			else
			{
				if (iMaxCount < 4)
				{
					l_strSelectFileName = l_strSelectFileName + "; " + l_strCurFileName;
					iMaxCount++;
				}			
			}
		}
    }  

	if (m_vecFilename.size() > 0)
	{
		m_bFileSelect = true;
	}
	if (m_bFileSelect)//显示有多少日志文件被选择
	{
		CString str;
		str.Format("%d",m_mapFileDateName.size());
		CString strcount = "File Count: "; 
		strcount += str;
		GetDlgItem(IDYES)->SetWindowText(strcount.GetBuffer(strcount.GetLength()));
		GetDlgItem(IDYES)->ShowWindow(SW_SHOW);

		if(4 <= iMaxCount)
		{
			l_strSelectFileName += " ... ...";
		}
		GetDlgItem(IDC_FileName)->SetWindowText(l_strSelectFileName.GetBuffer(l_strSelectFileName.GetLength()));
	    GetDlgItem(IDC_FileName)->ShowWindow(SW_SHOW);
	}
	m_hWndListCtrol3->ShowWindow(SW_SHOW);
	m_bMoreMachineAnalysis = false;//单机分析
    delete[] l_chTemp; 
}

void CLogAnalysisToolDlg::OnBtnSelectFolder() 
{
	// TODO: Add your control notification handler code here
	if (!m_bFinishProcessing)
	{
		return;
	}
	reset();//初始化变量
	m_bMoreMachineAnalysis = true;//多机分析

	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  
	
	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("Please select a folder:");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  
	
	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  
	
	//遍历文件夹中的文件
	GetFileNameFromFolder( strFolderPath);

	if (m_vecFilename.size() > 0)
	{
		m_bFileSelect = true;
	}

	if (m_bFileSelect)//显示有多少日志文件被选择及
	{
		CString str;
		str.Format("%d",m_vecFilename.size());
		CString strcount = "File Count: "; 
		strcount += str;
		GetDlgItem(IDYES)->SetWindowText(strcount.GetBuffer(strcount.GetLength()));
		GetDlgItem(IDYES)->ShowWindow(SW_SHOW);
	}
    return ;  
}

//遍历文件夹中所有文件(递归)
void CLogAnalysisToolDlg::GetFileNameFromFolder( CString strFolderPath)
{
     CFileFind fileFinder;
     CString filePath = strFolderPath + _T("//*.*");

	 CString l_strSelectFileName = "File Name: ";
	 int     iMaxCount = 0;
      
     BOOL bFinished = fileFinder.FindFile(filePath);
     while(bFinished)  //每次循环对应一个类别目录
     {
          bFinished = fileFinder.FindNextFile();
          if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
          {
                 GetFileNameFromFolder(fileFinder.GetFilePath());
          }
          else  //再判断是否为txt文件
          {
              //获取文件类型
              CString filePath = fileFinder.GetFilePath();
              int dotPos=filePath.ReverseFind('.');
              CString fileExt=filePath.Right(filePath.GetLength()-dotPos);
			
			  CString l_strFileName = filePath;
			  //获取文件名
			  int length = filePath.GetLength();        
			  for(int i = length -1; i>0;i--)  
			  {  
				  if('\\' == filePath. GetAt(i))  
				  {
					  l_strFileName = filePath.Right(length - i -1);  
					  break;//跳出循环  
				  }  
			  }

			  char* b = l_strFileName.GetBuffer(l_strFileName.GetLength());
              if(fileExt == _T(".log") && l_strFileName.Find("EJ") >= 0) //文件名为EJ日志文件
              {
                  char* a = filePath.GetBuffer(filePath.GetLength());
				  m_vecFilename.push_back(filePath);//保存文件路径名

				  if (0 == iMaxCount && strlen(l_strFileName) > 3)
				  {
					  l_strSelectFileName += l_strFileName;
					  iMaxCount++;
				  } 
				  else
				  {
					  if (iMaxCount < 4 && strlen(l_strFileName) > 3)
					  {
						  l_strSelectFileName = l_strSelectFileName + "; " + l_strFileName;
						  iMaxCount++;
					  }			
				  }
              }
          }
       }

	 if(4 <= iMaxCount)
	 {
		 l_strSelectFileName += " ... ...";
	 }
	 GetDlgItem(IDC_FileName)->SetWindowText(l_strSelectFileName.GetBuffer(l_strSelectFileName.GetLength()));
	 GetDlgItem(IDC_FileName)->ShowWindow(SW_SHOW);

 
       fileFinder.Close();
}

//根据日志内容得到该日志日期（年-月-日）
int	CLogAnalysisToolDlg::GetLogDate( CString l_strFileName, COleDateTime& l_EndTime)
{
	wchar_t l_szwLineData[2048]={0};
	char	l_szLineData[2048]={0};
	char*   l_pcLineData = NULL;
	char  l_szFileName[256];

    memset( l_szFileName, 0, sizeof(l_szFileName) );
	strcpy( l_szFileName, l_strFileName.GetBuffer(l_strFileName.GetLength()) );
	if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
	{
		try
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
				return 0;
			delete[] l_pwcFileName;//释放动态分配的内存
				
			char*	pstr=NULL;
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);		
				if( l_pcLineData != NULL )
				{
					if (0 == GetLineTime( l_pcLineData, l_EndTime))//有效时间
					{
							
						return 0;
					}	
					else
					{
						continue;
					}
				}
				delete[] l_pcLineData;//释放动态分配的内存
			}

			fclose( fp );
		}
		catch (...)
		{
			return -1;
		}
	} 
	else//当文本为Ansi编码
	{
		try
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
				return 0;
			
			char*	pstr=NULL;
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
			{		
				if( l_szLineData != NULL )
				{
					if (0 == GetLineTime( l_szLineData, l_EndTime))//有效时间
					{
						
						return 0;
					}	
					else
					{
						continue;
					}
				}
			}
			
			fclose( fp );
		}
		catch (...)
		{
			return -1;
		}	
	}

	return -1;
}

UINT AnalysisThreadFun(LPVOID lpParam)  
{
	CLogAnalysisToolDlg* l_hWnMainForm = ( CLogAnalysisToolDlg* )lpParam;
	l_hWnMainForm->GetDlgItem(IDC_State)->SetWindowText("System is processing and please wait ...");
	l_hWnMainForm->GetDlgItem(IDC_State)->ShowWindow(SW_SHOW);
	l_hWnMainForm->SetTimer(ID_TIMER,200,NULL);
	l_hWnMainForm->m_bFinishProcessing = false;


	l_hWnMainForm->WriteSearchConfiFile();//根据选定文件得到钱箱ID，币值，自动配置
	l_hWnMainForm->StatCashDispensed();//配钞信息统计
	l_hWnMainForm->StatCashRejected();//拒钞信息统计
	l_hWnMainForm->StatCashACDeposited();//AC钱箱存款信息统计
	l_hWnMainForm->StatCashDeposited();//存款信息统计
	l_hWnMainForm->GetCassetteState();//统计钱箱状态信息

	l_hWnMainForm->TranscationAnalysisFun();

	//bool bModeStat = l_hWnMainForm->GetDlgItem(IDC_CHECK2)->GetCheck(); 
	CButton* pBtn = (CButton*)l_hWnMainForm->GetDlgItem(IDC_CHECK2);
	l_hWnMainForm->m_bModeStat = pBtn->GetCheck();
	if (1 == l_hWnMainForm->m_bModeStat)//根据复选框的值确定是否对模式进行分析
	{
		l_hWnMainForm->StatMode();//ATM模式统计
	}
	

	KillTimer(l_hWnMainForm->GetSafeHwnd(), ID_TIMER);
	l_hWnMainForm->GetDlgItem(IDC_State)->SetWindowText("Finish processing and Congratulation!");
	l_hWnMainForm->m_hWndProgressCtrl1->SetPos(ProcessMaxValue);//交易分析完成
	l_hWnMainForm->m_bFinishProcessing = true;
	return 0;
}

//交易信息分析按钮
void CLogAnalysisToolDlg::OnBtnTransactionAnalysis()
{
	// TODO: Add your control notification handler code here
	if( m_bFileSelect == FALSE || m_vecFilename.empty())//当未选中任何文件时，对话框提示
	{
		MessageBox( "Please select a file." );
		return;
	}
	m_bFileSelect = FALSE;//选择文件变量复位

	//日志文件存在时间间隔，显示提示对话框
	if (m_strSumPrompt.GetLength() != 0)
	{//There are time gaps between these logs!
		if(MessageBox( m_strSumPrompt, "Tips: Following Logs are Discontinuous in date! Do you want to continue?  ",   
            MB_ICONWARNING   |   MB_YESNO) == IDYES) 
		{
			//创建交易分析线程
	        AfxBeginThread(AnalysisThreadFun,this);
		}	
		else
		{
			reset();//选择的文件无效
		}
	}
	else
	{
		//创建交易分析线程
	    AfxBeginThread(AnalysisThreadFun,this);
	}

	return ;
}

struct CassetteInfo
{
	char CASID[10];
	char Deno[10];
	char NoteCode[10];
};

//根据选定的日志文件得到钱箱ID，币值，自动配置搜索文件
int CLogAnalysisToolDlg::WriteSearchConfiFile()
{
	char	l_szFileName[256];//保存打开的文件名字
	long	l_CasNumber=0;
	char	l_szKeyWord[10]="CAS(";
	char    l_szCasID[10][10] = {0};
	CassetteInfo l_CasInfo[10];
	memset( l_CasInfo, 0, sizeof(l_CasInfo) );
	bool    l_bFinish = false;
	vector<CString>::iterator iter; 
	for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//循环搜索选定日志文件
	{
		if (l_bFinish)//已经找到8个CAS ID，完成搜索
		{
			break;
		}
		memset( l_szFileName, 0, sizeof(l_szFileName) );
		strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
		
		wchar_t l_szwLineData[2048]={0};
		char	l_szLineData[2048]={0};
		char*   l_pcLineData = NULL;
		
		if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
		{
			try
			{
				wchar_t*  l_pwcFileName = NULL;
				l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
				FILE*	fp=NULL;
				if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
					return 0;
				delete[] l_pwcFileName;//释放动态分配的内存
				
				char*	pstr=NULL;
				while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )//循环读取每一行的循环
				{
					l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
					pstr = NULL;
					pstr = strstr(l_pcLineData, l_szKeyWord ); //当找到CAS(字符
					
					if( pstr != NULL &&strlen(pstr) > 35 && pstr[9] == ')' )//是CAS1-CAS4钱箱，防止"CAS(00050) EMPTY"这种情况造成的访问越界
					{
						char temp[10] = {0};
						memset( temp, 0, sizeof(temp) );
						memcpy(temp, pstr+4, 5);//得到CAS ID

						bool bAddCasInfo = true;
						for (int i = 0; i < l_CasNumber; i++)
						{
							if (0== strcmp(l_CasInfo[i].CASID, temp))//已经存在该钱箱信息
							{
								bAddCasInfo = false;
							}
						}

						if (bAddCasInfo)//添加到CasInfo数组
						{
							memcpy(l_CasInfo[l_CasNumber].CASID, temp, 5);//添加CASID
							
							pstr = strstr(pstr, "/" );
							char DnoTemp[10] = {0};
							int DnoTempIndex = 0;
							int tempIndex = 0;
							for (tempIndex= 1; pstr[tempIndex] != '/'; tempIndex++)
							{
								if (' ' != pstr[tempIndex])//将面额的空格去掉
								{
									DnoTemp[DnoTempIndex] = pstr[tempIndex];
									DnoTempIndex++;
								}
							}
							DnoTemp[DnoTempIndex] = '\0';
							memcpy(l_CasInfo[l_CasNumber].Deno, DnoTemp, DnoTempIndex);//添加Deno
							
							char NoteCode[10] = {0};
							memcpy(l_CasInfo[l_CasNumber].NoteCode, pstr + tempIndex+1, 3);//添加NoteCode
							l_CasNumber ++;
							
							if (8 == l_CasNumber)//已经找到4个CasInfo
							{
								l_bFinish = true;
								break;
							}
						}
					}
					delete[] l_pcLineData;//释放动态分配的内存
				}//end while循环读取每一行的循环
				fclose( fp );
			}
			catch (...)
			{
				return 0;
			}

		} 
		else//当文本为Ansi编码
		{
			try
			{
				FILE*	fp=NULL;
				if( (fp=fopen(l_szFileName, "r")) == NULL )
					return 0;
				
				char*	pstr=NULL;
				while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
				{
					pstr = NULL;
					pstr = strstr(l_szLineData, l_szKeyWord ); 

					if( pstr != NULL &&strlen(pstr) > 35 && pstr[9] == ')')//是CAS1-CAS4钱箱，防止"CAS(00050) EMPTY"这种情况造成的访问越界
					{
						char temp[10] = {0};
						memset( temp, 0, sizeof(temp) );
						memcpy(temp, pstr+4, 5);
						bool bAddCasInfo = true;
						for (int i = 0; i < l_CasNumber; i++)
						{
							if (0== strcmp(l_CasInfo[i].CASID, temp))//已经存在该钱箱信息
							{
								bAddCasInfo = false;
							}
						}
						
						if (bAddCasInfo)//添加到CasInfo数组
						{
							memcpy(l_CasInfo[l_CasNumber].CASID, temp, 5);//添加CASID
							
							pstr = strstr(pstr, "/" );
							char DnoTemp[10] = {0};
							int DnoTempIndex = 0;
							int tempIndex = 0;
							for (tempIndex= 1; pstr[tempIndex] != '/'; tempIndex++)
							{
								if (' ' != pstr[tempIndex])//将面额的空格去掉
								{
									DnoTemp[DnoTempIndex] = pstr[tempIndex];
									DnoTempIndex++;
								}
							}
							DnoTemp[DnoTempIndex] = '\0';
							memcpy(l_CasInfo[l_CasNumber].Deno, DnoTemp, DnoTempIndex);//添加Deno
							
							char NoteCode[10] = {0};
							memcpy(l_CasInfo[l_CasNumber].NoteCode, pstr + tempIndex+1, 3);//添加NoteCode
							l_CasNumber ++;
							
							if (8 == l_CasNumber)//已经找到4个CasInfo
							{
								l_bFinish = true;
								break;
							}
						}
					}
				}
				fclose( fp );
			}
			catch (...)
			{
				return 0;
			}
		}
	}//end for循环搜索选定日志文件

	CString l_strWriteTemp = "";
	l_strWriteTemp.Empty();
	for(int CurrencyIndex = 0; CurrencyIndex < l_CasNumber; CurrencyIndex++)
	{
		CString str = l_CasInfo[CurrencyIndex].CASID;
		if (l_strWriteTemp.IsEmpty())
		{
			l_strWriteTemp +=  str;
		}
		else
		{
			l_strWriteTemp +=  "," +str;
		}
	}

	WritePrivateProfileString( "Dispensed", "CASIDs", l_strWriteTemp, m_szConfigFileName );
	WritePrivateProfileString( "Rejected", "CASIDs", l_strWriteTemp, m_szConfigFileName );
	WritePrivateProfileString( "CassetteState", "CASIDs", l_strWriteTemp, m_szConfigFileName );
	l_strWriteTemp = "CAS0," + l_strWriteTemp;//多了一个AC回收箱
	WritePrivateProfileString( "Deposited", "CASIDs", l_strWriteTemp, m_szConfigFileName );

	//dispensed出钞配置项
// 	l_strWriteTemp.Empty();//设定KeyWord
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[0].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Dispensed", "CAS1", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[1].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Dispensed", "CAS2", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[2].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Dispensed", "CAS3", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[3].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Dispensed", "CAS4", l_strWriteTemp, m_szConfigFileName );

	for (int iIndex = 0; iIndex < l_CasNumber; iIndex++)//dispensed出钞配置项
	{
		l_strWriteTemp.Empty();
		l_strWriteTemp = "[CAS " + (CString)l_CasInfo[iIndex].CASID +  "] OUT:";
		CString str;
		str.Format("%d",iIndex + 1); 
	    WritePrivateProfileString( "Dispensed", "CAS"+ str, l_strWriteTemp, m_szConfigFileName );
	}


// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[0].Deno;//设定面额
// 	WritePrivateProfileString( "Dispensed", "CAS1_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[1].Deno;
// 	WritePrivateProfileString( "Dispensed", "CAS2_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[2].Deno;
// 	WritePrivateProfileString( "Dispensed", "CAS3_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[3].Deno;
// 	WritePrivateProfileString( "Dispensed", "CAS4_DENO", l_strWriteTemp, m_szConfigFileName );

	for (iIndex = 0; iIndex < l_CasNumber; iIndex++)
	{
		l_strWriteTemp.Empty();
		l_strWriteTemp = (CString)l_CasInfo[iIndex].Deno;//设定面额
		CString str;
		str.Format("%d",iIndex + 1); 
	    WritePrivateProfileString( "Dispensed", "CAS" + str + "_DENO", l_strWriteTemp, m_szConfigFileName );
	}
	
	//Rejected出钞据钞配置项
// 	l_strWriteTemp.Empty();//设定KeyWord
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[0].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Rejected", "CAS1", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[1].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Rejected", "CAS2", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[2].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Rejected", "CAS3", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = "[CAS " + (CString)l_CasInfo[3].CASID +  "] OUT:";
// 	WritePrivateProfileString( "Rejected", "CAS4", l_strWriteTemp, m_szConfigFileName );

	for (iIndex = 0; iIndex < l_CasNumber; iIndex++)//Rejected出钞据钞配置项
	{
		l_strWriteTemp.Empty();//设定KeyWord
		l_strWriteTemp = "[CAS " + (CString)l_CasInfo[iIndex].CASID +  "] OUT:";
		CString str;
		str.Format("%d",iIndex + 1); 
	    WritePrivateProfileString( "Rejected", "CAS" + str, l_strWriteTemp, m_szConfigFileName );
	}
	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[0].Deno;//设定面额
// 	WritePrivateProfileString( "Rejected", "CAS1_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[1].Deno;
// 	WritePrivateProfileString( "Rejected", "CAS2_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[2].Deno;
// 	WritePrivateProfileString( "Rejected", "CAS3_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[3].Deno;
// 	WritePrivateProfileString( "Rejected", "CAS4_DENO", l_strWriteTemp, m_szConfigFileName );

	for (iIndex = 0; iIndex < l_CasNumber; iIndex++)
	{
		l_strWriteTemp.Empty();
		l_strWriteTemp = (CString)l_CasInfo[iIndex].Deno;//设定面额
		CString str;
		str.Format("%d",iIndex + 1); 
	    WritePrivateProfileString( "Rejected", "CAS" + str + "_DENO", l_strWriteTemp, m_szConfigFileName );
	}
	
	//Deposited存款配置项
// 	l_strWriteTemp.Empty();//设定KeyWord
// 	l_strWriteTemp = "[CAS1] IN: [" + (CString)l_CasInfo[0].NoteCode + " " + (CString)l_CasInfo[0].Deno +  "]";
// 	WritePrivateProfileString( "Deposited", "CAS1", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp =  "[CAS2] IN: [" + (CString)l_CasInfo[1].NoteCode + " " + (CString)l_CasInfo[1].Deno +  "]";
// 	WritePrivateProfileString( "Deposited", "CAS2", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp =  "[CAS3] IN: [" + (CString)l_CasInfo[2].NoteCode + " " + (CString)l_CasInfo[2].Deno +  "]";
// 	WritePrivateProfileString( "Deposited", "CAS3", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp =  "[CAS4] IN: [" + (CString)l_CasInfo[3].NoteCode + " " + (CString)l_CasInfo[3].Deno +  "]";
// 	WritePrivateProfileString( "Deposited", "CAS4", l_strWriteTemp, m_szConfigFileName );

	for (iIndex = 0; iIndex < l_CasNumber; iIndex++)//Deposited存款配置项
	{
		l_strWriteTemp.Empty();//设定KeyWord
		CString str;
		str.Format("%d",iIndex + 1);
		str.Format("%d",iIndex + 1); 
		//l_strWriteTemp = "[CAS" + str + "] IN: [" + (CString)l_CasInfo[iIndex].NoteCode + " " + (CString)l_CasInfo[iIndex].Deno +  "]";
	    l_strWriteTemp = "IN: [" + (CString)l_CasInfo[iIndex].NoteCode + " " + (CString)l_CasInfo[iIndex].Deno +  "]";
		WritePrivateProfileString( "Deposited", "CAS" + str, l_strWriteTemp, m_szConfigFileName );
	}
	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[0].Deno;//设定面额
// 	WritePrivateProfileString( "Deposited", "CAS1_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[1].Deno;
// 	WritePrivateProfileString( "Deposited", "CAS2_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[2].Deno;
// 	WritePrivateProfileString( "Deposited", "CAS3_DENO", l_strWriteTemp, m_szConfigFileName );
// 	
// 	l_strWriteTemp.Empty();
// 	l_strWriteTemp = (CString)l_CasInfo[3].Deno;
// 	WritePrivateProfileString( "Deposited", "CAS4_DENO", l_strWriteTemp, m_szConfigFileName );

	for (iIndex = 0; iIndex < l_CasNumber; iIndex++)
	{
		l_strWriteTemp.Empty();
		l_strWriteTemp = (CString)l_CasInfo[iIndex].Deno;//设定面额
		CString str;
		str.Format("%d",iIndex + 1);
	    WritePrivateProfileString( "Deposited", "CAS" + str + "_DENO", l_strWriteTemp, m_szConfigFileName );
	}

	return l_CasNumber;
}

void CLogAnalysisToolDlg::TranscationAnalysisFun()
{
	int		l_iIndex=0;
	char	l_szTemp[128]={0};
	char	l_szName[256]={0};
	char	l_szKeyWord[256]={0};
	char	l_szDesc[256]={0};
	int     l_iSumResult[256]={0};//保存总的统计结果

	int     iWithdrwalTrans[256]={0};
	int     iWithdrwalSucess[256]={0};
    WritePrivateProfileString( "JAM", "FILE" , "  ", m_szConfigFileName );
	
	char	l_szFileName[256];//保存打开的文件名字
	while(TRUE)
	{
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%02d", l_iIndex+1 );
		
		memset( l_szName, 0, sizeof(l_szName) );//判断搜索停止的条件
		GetPrivateProfileString( l_szTemp, "NAME", "", l_szName, sizeof(l_szName), m_szConfigFileName );
		if( strlen(l_szName) == 0 )
			break;
		
		m_oFunc.MyInifetch( m_szConfigFileName, l_szTemp, "KEYWORD", l_szKeyWord );//根据索引得到KeyWord
		
		vector<CString>::iterator iter; 
		int     iWithdrwalIndex = 0;
		for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)
		{
			memset( l_szFileName, 0, sizeof(l_szFileName) );
			strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
			int iResult = GetOccurrences( l_szFileName, l_szKeyWord );//得到日志文件中KeyWord出现的次数
			memset( l_szTemp, 0, sizeof(l_szTemp) );
			sprintf( l_szTemp, "%d", iResult );
			l_iSumResult[l_iIndex] += iResult;

// 			if (0 == strcmp(l_szKeyWord, "INOP") && iResult > 0)//记录发生存款卡钞的文件名
// 			{
// 				CString l_strWriteTemp;
// 				char szWriteTemp[1024];
// 				GetPrivateProfileString( "JAM", "FILE", "", szWriteTemp, sizeof(szWriteTemp), m_szConfigFileName );
// 				l_strWriteTemp=CString(szWriteTemp) + "    " + *iter;
// 				WritePrivateProfileString( "JAM", "FILE" , l_strWriteTemp, m_szConfigFileName );
// 			}
// 
// 			if (0 == strcmp(l_szKeyWord, "DISPENSE COMMAND FROM HOST:"))
// 			{
// 				iWithdrwalTrans[iWithdrwalIndex] = iResult;
// 				iWithdrwalIndex ++;
// 			}
// 
// 			if (0 == strcmp(l_szKeyWord, "PRESENT SUCCESS"))
// 			{
// 				iWithdrwalSucess[iWithdrwalIndex] = iResult;
// 				iWithdrwalIndex ++;
// 			}
		}

// 		for (int i = 0; i < m_vecFilename.size(); i++)//统计取款失败的次数
// 		{
// 			if (iWithdrwalTrans[i] - iWithdrwalSucess[i] > 3)
// 			{
// 				CString l_strWriteTemp;
// 				char szWriteTemp[1024];
// 				GetPrivateProfileString( "JAM", "FILE", "", szWriteTemp, sizeof(szWriteTemp), m_szConfigFileName );
// 				l_strWriteTemp=CString(szWriteTemp) + "    " + m_vecFilename[i];
// 				WritePrivateProfileString( "JAM", "FILE" , l_strWriteTemp, m_szConfigFileName );
// 			}
// 		}
		

		if( l_iSumResult[l_iIndex] == 0 && m_bHiddenResultWhenZero)//不显示为0的统计项
		{
			continue;
		}
		LVITEM	lvitem;
		int		iPos;
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%02d", l_iIndex+1 );
		memset( l_szName, 0, sizeof(l_szName) );
		GetPrivateProfileString( l_szTemp, "NAME", "", l_szName, sizeof(l_szName), m_szConfigFileName );
		if( strlen(l_szName) == 0 )
			break;
		lvitem.mask         =LVIF_TEXT;//第一列不能居中，隐藏
		lvitem.iItem		= l_iIndex;
		lvitem.iSubItem		=0;
		lvitem.pszText		="";
		lvitem.lParam		=l_iIndex;
		iPos=m_hWndListCtrol1->InsertItem(&lvitem);//返回表项插入后的索引号
		
		lvitem.mask         =LVIF_TEXT;//插入列表框的第一列“统计项”
		lvitem.iItem		=iPos;
		lvitem.iSubItem		=1;
		lvitem.pszText		=l_szName;
		m_hWndListCtrol1->SetItem(&lvitem);
		
		char	l_szSumTemp[128]={0};
		memset( l_szSumTemp, 0, sizeof(l_szSumTemp) );
		sprintf( l_szSumTemp, "%d", l_iSumResult[l_iIndex] );
		lvitem.mask         =LVIF_TEXT;//插入列表框第二列“统计结果”
		lvitem.iItem		=iPos;
		lvitem.iSubItem		=2;
		lvitem.pszText		=l_szSumTemp;
		m_hWndListCtrol1->SetItem(&lvitem);
		
		GetPrivateProfileString( l_szTemp, "DESC", "", l_szDesc, sizeof(l_szDesc), m_szConfigFileName );
		lvitem.iSubItem		=3;//插入列表框第三项“注释”
		lvitem.pszText		=l_szDesc;
		m_hWndListCtrol1->SetItem(&lvitem);

		l_iIndex ++; 
	}
}

//在选定日志文件中统计KeyWord出现的次数
int	CLogAnalysisToolDlg::GetOccurrences( char* l_szFileName, char* l_szKeyWord )
{
	wchar_t l_szwLineData[2048]={0};
	char	l_szLineData[2048]={0};
	char*   l_pcLineData = NULL;
	long	l_lOcc=0;
	
	if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
	{
		try
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
				return 0;
			delete[] l_pwcFileName;//释放动态分配的内存
			
			char*	pstr=NULL;
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
				pstr = NULL;
				pstr = strstr(l_pcLineData, l_szKeyWord ); 
				
				if( pstr != NULL )
					l_lOcc ++;
				delete[] l_pcLineData;//释放动态分配的内存
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	} 
	else//当文本为Ansi编码
	{
		try
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
				return 0;
			
			char*	pstr=NULL;
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
			{
				pstr = NULL;
				pstr = strstr(l_szLineData, l_szKeyWord ); 
				
				if( pstr != NULL )
					l_lOcc ++;
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	}
	
	return l_lOcc;
}



/*************************************************************************************************************************/
/*                                             根据配置文件的格式，统计配钞张数                                          */
/*************************************************************************************************************************/
void CLogAnalysisToolDlg::StatCashDispensed()
{
	InsertDataToNotesList( "---Dispensed---", "---------", "---------", "---------", "--------------------" );//插入列表框标题
	char l_szTitleList[1024]={0};
	::GetPrivateProfileString( "Dispensed", "CASIDs", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
	if( strlen(l_szTitleList) == 0 )
	{
		InsertDataToNotesList( "0", "", "", "", "" );
		return;
	}
	char l_szTemp[512]={0};
	char *pstr=l_szTitleList;
	
	int		iPacer=0;
	int		l_iDeno = 1;
	char	l_szKeyWord[512]={0};
	char	l_szComment[1024]={0};
	char	l_szTitle[128]={0};
	CString	l_strCasName = "";
	iPacer = ::GetPrivateProfileInt( "Dispensed", "Pacer", 0, m_szConfigFileName );
	
	int		l_iTotalTimes = 0;
	int		l_iTotalCounter = 0;
	long	l_iTotalAmount = 0;
	int     CasIndex = 1;
	while( TRUE )//根据配置的币种进行循环搜素
	{
		if( pstr == NULL || *pstr == NULL || (pstr+1) == NULL || *(pstr+1) == NULL)
			break;
		memset( l_szTitle, 0, sizeof(l_szTitle) );
		int i=0;
		for( ; *pstr !=',' && *pstr != NULL; pstr++ )//得到每一个钱箱ID
		{
			l_szTitle[i++] = *pstr;
		}
		pstr ++;

		l_strCasName.Empty();//将钱箱ID转换为钱箱名字
		CString strTemp;
		strTemp.Format("%d", CasIndex);
		l_strCasName = "CAS" + strTemp;
		CasIndex++;

		memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
		::GetPrivateProfileString( "Dispensed", l_strCasName.GetBuffer(l_strCasName.GetLength()), NULL, l_szKeyWord, sizeof(l_szKeyWord), m_szConfigFileName );
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%s_Comment", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
		memset( l_szComment, 0, sizeof(l_szComment) );
		::GetPrivateProfileString( "Dispensed", l_szTemp, NULL, l_szComment, sizeof(l_szComment), m_szConfigFileName );
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%s_DENO", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
		l_iDeno = ::GetPrivateProfileInt( "Dispensed", l_szTemp, 1, m_szConfigFileName );//得到该币种面额
		
		int iSumTimes = 0;
		int iSumCounter = 0;
		vector<CString>::iterator iter; 
	    for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//遍历所有打开的文件
		{
			char  l_szFileName[256];
			memset( l_szFileName, 0, sizeof(l_szFileName) );
	        strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
			int iTimes=0;
	    	int iCounter = GetCounter( l_szFileName, l_szKeyWord, iPacer, &iTimes );//得到该币种次数及该钞票的张数
			iSumTimes += iTimes;
			iSumCounter += iCounter;
		}
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%d", iSumCounter );
		l_iTotalCounter += iSumCounter;//得到总的数量
		
		char l_szTemp2[32]={0};
		memset( l_szTemp2, 0, sizeof(l_szTemp2) );
		sprintf( l_szTemp2, "%d", iSumCounter*l_iDeno );//得到配钞总金额
		l_iTotalAmount += iSumCounter*l_iDeno;//计算总的数量
		
		char l_szDeno[32]={0};
		memset( l_szDeno, 0, sizeof(l_szDeno) );
		sprintf( l_szDeno, "%d", l_iDeno );
		
		char l_szTimes[32]={0};
		memset( l_szTimes, 0, sizeof(l_szTimes) );
		sprintf( l_szTimes, "%d", iSumTimes );
		l_iTotalTimes += iSumTimes;
		InsertDataToNotesList( l_szTitle, l_szDeno, l_szTimes, l_szTemp, l_szTemp2 );
	}
	
	//添加NotesList列表框总的（total）统计
	memset( l_szTemp, 0, sizeof(l_szTemp) );
	sprintf( l_szTemp, "%d", l_iTotalCounter );
	
	char l_szTemp2[32]={0};
	memset( l_szTemp2, 0, sizeof(l_szTemp2) );
	sprintf( l_szTemp2, "%d", l_iTotalAmount );
	
	char l_szTimes[32]={0};
	memset( l_szTimes, 0, sizeof(l_szTimes) );
	sprintf( l_szTimes, "%d", l_iTotalTimes );
	InsertDataToNotesList( "Total", "/", l_szTimes, l_szTemp, l_szTemp2 );
}

//在NotesList中插入一行记录数据
void CLogAnalysisToolDlg::InsertDataToNotesList( char *szTitle, char *szDeno, char *szTimes, char *szCounter, char *szAmount )
{
	LVITEM	lvitem;
	int		iPos;
	char	l_szCounter[32]={0};
	char	l_szAmount[32]={0};
	
	lvitem.mask         =LVIF_TEXT;
	lvitem.iItem		= m_iListViewIndex;
	lvitem.iSubItem		=0;
	lvitem.pszText		=" ";
	lvitem.lParam		=m_iListViewIndex;
	iPos=m_hWndListCtrol2->InsertItem(&lvitem);//返回表项插入后的索引号

	lvitem.mask         =LVIF_TEXT;
	lvitem.iItem		=iPos;
	lvitem.iSubItem		=1;
	lvitem.pszText		=szTitle;
	m_hWndListCtrol2->SetItem(&lvitem);
	
	lvitem.mask         =LVIF_TEXT;
	lvitem.iItem		=iPos;
	lvitem.iSubItem		=2;
	lvitem.pszText		=szDeno;
	m_hWndListCtrol2->SetItem(&lvitem);
	
	lvitem.mask         =LVIF_TEXT;
	lvitem.iItem		=iPos;
	lvitem.iSubItem		=3;
	lvitem.pszText		=szTimes;
	m_hWndListCtrol2->SetItem(&lvitem);
	
	lvitem.mask         =LVIF_TEXT;
	lvitem.iSubItem		=4;
	lvitem.pszText		=szCounter;
	m_hWndListCtrol2->SetItem(&lvitem);
	
	lvitem.mask         =LVIF_TEXT;
	lvitem.iItem		=iPos;
	lvitem.iSubItem		=5;
	lvitem.pszText		=szAmount;
	m_hWndListCtrol2->SetItem(&lvitem);
	
	m_iListViewIndex ++;
}

/****************************************************************************/
/* 根据搜关键字，得到统计数据，失败返回-1。并且得到关键字后面的数据信息总和 */
/****************************************************************************/
int	CLogAnalysisToolDlg::GetCounter( char *l_szFileName, char *l_szKeyWord, int iPacer, int *Times )
{
	char	l_szTempData[256]={0};
	wchar_t l_szwLineData[2048]={0};
	char	l_szLineData[2048]={0};
	char*   l_pcLineData = NULL;
	long	l_lOcc=0;
	*Times = 0;
	if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
	{
		try
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
				return 0;
			delete[] l_pwcFileName;//释放动态分配的内存
			
			char*	pstr=NULL;
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
				pstr = NULL;
				pstr = strstr(l_pcLineData, l_szKeyWord ); 
				
				if( pstr != NULL )	//找到出现
				{
					try
					{
						if ('0' == *(pstr-3) && 'S' == *(pstr-4))
						{
							continue;
						}
					}
					catch (...)
					{

					}
					
					memset( l_szTempData, 0, sizeof(l_szTempData) );
					int i=0;
					char* pstrStart = pstr + strlen(l_szKeyWord) + iPacer;
					
					//获得钞票张数的判断
					//dispense张数以‘，’结束，iPacer = 0（[CAS 00010] OUT: 01, REJECT: 00）
					//reject张数以Null结束，iPacer = 12
					//deposit张数以Null或者特殊字符结束iPacer = 0
					//refuse张数以？结束iPacer = 0
					//总之保证得到的张数字符串要全部为数字，才不会错过或者遗漏
					for( ; (*pstrStart !=',' || *pstrStart !='?')&& (*pstrStart != NULL) &&
						((*pstrStart >= '0') && (*pstrStart <= '9') || *pstrStart == ' '); pstrStart++ )
					{
						l_szTempData[i++] = *pstrStart;
					}
					if( atoi(l_szTempData) != 0 )
					{
						l_lOcc += atoi(l_szTempData);
						(*Times)++;
					}
				}
				delete[] l_pcLineData;//释放动态分配的内存
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	} 
	else//当文本为Ansi编码
	{
		try
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
				return 0;
			
			char*	pstr=NULL;
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
			{
				pstr = NULL;
				pstr = strstr(l_szLineData, l_szKeyWord ); 
				
				if( pstr != NULL )	//找到出现
				{
					try
					{
						if ('0' == *(pstr-3) && 'S' == *(pstr-4))
						{
							continue;
						}
					}
					catch (...)
					{
						
					}

					memset( l_szTempData, 0, sizeof(l_szTempData) );
					int i=0;
					char* pstrStart = pstr + strlen(l_szKeyWord) + iPacer;
					
					//获得钞票张数的判断
					//dispense张数以‘，’结束，iPacer = 0（[CAS 00010] OUT: 01, REJECT: 00）
					//reject张数以Null结束，iPacer = 12
					//deposit张数以Null或者特殊字符结束iPacer = 0
					//refuse张数以？结束iPacer = 0
					//总之保证得到的张数字符串要全部为数字，才不会错过或者遗漏
					for( ; (*pstrStart !=',' || *pstrStart !='?')&& (*pstrStart != NULL) &&
						((*pstrStart >= '0') && (*pstrStart <= '9') || *pstrStart == ' '); pstrStart++ )
					{
						l_szTempData[i++] = *pstrStart;
					}
					if( atoi(l_szTempData) != 0 )
					{
						l_lOcc += atoi(l_szTempData);
						(*Times)++;
					}
				} 
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	}

	return l_lOcc;
}


/*************************************************************************************************************************/
/*                                             拒钞张数，函数处理跟出钞一样                                             */
/*************************************************************************************************************************/
void CLogAnalysisToolDlg::StatCashRejected()
{
	InsertDataToNotesList( "-DispenseReject-", "---------", "---------", "---------", "--------------------" );
	
	char l_szTitleList[512]={0};
	::GetPrivateProfileString( "Rejected", "CASIDs", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
	if( strlen(l_szTitleList) == 0 )
	{
		InsertDataToNotesList( "0", "", "", "", "" );
		return;
	}
	char l_szTemp[512]={0};
	char *pstr=l_szTitleList;
	
	int		iPacer=0;
	int		l_iDeno = 1;
	char	l_szKeyWord[512]={0};
	char	l_szComment[1024]={0};
	char	l_szTitle[128]={0};
	CString	l_strCasName = "";
	iPacer = ::GetPrivateProfileInt( "Rejected", "Pacer", 0, m_szConfigFileName );
	
	int		l_iTotalTimes = 0;
	int		l_iTotalCounter = 0;
	long	l_iTotalAmount = 0;
	int     CasIndex = 1;
	while( TRUE )
	{
		if( pstr == NULL || *pstr == NULL || (pstr+1) == NULL || *(pstr+1) == NULL)
			break;
		memset( l_szTitle, 0, sizeof(l_szTitle) );
		int i=0;
		for( ; *pstr !=',' && *pstr != NULL; pstr++ )
		{
			l_szTitle[i++] = *pstr;
		}
		pstr ++;

		l_strCasName.Empty();//将钱箱ID转换为钱箱名字
		CString strTemp;
		strTemp.Format("%d", CasIndex);
		l_strCasName = "CAS" + strTemp;
		CasIndex++;

		memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
		::GetPrivateProfileString( "Rejected", l_strCasName.GetBuffer(l_strCasName.GetLength()), NULL, l_szKeyWord, sizeof(l_szKeyWord), m_szConfigFileName );
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%s_Comment", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
		memset( l_szComment, 0, sizeof(l_szComment) );
		::GetPrivateProfileString( "Rejected", l_szTemp, NULL, l_szComment, sizeof(l_szComment), m_szConfigFileName );
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%s_DENO", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
		l_iDeno = ::GetPrivateProfileInt( "Rejected", l_szTemp, 1, m_szConfigFileName );

		int iSumTimes = 0;
		int iSumCounter = 0;
		vector<CString>::iterator iter; 
		for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//遍历所有打开的文件
		{
			char  l_szFileName[256];
			memset( l_szFileName, 0, sizeof(l_szFileName) );
			strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
			int iTimes=0;
			int iCounter = GetCounter( l_szFileName, l_szKeyWord, iPacer, &iTimes );//得到该币种次数及该钞票的张数
			iSumTimes += iTimes;
			iSumCounter += iCounter;
		}
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%d", iSumCounter );
		l_iTotalCounter += iSumCounter;
		
		char l_szTemp2[32]={0};
		memset( l_szTemp2, 0, sizeof(l_szTemp2) );
		sprintf( l_szTemp2, "%d", iSumCounter*l_iDeno );
		l_iTotalAmount += iSumCounter*l_iDeno;
		
		char l_szDeno[32]={0};
		memset( l_szDeno, 0, sizeof(l_szDeno) );
		sprintf( l_szDeno, "%d", l_iDeno );
		char l_szTimes[32]={0};
		memset( l_szTimes, 0, sizeof(l_szTimes) );
		sprintf( l_szTimes, "%d", iSumTimes );
		l_iTotalTimes += iSumTimes;
		
		InsertDataToNotesList( l_szTitle, l_szDeno, l_szTimes, l_szTemp, l_szTemp2 );
	}
	memset( l_szTemp, 0, sizeof(l_szTemp) );
	sprintf( l_szTemp, "%d", l_iTotalCounter );
	
	char l_szTemp2[32]={0};
	memset( l_szTemp2, 0, sizeof(l_szTemp2) );
	sprintf( l_szTemp2, "%d", l_iTotalAmount );
	
	char l_szTimes[32]={0};
	memset( l_szTimes, 0, sizeof(l_szTimes) );
	sprintf( l_szTimes, "%d", l_iTotalTimes );
	InsertDataToNotesList( "Total", "/", l_szTimes, l_szTemp, l_szTemp2 );//在列表框中添加拒钞信息统计
}

/*************************************************************************************************************************/
/*                                                        统计AC箱的存入明细                                             */
/*************************************************************************************************************************/
void CLogAnalysisToolDlg::StatCashACDeposited()
{
	InsertDataToNotesList( "--AC_A Deposit--", "---------", "---------", "---------", "--------------------" );
	
	m_iACDepositedAmount = 0;
	m_iACDepositedCounter = 0;
	m_iACDepositedTimes = 0;
	memset( &m_tNotesInfo, 0, sizeof(m_tNotesInfo) );
	
	char l_szKeyWord[256] = {0};
	memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
	::GetPrivateProfileString( "Deposited", "AC_A_Deposited", NULL, l_szKeyWord, sizeof(l_szKeyWord), m_szConfigFileName );
	if( strlen(l_szKeyWord) == 0 )
	{
		InsertDataToNotesList( "0", "", "", "", "" );
		return;
	}

	vector<CString>::iterator iter; 
	for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//遍历所有打开的文件
	{
		char  l_szFileName[256];
		memset( l_szFileName, 0, sizeof(l_szFileName) );
		strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
		int l_iACDepositedTimes = GetACDepositDetail( l_szFileName, l_szKeyWord );//获得AC存款次数，同时解析保存钞票信息
        m_iACDepositedTimes += l_iACDepositedTimes;
	}
	
	int iCounter = 0;
	while ( TRUE )
	{
		if( strlen(m_tNotesInfo[iCounter].szCur)==0 || 
			m_tNotesInfo[iCounter].iDeno == 0 ||
			m_tNotesInfo[iCounter].iCounter == 0 )
		{
			break;
		}
		char l_szDeno[32]={0};
		char l_szCounter[32]={0};
		char l_szAmount[32]={0};
		
		memset( l_szDeno, 0, sizeof(l_szDeno) );
		sprintf( l_szDeno, "%d", m_tNotesInfo[iCounter].iDeno );
		
		memset( l_szCounter, 0, sizeof(l_szCounter) );
		sprintf( l_szCounter, "%d", m_tNotesInfo[iCounter].iCounter );
		m_iACDepositedCounter += m_tNotesInfo[iCounter].iCounter;
		
		memset( l_szAmount, 0, sizeof(l_szAmount) );
		sprintf( l_szAmount, "%d", m_tNotesInfo[iCounter].iDeno*m_tNotesInfo[iCounter].iCounter );
		m_iACDepositedAmount += m_tNotesInfo[iCounter].iDeno*m_tNotesInfo[iCounter].iCounter;
		InsertDataToNotesList( "CAS0", l_szDeno, "/", l_szCounter, l_szAmount );
		iCounter ++;
	}

	char l_szTotalTimes[32]={0};
	char l_szTotalCounter[32]={0};
	char l_szTotalAmount[32]={0};

	memset( l_szTotalTimes, 0, sizeof(l_szTotalTimes) );
	sprintf( l_szTotalTimes, "%d", m_iACDepositedTimes );

	memset( l_szTotalCounter, 0, sizeof(l_szTotalCounter) );
	sprintf( l_szTotalCounter, "%d", m_iACDepositedCounter );

	memset( l_szTotalAmount, 0, sizeof(l_szTotalAmount) );
	sprintf( l_szTotalAmount, "%d", m_iACDepositedAmount );
	InsertDataToNotesList( "CAS0", "Total", l_szTotalTimes, l_szTotalCounter, l_szTotalAmount );
}

//得到AC存款细节信息
int CLogAnalysisToolDlg::GetACDepositDetail( char* l_szFileName, char* l_szKeyWord )
{
	wchar_t l_szwLineData[2048]={0};
	char	l_szLineData[2048]={0};
	char*   l_pcLineData = NULL;
	long	l_lOcc=0;
	
	if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
	{
		try
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
				return 0;
			delete[] l_pwcFileName;//释放动态分配的内存（如果不释放内存将飙涨）
			
			char*	pstr=NULL;
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
				pstr = NULL;
				pstr = strstr(l_pcLineData, l_szKeyWord ); 
				
				if( pstr != NULL  && strlen(pstr+strlen(l_szKeyWord)) > 2)
				{
					ParseNoteInfo( pstr+strlen(l_szKeyWord) );
					l_lOcc ++;
				}
				delete[] l_pcLineData;//释放动态分配的内存（如果不释放内存将飙涨）
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	} 
	else//当文本为Ansi编码
	{
		try
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
				return 0;
			
			char*	pstr=NULL;
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
			{
				pstr = NULL;
				pstr = strstr(l_szLineData, l_szKeyWord ); 
				
				if( pstr != NULL && strlen(pstr+strlen(l_szKeyWord)) > 2)
				{
					ParseNoteInfo( pstr+strlen(l_szKeyWord) );
					l_lOcc ++;
				}
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	}

	return l_lOcc;
}

//根据传入的钞票日志记录，将数据放到结构体
void CLogAnalysisToolDlg::ParseNoteInfo( char *szLogsMessage )
{
	if( strlen(szLogsMessage) == 0 )
		return;
	char *pstr=szLogsMessage;
	char szTemp[256]={0};
	_NotesDef l_tNoteInfo;
	
	for( int i=0; i<strlen(szLogsMessage); i++ )
	{
		if( (*pstr) == NULL )
			break;
		memset( &l_tNoteInfo, 0, sizeof(l_tNoteInfo) );
		if( *pstr == '[' )
		{
			int j=0;
			pstr ++;
			for( pstr; pstr[j] != ' '&& pstr[j] != 0; j++ )
				szTemp[j] = pstr[j];
			pstr += j+1;
			
			if( strlen(szTemp) != 0 )	//币种
			{
				strcpy( l_tNoteInfo.szCur, szTemp );
			}
			
			memset( szTemp, 0, sizeof(szTemp) );
			for( j=0; pstr[j] != ']' && pstr[j] != 0; j++ )
				szTemp[j] = pstr[j];
			if( strlen(szTemp) != 0 && strlen(l_tNoteInfo.szCur)!=0 )	//币种
			{
				l_tNoteInfo.iDeno = atoi( szTemp );
			}
			pstr += j+1;
			
			memset( szTemp, 0, sizeof(szTemp) );
			for( j=0; pstr[j] != ' ' && pstr[j] != 0; j++ )
				szTemp[j] = pstr[j];
			if( strlen(szTemp) != 0 && strlen(l_tNoteInfo.szCur)!=0 )	//币种
			{
				l_tNoteInfo.iCounter = atoi( szTemp );
			}
			pstr += j;
			
			AddToList( l_tNoteInfo );
		}
		else
		{
			pstr ++;
		}
	}
}

//将钞票结构体添加到列表
void CLogAnalysisToolDlg::AddToList( _NotesDef NotesDetail )
{
	int iIndex=0;
	if( NotesDetail.iDeno == 0 || strlen(NotesDetail.szCur) == 0 || NotesDetail.iCounter == 0 )
		return;
	
	BOOL bExist = FALSE;
	while(TRUE)
	{
		if( strlen( m_tNotesInfo[iIndex].szCur) == 0 || m_tNotesInfo[iIndex].iDeno == 0 )
		{
			//memcpy( m_tNotesInfo[iIndex], NotesDetail, sizeof(_NotesDef) );
			bExist = FALSE;
			break;
		}
		if( strcmp( m_tNotesInfo[iIndex].szCur, NotesDetail.szCur) == 0 &&
			m_tNotesInfo[iIndex].iDeno == NotesDetail.iDeno )
		{
			m_tNotesInfo[iIndex].iCounter += NotesDetail.iCounter;
			bExist = TRUE;
			break;
		}
		iIndex ++;
	}
	if( !bExist )
		memcpy( &(m_tNotesInfo[iIndex]), &NotesDetail, sizeof(_NotesDef) );
}

/*************************************************************************************************************************/
/*                                                        统计存款明细                                                   */
/*************************************************************************************************************************/
void CLogAnalysisToolDlg::StatCashDeposited()
{
	InsertDataToNotesList( "----Deposit----", "---------", "---------", "---------", "--------------------" );

	char l_szTitleList[512]={0};
	::GetPrivateProfileString( "Deposited", "CASIDs", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
	if( strlen(l_szTitleList) == 0 )
	{
		InsertDataToNotesList( "0", "", "", "", "" );
		return;
	}
	char l_szTemp[512]={0};
	char *pstr=l_szTitleList;
	
	int		iPacer=0;
	int		l_iDeno = 1;
	char	l_szKeyWord[512]={0};
	char	l_szComment[1024]={0};
	char	l_szTitle[128]={0};
	CString	l_strCasName = "";
	iPacer = ::GetPrivateProfileInt( "Deposited", "Pacer", 0, m_szConfigFileName );

	long	l_lTotalCounter	= 0;
	long	l_lTotalTimes	= 0;
	long	l_lTotalAmount	= 0;
	int     CasIndex = 1;
	while( TRUE )//循环统计五个钱箱的存款信息
	{
		if( pstr == NULL || *pstr == NULL || (pstr+1) == NULL || *(pstr+1) == NULL)
			break;
		memset( l_szTitle, 0, sizeof(l_szTitle) );
		int i=0;
		for( ; *pstr !=',' && *pstr != NULL; pstr++ )
		{
			l_szTitle[i++] = *pstr;
		}
		pstr ++;


		l_strCasName.Empty();//将钱箱ID转换为钱箱名字
		CString strTemp;
		strTemp.Format("%d", CasIndex);
		l_strCasName = "CAS" + strTemp;
	    CasIndex++;
		
		memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
		::GetPrivateProfileString( "Deposited", l_strCasName.GetBuffer(l_strCasName.GetLength()), NULL, l_szKeyWord, sizeof(l_szKeyWord), m_szConfigFileName );
		
		/****************************************/
		/*         如果没有，则分解             */
		/****************************************/
		int  iAllDaysCounter =0;
		long iAllDaysAmount = 0;
		int  iAllDaysTimes = 0;

		int iDailyCounter =0;
		int iDailyAmount = 0;
		int iDailyTimes = 0;
		char l_szDeno[32]={0};
		memset( l_szDeno, 0, sizeof(l_szDeno) );

		if(0 == strcmp(l_szTitle, "CAS0"))//添加CAS0的存款统计
		{
			CasIndex--;
			sprintf( l_szDeno, "/" );
			iAllDaysCounter = m_iACDepositedCounter;
			iAllDaysAmount = m_iACDepositedAmount;
			iAllDaysTimes = m_iACDepositedTimes;
		}//end if添加CAS0的存款统计
		else//添加CAS1，CAS2,CAS3，CAS4的存款统计
		{			
			memset( l_szTemp, 0, sizeof(l_szTemp) );
			sprintf( l_szTemp, "%s_DENO", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
			l_iDeno = ::GetPrivateProfileInt( "Deposited", l_szTemp, 1, m_szConfigFileName );
			
			vector<CString>::iterator iter; 
			for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//遍历所有打开的文件
			{
				char  l_szFileName[256];
				memset( l_szFileName, 0, sizeof(l_szFileName) );
				strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );

				iDailyCounter = GetCounter( l_szFileName, l_szKeyWord, iPacer, &iDailyTimes );
				iAllDaysCounter += iDailyCounter;
			    iAllDaysTimes	+= iDailyTimes;
			}

			memset( l_szTemp, 0, sizeof(l_szTemp) );
			sprintf( l_szTemp, "%s_DENO", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
			l_iDeno = ::GetPrivateProfileInt( "Deposited", l_szTemp, 1, m_szConfigFileName );
			iAllDaysAmount = iAllDaysCounter*l_iDeno;
			memset( l_szDeno, 0, sizeof(l_szDeno) );
			sprintf( l_szDeno, "%d", l_iDeno );

		}//end else添加CAS1，CAS2,CAS3，CAS4的存款统计

		memset( l_szTemp, 0, sizeof(l_szTemp) );//注释
		sprintf( l_szTemp, "%s_Comment", l_strCasName.GetBuffer(l_strCasName.GetLength()) );
		memset( l_szComment, 0, sizeof(l_szComment) );
		::GetPrivateProfileString( "Deposited", l_szTemp, NULL, l_szComment, sizeof(l_szComment), m_szConfigFileName );
		
		memset( l_szTemp, 0, sizeof(l_szTemp) );//总数
		sprintf( l_szTemp, "%d", iAllDaysCounter );
		
		char l_szTemp2[32]={0};
		memset( l_szTemp2, 0, sizeof(l_szTemp2) );//总量
		sprintf( l_szTemp2, "%d", iAllDaysAmount );
		
		char l_szTimes[32]={0};
		memset( l_szTimes, 0, sizeof(l_szTimes) );//次数
		sprintf( l_szTimes, "%d", iAllDaysTimes );
		
		InsertDataToNotesList( l_szTitle, l_szDeno, l_szTimes, l_szTemp, l_szTemp2 );//插入记录

		l_lTotalCounter += iAllDaysCounter;
		l_lTotalAmount	+= iAllDaysAmount;
		l_lTotalTimes	+= iAllDaysTimes;
	}//end while循环统计五个钱箱的存款信息

	memset( l_szTemp, 0, sizeof(l_szTemp) );//计算Total数据
	sprintf( l_szTemp, "%d", l_lTotalCounter );
	
	char l_szTemp2[32]={0};
	memset( l_szTemp2, 0, sizeof(l_szTemp2) );
	sprintf( l_szTemp2, "%d", l_lTotalAmount );
	
	char l_szTimes[32]={0};
	memset( l_szTimes, 0, sizeof(l_szTimes) );
	sprintf( l_szTimes, "%d", l_lTotalTimes );
	
	InsertDataToNotesList( "Total", "/", l_szTimes, l_szTemp, l_szTemp2 );

	//统计存款拒钞信息
	int l_iTotalRefuseCounter = 0;
	int iRefuseTimes = 0;
	int l_iTotalRefuseTimes = 0;
	memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
	::GetPrivateProfileString( "Deposited", "DepositRefused", NULL, l_szKeyWord, sizeof(l_szKeyWord), m_szConfigFileName );
	iPacer = 0;
	vector<CString>::iterator iter; 
	for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//遍历所有打开的文件
	{
		char  l_szFileName[256];
		memset( l_szFileName, 0, sizeof(l_szFileName) );
		strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
		
		iRefuseTimes = 0;
		int iRefuseCounter = GetCounter( l_szFileName, l_szKeyWord, iPacer, &iRefuseTimes );
		l_iTotalRefuseCounter += iRefuseCounter;
		l_iTotalRefuseTimes	+= iRefuseTimes;
	}

	InsertDataToNotesList( "-DepositRefuse-", "---------", "---------", "---------", "--------------------" );
	char l_szTimesTemp[32]={0};
	memset( l_szTimesTemp, 0, sizeof(l_szTimesTemp) );//总数
	sprintf( l_szTimesTemp, "%d", l_iTotalRefuseTimes );
	
	char l_szCounterTemp[32]={0};
	memset( l_szCounterTemp, 0, sizeof(l_szCounterTemp) );//总量
	sprintf( l_szCounterTemp, "%d", l_iTotalRefuseCounter );

	InsertDataToNotesList( "CAS0", "/", l_szTimesTemp, l_szCounterTemp, "/" );//插入记录
}

/*************************************************************************************************************************/
/*                                                        统计模式信息                                                   */
/*************************************************************************************************************************/
void CLogAnalysisToolDlg::StatMode()
{
	char l_szTitleList[512]={0};
	::GetPrivateProfileString( "Mode", "KEYWORD", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
	if( strlen(l_szTitleList) == 0 )
	{
		//InsertDataToNotesList( "0", "", "", "", "" );
		return;
	}

	int  l_iSumTime = 0;
	char l_szKeyWord[128]={0};
	char *pstr=l_szTitleList;
	int  sumTime[5] = {0};
	COleDateTimeSpan sumModeSpan[5] = {0};
	int  l_iSumIndex = 0;

	while( TRUE )//多个搜索关键字循环
	{
		if( pstr == NULL || *pstr == NULL )
			break;
		memset( l_szKeyWord, 0, sizeof(l_szKeyWord) );
		int i=0;
		for( ; *pstr !=',' && *pstr != NULL; pstr++ )
		{
			l_szKeyWord[i++] = *pstr;
		}
		pstr ++;
		
		//遍历所有文件
		vector<CString>::iterator iter; 
		for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)
		{
			char  l_szFileName[256];
			memset( l_szFileName, 0, sizeof(l_szFileName) );
			strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
			int l_iFileFlag = 0;
			bool l_bFirstFile = false;
			bool l_bFinalFile = false;
			if (m_vecFilename.begin() == iter)
			{
				l_bFirstFile = true;//第一个文件特殊处理
			}
			if (m_vecFilename.end() == (iter+1))
			{
				l_bFinalFile = true;//最后一个文件特殊处理
			}

			//文件存在时间间隔时，去掉间隔时间
			vector<CString>::iterator vecGapLeft_iter; 
		    for (vecGapLeft_iter=m_vecGapLeftFilename.begin();vecGapLeft_iter!=m_vecGapLeftFilename.end();vecGapLeft_iter++)//遍历所有打开的文件
			{
				if (strstr(l_szFileName, vecGapLeft_iter->GetBuffer(vecGapLeft_iter->GetLength())))
				{
					l_bFinalFile = true;//间隔的左侧作为最后一个文件处理
				}
			}

			vector<CString>::iterator vecGapRight_iter; 
			for (vecGapRight_iter=m_vecGapRightFilename.begin();vecGapRight_iter!=m_vecGapRightFilename.end();vecGapRight_iter++)//遍历所有打开的文件
			{
				if (strstr(l_szFileName, vecGapRight_iter->GetBuffer(vecGapRight_iter->GetLength())))
				{
					l_bFirstFile = true;//间隔的右侧作为第一个文件处理
				}
			}

			int iResult = GetModeOccurrencesTime( l_szFileName, l_szKeyWord, l_bFirstFile, l_bFinalFile);//统计模式关键字频率及时间
			l_iSumTime += iResult;
		}//end for  遍历所有文件结束
		sumTime[l_iSumIndex] = l_iSumTime;//保存该模式频率与时间
		sumModeSpan[l_iSumIndex] = m_sumTimeSpan;
		l_iSumIndex++;
		
		//完成一次关键字搜索变量复位
		memset( &m_LastFileSearchBeginTime, 0, sizeof(m_LastFileSearchBeginTime) );//上个文件最后一次搜索开始时间
		m_bFindKeyWord = false;//模式跨越文件
		memset( &m_sumTimeSpan, 0, sizeof(m_sumTimeSpan) );//模式总时间
	}//while end搜索多个模式关键字结束

	//向列表框插入第一行Frequency数据
	LVITEM	lvitem;
	int		iPos;
	char    l_szSumTemp[126] = {0};
	char    l_szTimeSpan[126] = {0};
	lvitem.mask         =LVIF_TEXT;//第一列不能居中，隐藏
	lvitem.iItem		= 0;
	lvitem.iSubItem		=0;
	lvitem.pszText		="";
	lvitem.lParam		=0;
	iPos=m_hWndListCtrol3->InsertItem(&lvitem);

	lvitem.mask         =LVIF_TEXT;//第二列开始
	lvitem.iItem		=0;
	lvitem.iSubItem		=1;
	lvitem.pszText		="Frequency";
	m_hWndListCtrol3->SetItem(&lvitem);

	for (int i = 0; i<l_iSumIndex; i++)
	{
		lvitem.mask         =LVIF_TEXT;
		lvitem.iItem		=0;
		lvitem.iSubItem		=i+2;
		memset( l_szSumTemp, 0, sizeof(l_szSumTemp) );
		sprintf( l_szSumTemp, "%d", sumTime[i]);
		lvitem.pszText		=l_szSumTemp;
		m_hWndListCtrol3->SetItem(&lvitem);
	}

	//插入第二行Spans数据
	lvitem.mask         =LVIF_TEXT;//第一列不能居中，隐藏
	lvitem.iItem		= 1;
	lvitem.iSubItem		=0;
	lvitem.pszText		="";
	lvitem.lParam		=1;
	iPos=m_hWndListCtrol3->InsertItem(&lvitem);

	lvitem.mask         =LVIF_TEXT;//第二列开始
	lvitem.iItem		=1;
	lvitem.iSubItem		=1;
	lvitem.pszText		="Spans";
	m_hWndListCtrol3->SetItem(&lvitem);

	for (i = 0; i<l_iSumIndex; i++)
	{
		lvitem.mask         =LVIF_TEXT;
		lvitem.iItem		=1;
		lvitem.iSubItem		=i+2;
		memset( l_szTimeSpan, 0, sizeof(l_szTimeSpan) );
		sprintf(l_szTimeSpan,"%d d %d h %d m", sumModeSpan[i].GetDays(),sumModeSpan[i].GetHours(),sumModeSpan[i].GetMinutes());
		if (0 == i)
		{
			strcpy(l_szTimeSpan, "/");
		}
		lvitem.pszText		=l_szTimeSpan;
		m_hWndListCtrol3->SetItem(&lvitem);
	}
}

//获得当前行的时间
int	CLogAnalysisToolDlg::GetLineTime( char* l_pstr, COleDateTime& CurTime)
{
	char	l_szBeginTime[20]={0};
	char*   l_pTempStr = NULL;
	l_pTempStr = strstr(l_pstr, "20");
	if (l_pTempStr != NULL && strlen(l_pTempStr) > 19 && l_pTempStr[4] == '-')//当为有效时间格式
	{
		memcpy(l_szBeginTime, l_pTempStr, 19);
		l_szBeginTime[19] = '\0';
		CString sTime(l_szBeginTime);  
        CurTime.ParseDateTime(sTime);
		return 0;
	}
	else//当为无效时间
	{
		return -1;
	}
    
}



int	CLogAnalysisToolDlg::GetModeOccurrencesTime( char* l_szFileName, char* l_szKeyWord, int l_bFirstFile = 0, int l_bFinalFile = 0)
{
	wchar_t l_szwLineData[2048]={0};
	char	l_szLineData[2048]={0};
	char*   l_pcLineData = NULL;
	long	l_lOcc=0;
	COleDateTime l_beginTime;//搜索到l_szKeyWord模式关键字的时间
	COleDateTime l_fileEndTime;//搜索到l_szKeyWord模式关键字的时间

	if (m_bFindKeyWord)//当一个模式跨越了两个文件
	{
		l_beginTime = m_LastFileSearchBeginTime;
	}

	if (m_oFunc.IsUnicode(l_szFileName))//Unicode文件编码文件处理
	{
		try
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
				return 0;
			delete[] l_pwcFileName;//释放动态分配的内存
		
			char*	pstr=NULL; 
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )//循环按行读取文件
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
				pstr = NULL;
				pstr = strstr(l_pcLineData, l_szKeyWord );

				if (l_bFirstFile && strstr(l_szKeyWord, "INSERVICE"))//将第一个文件开始部分算为inservice模式时间统计
				{
					memset( &l_beginTime, 0, sizeof(l_beginTime) );
					if (0 == GetLineTime( l_pcLineData, l_beginTime))//第一个文件开始时间（只记录一次）
					{
						m_bFindKeyWord = true;
					    l_bFirstFile = false;//复位
					}
					else
					{
						continue;//无效时间
					}

				}

				if (l_bFinalFile)//当读最后一个文件时记录文件末尾时间
				{
					COleDateTime l_TempTime;//搜索到l_szKeyWord模式关键字的时间
					if (0 == GetLineTime( l_pcLineData, l_TempTime))//返回有效时间
					{
						memset( &l_fileEndTime, 0, sizeof(l_fileEndTime) );
						l_fileEndTime = l_TempTime;
					}
					else
					{
						continue;//无效时间
					}
				}
		
				if( pstr != NULL )//搜到该模式关键字，统计频率并记录开始时间
				{
					l_lOcc ++;
					memset( &l_beginTime, 0, sizeof(l_beginTime));
					if (0 == GetLineTime( l_pcLineData, l_beginTime))//模式开始有效时间
					{
						m_bFindKeyWord = true;
					}
					else
					{
						continue;
					}
					
				}
				else if(m_bFindKeyWord)//通过搜索其他模式关键字，统计该模式总时间
				{
					char l_szTitleList[512]={0};
					::GetPrivateProfileString( "Mode", "KEYWORD", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
					if( strlen(l_szTitleList) == 0 )
					{
						continue;
					}
					
					char otherKeyWord[128]={0};
					char *pstrTitleList=l_szTitleList;
					char *pstrOther = NULL;
					while( TRUE )//五个模式关键字的搜索循环
					{	
						if( pstrTitleList == NULL || *pstrTitleList == NULL )
			              break;
						memset( otherKeyWord, 0, sizeof(otherKeyWord) );
						int i=0;
					    for( ; *pstrTitleList !=',' && *pstrTitleList != NULL; pstrTitleList++ )//得到keyWord
						{
							otherKeyWord[i++] = *pstrTitleList;
						}
	                	pstrTitleList ++;

						pstrOther = strstr(l_pcLineData, otherKeyWord ); //重复搜索l_szKeyWord，不过为了简化代码没关系

						if (pstrOther != NULL)//找到其他模式关键字，该模式时间统计结束
						{
							COleDateTime l_EndTime;
							if (0 == GetLineTime( l_pcLineData, l_EndTime))//有效终止时间
							{
								COleDateTimeSpan timeSpan = l_EndTime - l_beginTime;
								if (l_EndTime > l_beginTime)
								{
									m_sumTimeSpan += timeSpan;//该模式总时间段
								}
								m_bFindKeyWord = false;
							}	
							break;
						}
					}//while end搜索其他模式关键字结束
				}
				delete[] l_pcLineData;//释放动态分配的内存

			}//while end循环按行读取文件结束

			if (m_bFindKeyWord && !l_bFinalFile)//该模式跨越前后两个文件
			{
				m_LastFileSearchBeginTime = l_beginTime;
			}

			if (m_bFindKeyWord && l_bFinalFile)//最后一个文件结尾时，时间处理
			{
				COleDateTimeSpan timeSpan = l_fileEndTime - l_beginTime;
				if (l_fileEndTime > l_beginTime)
				{
					m_sumTimeSpan += timeSpan;//计算该模式总的时间段
				}
				m_bFindKeyWord = false;
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	} //end if Unicode文件编码文件处理结束
	else//Ansi编码处理
	{
		try
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
				return 0;
			char*	pstr=NULL; 
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )//循环按行读取文件
			{
				pstr = NULL;
				pstr = strstr(l_szLineData, l_szKeyWord );

				if (l_bFirstFile && strstr(l_szKeyWord, "INSERVICE"))//将第一个文件开始部分算为inservice模式时间统计
				{
					memset( &l_beginTime, 0, sizeof(l_beginTime) );
					if (0 == GetLineTime( l_szLineData, l_beginTime))//第一个文件开始时间（只记录一次）
					{
						m_bFindKeyWord = true;
					    l_bFirstFile = false;//复位
					}
					else
					{
						continue;//无效时间
					}

				}

				if (l_bFinalFile)//当读最后一个文件时记录文件末尾时间
				{
					COleDateTime l_TempTime;//搜索到l_szKeyWord模式关键字的时间
					if (0 == GetLineTime( l_szLineData, l_TempTime))//返回有效时间
					{
						memset( &l_fileEndTime, 0, sizeof(l_fileEndTime) );
						l_fileEndTime = l_TempTime;
					}
					else
					{
						continue;//无效时间
					}
				}
		
				if( pstr != NULL )//搜到该模式关键字，统计频率并记录开始时间
				{
					l_lOcc ++;
					memset( &l_beginTime, 0, sizeof(l_beginTime));
					if (0 == GetLineTime( l_szLineData, l_beginTime))//模式开始有效时间
					{
						m_bFindKeyWord = true;
					}
					else
					{
						continue;
					}
					
				}
				else if(m_bFindKeyWord)//通过搜索其他模式关键字，统计该模式总时间
				{
					char l_szTitleList[512]={0};
					::GetPrivateProfileString( "Mode", "KEYWORD", "", l_szTitleList, sizeof(l_szTitleList), m_szConfigFileName );
					if( strlen(l_szTitleList) == 0 )
					{
						continue;
					}
					
					char otherKeyWord[128]={0};
					char *pstrTitleList=l_szTitleList;
					char *pstrOther = NULL;
					while( TRUE )//五个模式关键字的搜索循环
					{	
						if( pstrTitleList == NULL || *pstrTitleList == NULL )
			              break;
						memset( otherKeyWord, 0, sizeof(otherKeyWord) );
						int i=0;
					    for( ; *pstrTitleList !=',' && *pstrTitleList != NULL; pstrTitleList++ )//得到keyWord
						{
							otherKeyWord[i++] = *pstrTitleList;
						}
	                	pstrTitleList ++;

						pstrOther = strstr(l_szLineData, otherKeyWord ); //重复搜索l_szKeyWord，不过为了简化代码没关系

						if (pstrOther != NULL)//找到其他模式关键字，该模式时间统计结束
						{
							COleDateTime l_EndTime;
							if (0 == GetLineTime( l_szLineData, l_EndTime))//有效终止时间
							{
								COleDateTimeSpan timeSpan = l_EndTime - l_beginTime;
								if (l_EndTime > l_beginTime)
								{
									m_sumTimeSpan += timeSpan;//该模式总时间段
								}
								m_bFindKeyWord = false;
							}	
							break;
						}
					}//while end搜索其他模式关键字结束
				}

			}//while end循环按行读取文件结束

			if (m_bFindKeyWord && !l_bFinalFile)//该模式跨越前后两个文件
			{
				m_LastFileSearchBeginTime = l_beginTime;
			}

			if (m_bFindKeyWord && l_bFinalFile)//最后一个文件结尾时，时间处理
			{
				COleDateTimeSpan timeSpan = l_fileEndTime - l_beginTime;
				if (l_fileEndTime > l_beginTime)
				{
					m_sumTimeSpan += timeSpan;//计算该模式总的时间段
				}
				m_bFindKeyWord = false;
			}
			fclose( fp );
		}
		catch (...)
		{
			return 0;
		}
	}//end else Ansi文件编码文件处理
	
	return l_lOcc;
}

void CLogAnalysisToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TIMER)
	{
		progressNumber += 10;//更新进度条
		if (progressNumber > ProcessMaxValue)
		{
			progressNumber = 0;
		}
	    m_hWndProgressCtrl1->SetPos(progressNumber);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CLogAnalysisToolDlg::OnBtnExcelExport() 
{
	// TODO: Add your control notification handler code here
	if (m_bFinishProcessing)//交易分析完成可以导出Excel表格
	{
		CListCtrl* m_hWndListCtrol[4];
		m_hWndListCtrol[0] = m_hWndListCtrol1;
		m_hWndListCtrol[1] = m_hWndListCtrol2;
		m_hWndListCtrol[2] = m_hWndListCtrol3;
		m_hWndListCtrol[3] = m_hWndListCtrol4;
		CString l_strTableName[4];
		l_strTableName[0] = "TranscationAnalysis";
		l_strTableName[1] = "NoteInforAnalysis";
		l_strTableName[2] = "ModeAnalysis";
		//l_strTableName[3] = "CassetteStateAnalysis";
		if (m_bModeStat)
		{
			m_oFunc.ExportListToExcel(m_hWndListCtrol, l_strTableName, 3);
		} 
		else
		{
			m_oFunc.ExportListToExcel(m_hWndListCtrol, l_strTableName, 2);
		}
	}
}

struct  CASStateInfo
{
	char szName[10];
	int  iEmptyFreqency;
	int  iFullFeqency;
	char szPreState[10];
};

int	CLogAnalysisToolDlg::GetCassetteState()
{
	int CasStateInfoNumber = 0;
	CASStateInfo CasState[6];
	memset(CasState, 0, sizeof(CasState));
	char l_szKeyWord[10] = "CAS(";

	//遍历所有文件
	vector<CString>::iterator iter; 
	for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)//循环搜索多个文件
	{
		char  l_szFileName[256];
		memset( l_szFileName, 0, sizeof(l_szFileName) );
		strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );

		char	l_szTempData[256]={0};
		wchar_t l_szwLineData[2048]={0};
		char	l_szLineData[2048]={0};
		char*   l_pcLineData = NULL;
		long	l_lOcc=0;
		
		if (m_oFunc.IsUnicode(l_szFileName))//当文本为Unicode编码
		{
			wchar_t*  l_pwcFileName = NULL;
			l_pwcFileName = m_oFunc.AnsiToUnicode(l_szFileName);
			FILE*	fp=NULL;
			if( (fp=_wfopen(l_pwcFileName, L"rb")) == NULL )
			{
				continue;
			}
			delete[] l_pwcFileName;//释放动态分配的内存
				
			char*	pstr=NULL;
			while( fgetws(l_szwLineData, sizeof(l_szwLineData), fp) != NULL )
			{
				l_pcLineData = m_oFunc.UnicodToAnsi( l_szwLineData);
				pstr = NULL;
				pstr = strstr(l_pcLineData, l_szKeyWord ); //找到CAS(
					
				if( pstr != NULL && strlen(pstr) > 35)	//防止访问越界，防止"CAS(00050) EMPTY"这种情况造成的访问越界
				{
					try
					{
						char szCasName[10] = {0};
						char szCurState[10] = {0};
						int iIndex = 0;
						for (iIndex = 0; pstr[iIndex] != ':'; iIndex++)//得到钱箱名字
						{
							szCasName[iIndex] = pstr[iIndex];
						}
						szCasName[iIndex] = '\0';
						pstr = strstr(pstr, "/" );
						pstr++;
						pstr = strstr(pstr, "/" );
						pstr++;
						pstr = strstr(pstr, "/" );
						pstr++;
						for (iIndex = 0; pstr[iIndex] != '/'; iIndex++)//得到钱箱状态
						{								
							szCurState[iIndex] = pstr[iIndex];
						}
						szCurState[iIndex] = '\0';
						bool bAddCasStateInfo = true;
						for (iIndex = 0; iIndex < CasStateInfoNumber; iIndex++)
						{
							if (0 == strcmp(CasState[iIndex].szName, szCasName))//已存在钱箱状态节点
							{									
								bAddCasStateInfo = false;
									
								if (0 == strcmp(CasState[iIndex].szPreState, "NORMAL") &&
									0 == strcmp(szCurState, "FULL"))
								{
									CasState[iIndex].iFullFeqency++;//计算钱箱满次数
								}
									
								if ((0 == strcmp(CasState[iIndex].szPreState, "LOW") ||
									0 == strcmp(CasState[iIndex].szPreState, "NORMAL")) &&
									0 == strcmp(szCurState, "EMPTY"))
								{
									CasState[iIndex].iEmptyFreqency++;//计算钱箱空次数
								}
								strcpy(CasState[iIndex].szPreState, szCurState);//记录该次钱箱状态
									
							}
						}
							
						if (bAddCasStateInfo && CasStateInfoNumber < 6)
						{
							memcpy(CasState[CasStateInfoNumber].szName, szCasName, strlen(szCasName));
							memcpy(CasState[CasStateInfoNumber].szPreState, szCurState, strlen(szCurState));
							CasState[CasStateInfoNumber].iFullFeqency = 0;
							CasState[CasStateInfoNumber].iFullFeqency = 0;
							CasStateInfoNumber++;
						}
					}
					catch (...)
					{
						return 0;
					}
				}
					delete[] l_pcLineData;//释放动态分配的内存
			}
				fclose( fp );
		} //end if Unicode文件处理
		else//ANSI文件处理
		{
			FILE*	fp=NULL;
			if( (fp=fopen(l_szFileName, "r")) == NULL )
			{
				continue;
			}
			
			char*	pstr=NULL;
			while( fgets(l_szLineData, sizeof(l_szLineData), fp) != NULL )
			{
				pstr = NULL;
				pstr = strstr(l_szLineData, l_szKeyWord ); 
				
				if( pstr != NULL && strlen(pstr) > 35)	//找到出现CAS(
				{
					try
					{
						char szCasName[10] = {0};
						char szCurState[10] = {0};
						int iIndex = 0;
						for (iIndex = 0; pstr[iIndex] != ':'; iIndex++)//得到钱箱名字
						{
							szCasName[iIndex] = pstr[iIndex];
						}
						szCasName[iIndex] = '\0';
						pstr = strstr(pstr, "/" );
						pstr++;
						pstr = strstr(pstr, "/" );
						pstr++;
						pstr = strstr(pstr, "/" );
						pstr++;
						for (iIndex = 0; pstr[iIndex] != '/'; iIndex++)//得到钱箱状态
						{
							szCurState[iIndex] = pstr[iIndex];
						}
						szCurState[iIndex] = '\0';
						bool bAddCasStateInfo = true;
						for (iIndex = 0; iIndex < CasStateInfoNumber; iIndex++)
						{
							if (0 == strcmp(CasState[iIndex].szName, szCasName))//已存在钱箱状态节点
							{
								bAddCasStateInfo = false;
								
								if (0 == strcmp(CasState[iIndex].szPreState, "NORMAL") &&
									0 == strcmp(szCurState, "FULL"))
								{
									CasState[iIndex].iFullFeqency++;//计算钱箱满次数
								}
								
								if ((0 == strcmp(CasState[iIndex].szPreState, "LOW") ||
									0 == strcmp(CasState[iIndex].szPreState, "NORMAL")) &&
									0 == strcmp(szCurState, "EMPTY"))
								{
									CasState[iIndex].iEmptyFreqency++;//计算钱箱空次数
								}
								strcpy(CasState[iIndex].szPreState, szCurState);//记录该次钱箱状态
								
							}
						}
						
						if (bAddCasStateInfo && CasStateInfoNumber < 6)
						{
							memcpy(CasState[CasStateInfoNumber].szName, szCasName, strlen(szCasName));
							memcpy(CasState[CasStateInfoNumber].szPreState, szCurState, strlen(szCurState));
							CasState[CasStateInfoNumber].iFullFeqency = 0;
							CasState[CasStateInfoNumber].iFullFeqency = 0;
							CasStateInfoNumber++;
						}
					}
					catch (...)
					{
						return 0;
					}
					
				}
			}
			fclose( fp );
		}//end else ANSI文件处理
	}//end for循环搜索多个文件


	//向列表框插入第一行Frequency数据
	LVITEM	lvitem;
	int		iPos;
	char l_szTempFrequency[10] = {0};
	for (int iInfoIndex = 0; iInfoIndex < CasStateInfoNumber; iInfoIndex++)
	{
		lvitem.mask         =LVIF_TEXT;//第一列不能居中，隐藏
		lvitem.iItem		= iInfoIndex;
		lvitem.iSubItem		=0;
		lvitem.pszText		="";
		lvitem.lParam		=iInfoIndex;
		iPos=m_hWndListCtrol4->InsertItem(&lvitem);
		
		lvitem.mask         =LVIF_TEXT;//第二列开始
		lvitem.iItem		=iInfoIndex;
		lvitem.iSubItem		=1;
		lvitem.pszText		=CasState[iInfoIndex].szName;
		m_hWndListCtrol4->SetItem(&lvitem);
		
		lvitem.mask         =LVIF_TEXT;
		lvitem.iItem		=iInfoIndex;
		lvitem.iSubItem		=2;
		memset( l_szTempFrequency, 0, sizeof(l_szTempFrequency) );
		sprintf(l_szTempFrequency,"%d", CasState[iInfoIndex].iFullFeqency);
		lvitem.pszText		=l_szTempFrequency;
		m_hWndListCtrol4->SetItem(&lvitem);

		lvitem.mask         =LVIF_TEXT;
		lvitem.iItem		=iInfoIndex;
		lvitem.iSubItem		=3;
		memset( l_szTempFrequency, 0, sizeof(l_szTempFrequency) );
		sprintf(l_szTempFrequency,"%d", CasState[iInfoIndex].iEmptyFreqency);
		lvitem.pszText		=l_szTempFrequency;
		m_hWndListCtrol4->SetItem(&lvitem);
	}
	
	return 0;
}

void CLogAnalysisToolDlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here

	if( m_vecFilename.empty())//当未选中任何文件时，对话框提示
	{
		MessageBox( "Please select a file." );
		return;
	}



	int		l_iIndex=0;
	char	l_szTemp[128]={0};
	char	l_szName[256]={0};
	char	l_szKeyWord[256]={0};
	int     l_iSumResult[256]={0};//保存总的统计结果

	m_hWndListCtrol6->DeleteAllItems();
    GetDlgItem(IDC_EDIT1)->GetWindowText(l_szKeyWord, sizeof(l_szKeyWord));

	if( 0 == strlen(l_szKeyWord))//当未选中任何文件时，对话框提示
	{
		MessageBox( "Please input keyword." );
		return;
	}
	
	char	l_szFileName[256];//保存打开的文件名字
	vector<CString> l_vecFilePath;

	vector<CString>::iterator iter; 
	int     iWithdrwalIndex = 0;
	for (iter=m_vecFilename.begin();iter!=m_vecFilename.end();iter++)
	{
		memset( l_szFileName, 0, sizeof(l_szFileName) );
		strcpy( l_szFileName, iter->GetBuffer(iter->GetLength()) );
		int iResult = GetOccurrences( l_szFileName, l_szKeyWord );//得到日志文件中KeyWord出现的次数
		memset( l_szTemp, 0, sizeof(l_szTemp) );
		sprintf( l_szTemp, "%d", iResult );
		l_iSumResult[l_iIndex] += iResult;
		
		if (iResult > 0)
		{
			l_vecFilePath.push_back(*iter);
		}
		
	}
	

	LVITEM	lvitem;
	int		iPos;
	char l_szIndex[10] = {0};
	for (int iInfoIndex = 0; iInfoIndex < l_vecFilePath.size(); iInfoIndex++)
	{
		lvitem.mask         =LVIF_TEXT;//第一列不能居中，隐藏
		lvitem.iItem		= iInfoIndex;
		lvitem.iSubItem		=0;
		lvitem.pszText		="";
		lvitem.lParam		=iInfoIndex;
		iPos=m_hWndListCtrol6->InsertItem(&lvitem);
		
		lvitem.mask         =LVIF_TEXT;//第二列开始
		lvitem.iItem		=iInfoIndex;
		lvitem.iSubItem		=1;
		memset( l_szIndex, 0, sizeof(l_szIndex) );
		sprintf(l_szIndex,"%d", iInfoIndex+1);
		lvitem.pszText		=l_szIndex;
		m_hWndListCtrol6->SetItem(&lvitem);
		
		lvitem.mask         =LVIF_TEXT;
		lvitem.iItem		=iInfoIndex;
		lvitem.iSubItem		=2;
		lvitem.pszText		=l_vecFilePath[iInfoIndex].GetBuffer(l_vecFilePath[iInfoIndex].GetLength());
		m_hWndListCtrol6->SetItem(&lvitem);
		
	}
}

void CLogAnalysisToolDlg::OnDblclkList6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
	int nItem=pNMListView->iItem;
	if(nItem>=0 && nItem<m_hWndListCtrol6->GetItemCount())    //判断双击位置是否在有数据的列表项
	{
		TCHAR szBuf[1024];
		LVITEM lvi;
		lvi.iItem = nItem;
		lvi.iSubItem = 2;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuf;
		lvi.cchTextMax = 1024;
        m_hWndListCtrol6->GetItem(&lvi);
		try
		{
		    ShellExecute(NULL, _T("open"), szBuf, NULL, NULL, SW_SHOW);
		}
		catch (...)
		{
		}
	}
	
	*pResult = 0;
}
