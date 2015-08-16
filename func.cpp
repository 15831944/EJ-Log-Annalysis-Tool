// Func.cpp: implementation of the CFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogAnalysisTool.h"
#include "Func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunc::CFunc()
{
	
}

CFunc::~CFunc()
{
	
}

int  CFunc::TrimAll(char *Src)
{
	char *Pt;
	char *pHead = Src;
	TrimLeft(Src);//ȥ��Src������ұߵĿո�
	TrimRight(Src);
	
    if((Pt=pHead=strstr(Src," "))==0)
		return 1;
	while(*pHead != '\x0a')
	{
		while ( *pHead == ' ' && *pHead != '\0')
		{
			pHead ++ ;
		}
		while (*pHead != '\0'  && *pHead != ' '&& *pHead!='\x0a')
		{
			*Pt = *pHead ;
			Pt++ ;
			pHead++ ;
		}
		*Pt = '\0' ;
	}
	if((Pt=strstr(Src,";"))!=0)  //ȥ��Src�е�ע��
	{
		*Pt='\0';
	}
	return 1;
}

/******************************************************************
Function    : TrimLeft(Src)
Description : ȥ���ַ���Src��ߵĿո�
Input       : SrcҪ������ַ���
*******************************************************************/
void CFunc::TrimLeft(char *Src)
{
	char *pHead = Src;
	
	while ( *pHead == ' ' && *pHead != '\0')
	{
		pHead ++ ;
	}
	while (*pHead != '\0')
	{
		*Src = *pHead ;
		Src++ ;
		pHead++ ;
	}
	*Src = '\0' ;
}
/******************************************************************
Function    : TrimRight(Src)
Description : ȥ���ַ���Src��ߵĿո�
Input       : SrcҪ������ַ���
*******************************************************************/
void CFunc::TrimRight(char *Src)
{
	int iStrLength = strlen(Src) ;
	char *pTail = Src + iStrLength -1 ;
	
	while ( * pTail == ' ')
	{
		pTail -- ;
	}
	*(pTail+1)= '\0' ;
}

int CFunc::MyInifetch(const char *file, const char *sect, const char *key, char *value)
{      
	FILE *fp;
	char line[MAXLINE];
	char *p;
	int len;
	
	if ((fp=fopen(file, "r")) == NULL)
		return 0;
	
	len = strlen(sect);
	memset(value, 0, sizeof(value));
	
	while ((p=fgets(line, MAXLINE, fp)) != NULL)
	{
		TrimAll(line);//��������
		if (*line == ';')   /* �������ע����� */
			continue; 
		if (*line != '[')
			continue;
		
		if (strncmp(&line[1], sect, len) == 0 && line[1+len] == ']')
			break;
	}
	if (p != NULL)
	{
		len = strlen(key);
		while (fgets(line, MAXLINE, fp) != NULL   )
		{   
			//TrimAll(line);
			//TrimLeft(line);
			if (*line == ';')   /* �������ע����� */
				continue; 
			if (*line == '[')
			{
				if(fclose(fp)==EOF)
				{
					printf("�ļ��ر�ʧ��002!");
				}
				return 0;
			}
			if (memcmp(line, key, len) == 0 && line[len] == '=')
			{  
				//TrimLeft(line+len+1);
				strcpy(value, line+len+1);
				if ((p=strrchr(value,'\x0a')) != NULL)
					*p='\0';
				if ((p=strrchr(value,'\x0d')) != NULL)
					*p='\0';
				break;
			}
		} 
		if(fclose(fp)==EOF)
		{
            printf("�ļ��ر�ʧ��002!");
		}
		return 1;
	}
	else
	{   
		if(fclose(fp)==EOF)
		{
			printf("�ļ��ر�ʧ��002!");
		}
		return 0;
	}
}

// Unicod To ANSIת��
void CFunc::UnicodToAnsi(const wchar_t *wchar, char *chr, int length)  
{  
    WideCharToMultiByte( CP_ACP, 0, wchar, -1,  
        chr, length, NULL, NULL );  
}  

//ANSIתunicode  
wchar_t* CFunc::AnsiToUnicode(char *str)  
{  
    DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);  
    wchar_t *pwText;  
    pwText = new wchar_t[dwNum];  
    if(!pwText)  
    {  
        delete []pwText;  
    }  
    MultiByteToWideChar (CP_ACP, 0, str, -1, pwText, dwNum);  
    return pwText;  
}

//Unicodeתansi
char* CFunc::UnicodToAnsi(wchar_t *wstr)  
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wstr,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum];
	if(!psText)
	{
		delete []psText;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,wstr,-1,psText,dwNum,NULL,FALSE);
    return psText; 
}

//�ж��ı��Ƿ���Unicode
bool CFunc::IsUnicode(char* fileName)
{
	FILE *file = fopen(fileName, "r");
	if (NULL == file)
	{
		return false;
	}
	
	char fileBuf[8] = {0};
	size_t st = sizeof(fileBuf);
	fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
	fclose(file);
	if ( (char)fileBuf[0] == (char)0xff ) //check the first byte
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
//���ƣ�GetExcelDriver
//���ܣ���ȡODBC��Excel����
/////////////////////////////////////////////////////////////////////////////
CString CFunc::GetExcelDriver()
{
    char szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    char *pszBuf = szBuf;
    CString sDriver;
	
    // ��ȡ�Ѱ�װ����������(������odbcinst.h��)
    if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
        return "";
    
    // �����Ѱ�װ�������Ƿ���Excel...
    do
    {
        if (strstr(pszBuf, "Excel") != 0)
        {
            //���� !
            sDriver = CString(pszBuf);
            break;
        }
        pszBuf = strchr(pszBuf, '\0') + 1;
    }
    while (pszBuf[1] != '\0');
	
    return sDriver;
}

///////////////////////////////////////////////////////////////////////////////
//	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded)
//	������
//		Path				·��
//		FilenameIncluded	·���Ƿ�����ļ���
//	����ֵ:
//		�ļ��Ƿ����
//	˵��:
//		�ж�Path�ļ�(FilenameIncluded=true)�Ƿ����,���ڷ���TURE�������ڷ���FALSE
//		�Զ�����Ŀ¼
//
///////////////////////////////////////////////////////////////////////////////
BOOL CFunc::MakeSurePathExists( CString &Path,
						bool FilenameIncluded)
{
	int Pos=0;
	while((Pos=Path.Find('\\',Pos+1))!=-1)
		CreateDirectory(Path.Left(Pos),NULL);
	if(!FilenameIncluded)
		CreateDirectory(Path,NULL);
	//	return ((!FilenameIncluded)?!_access(Path,0):
	//	!_access(Path.Left(Path.ReverseFind('\\')),0));
	
	return !_access(Path,0);
}

//���Ĭ�ϵ��ļ���
BOOL CFunc::GetDefaultXlsFileName(CString& sExcelFile)
{
	///Ĭ���ļ�����yyyymmddhhmmss.xls
	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm�·�
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm����
	filesec=day.GetSecond();//ss
	timeStr.Format("%04d%02d%02d%02d%02d%02d",filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	
	sExcelFile =  timeStr + ".xls";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".xls",sExcelFile);
	CString title;
	CString strFilter;
	
	title = "����";
	strFilter = "Excel�ļ�(*.xls)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.xls");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	
	if (dlgFile.DoModal()==IDCANCEL)
		return FALSE; // open cancelled
	sExcelFile.ReleaseBuffer();
	if (MakeSurePathExists(sExcelFile,true)) {
		if(!DeleteFile(sExcelFile)) {    // delete the file
			AfxMessageBox("�����ļ�ʱ����");
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	void GetExcelDriver(CListCtrl* pList, CString strTitle)
//	������
//		pList		��Ҫ������List�ؼ�ָ��
//		strTitle	���������ݱ����
//	˵��:
//		����CListCtrl�ؼ���ȫ�����ݵ�Excel�ļ���Excel�ļ������û�ͨ�������Ϊ��
//		�Ի�������ָ����������ΪstrTitle�Ĺ�������List�ؼ��ڵ��������ݣ�����
//		��������������ı�����ʽ���浽Excel�������С��������й�ϵ��
//	
///////////////////////////////////////////////////////////////////////////////
void CFunc::ExportListToExcel(CListCtrl* pList[], CString strTitle[], int number)
{
	CString warningStr;
	bool l_bStart = true;
	for (int index = 0; index < number; index++)//�жϽ��׷����Ƿ����
	{
		if (pList[index]->GetItemCount () == 0 || pList[index]->GetItemCount () < 0)
		{
			l_bStart = false;
		}
	}

	if (l_bStart) //����ÿһ�е�ѭ��
	{	
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName;
		
		// �����Ƿ�װ��Excel���� "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// û�з���Excel����
			AfxMessageBox("You've not installed Excel!\nPlease install Excel first!");
			return;
		}
		
		///Ĭ���ļ���
		if (!GetDefaultXlsFileName(sExcelFile))
			return;
		
		// �������д�ȡ���ַ���
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		database.OpenEx(sSql,CDatabase::noOdbcDialog);

		// �������ݿ� (��Excel����ļ�)
		for (int tableIndex = 0; tableIndex < number; tableIndex++)//�������Excel���ѭ��
		{
			tableName = strTitle[tableIndex];
			// ������ṹ
			int i;
			LVCOLUMN columnData;
			CString columnName;
			int columnNum = 0;
			CString strH;
			CString strV;
			
			sSql = "";
			strH = "";
			columnData.mask = LVCF_TEXT;
			columnData.cchTextMax =100;
			columnData.pszText = columnName.GetBuffer (100);

			for(i=1;pList[tableIndex]->GetColumn(i,&columnData);i++)//��һ��û������Ӧ����
			{
				if (i!=1)
				{
					sSql = sSql + ", " ;
					strH = strH + ", " ;
				}
				sSql = sSql + " " + columnData.pszText +" TEXT";//������ʱȫ�������ı�����д��Excel
				strH = strH + " " + columnData.pszText +" ";
			}
			columnName.ReleaseBuffer ();
			columnNum = i;
			
			sSql = "CREATE TABLE " + tableName + " ( " + sSql +  " ) ";
			database.ExecuteSQL(sSql);
			
			// ����������
			int nItemIndex;
			for (nItemIndex=0;nItemIndex < pList[tableIndex]->GetItemCount(); nItemIndex++)
			{
				strV = "";
				for(i=1;i<columnNum;i++)//��һ��û������Ӧ����
				{
					if (i!=1)
					{
						strV = strV + ", " ;
					}
					strV = strV + " '" + pList[tableIndex]->GetItemText(nItemIndex,i) +"' ";
				}
				
				sSql = "INSERT INTO "+ tableName 
					+" ("+ strH + ")"
					+" VALUES("+ strV + ")";
				database.ExecuteSQL(sSql);
				sSql.Empty();
			}
		}//end for�������Excel���ѭ��

		// �ر����ݿ�
		database.Close();
			
		warningStr.Format("Excel file is saved as %s!",sExcelFile);
		AfxMessageBox(warningStr);
	}//end for����ÿһ�е�ѭ��
}