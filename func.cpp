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
	TrimLeft(Src);//去除Src左边与右边的空格
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
	if((Pt=strstr(Src,";"))!=0)  //去处Src中的注释
	{
		*Pt='\0';
	}
	return 1;
}

/******************************************************************
Function    : TrimLeft(Src)
Description : 去除字符串Src左边的空格
Input       : Src要处理的字符串
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
Description : 去除字符串Src左边的空格
Input       : Src要处理的字符串
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
		TrimAll(line);//休整该行
		if (*line == ';')   /* 如果读到注释语句 */
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
			if (*line == ';')   /* 如果读到注释语句 */
				continue; 
			if (*line == '[')
			{
				if(fclose(fp)==EOF)
				{
					printf("文件关闭失败002!");
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
            printf("文件关闭失败002!");
		}
		return 1;
	}
	else
	{   
		if(fclose(fp)==EOF)
		{
			printf("文件关闭失败002!");
		}
		return 0;
	}
}

// Unicod To ANSI转换
void CFunc::UnicodToAnsi(const wchar_t *wchar, char *chr, int length)  
{  
    WideCharToMultiByte( CP_ACP, 0, wchar, -1,  
        chr, length, NULL, NULL );  
}  

//ANSI转unicode  
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

//Unicode转ansi
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

//判断文本是否是Unicode
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
//名称：GetExcelDriver
//功能：获取ODBC中Excel驱动
/////////////////////////////////////////////////////////////////////////////
CString CFunc::GetExcelDriver()
{
    char szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    char *pszBuf = szBuf;
    CString sDriver;
	
    // 获取已安装驱动的名称(涵数在odbcinst.h里)
    if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
        return "";
    
    // 检索已安装的驱动是否有Excel...
    do
    {
        if (strstr(pszBuf, "Excel") != 0)
        {
            //发现 !
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
//	参数：
//		Path				路径
//		FilenameIncluded	路径是否包含文件名
//	返回值:
//		文件是否存在
//	说明:
//		判断Path文件(FilenameIncluded=true)是否存在,存在返回TURE，不存在返回FALSE
//		自动创建目录
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

//获得默认的文件名
BOOL CFunc::GetDefaultXlsFileName(CString& sExcelFile)
{
	///默认文件名：yyyymmddhhmmss.xls
	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format("%04d%02d%02d%02d%02d%02d",filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	
	sExcelFile =  timeStr + ".xls";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".xls",sExcelFile);
	CString title;
	CString strFilter;
	
	title = "导出";
	strFilter = "Excel文件(*.xls)";
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
			AfxMessageBox("覆盖文件时出错！");
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	void GetExcelDriver(CListCtrl* pList, CString strTitle)
//	参数：
//		pList		需要导出的List控件指针
//		strTitle	导出的数据表标题
//	说明:
//		导出CListCtrl控件的全部数据到Excel文件。Excel文件名由用户通过“另存为”
//		对话框输入指定。创建名为strTitle的工作表，将List控件内的所有数据（包括
//		列名和数据项）以文本的形式保存到Excel工作表中。保持行列关系。
//	
///////////////////////////////////////////////////////////////////////////////
void CFunc::ExportListToExcel(CListCtrl* pList[], CString strTitle[], int number)
{
	CString warningStr;
	bool l_bStart = true;
	for (int index = 0; index < number; index++)//判断交易分析是否完成
	{
		if (pList[index]->GetItemCount () == 0 || pList[index]->GetItemCount () < 0)
		{
			l_bStart = false;
		}
	}

	if (l_bStart) //遍历每一行的循环
	{	
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName;
		
		// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// 没有发现Excel驱动
			AfxMessageBox("You've not installed Excel!\nPlease install Excel first!");
			return;
		}
		
		///默认文件名
		if (!GetDefaultXlsFileName(sExcelFile))
			return;
		
		// 创建进行存取的字符串
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		database.OpenEx(sSql,CDatabase::noOdbcDialog);

		// 创建数据库 (既Excel表格文件)
		for (int tableIndex = 0; tableIndex < number; tableIndex++)//创建多个Excel表的循环
		{
			tableName = strTitle[tableIndex];
			// 创建表结构
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

			for(i=1;pList[tableIndex]->GetColumn(i,&columnData);i++)//第一列没有数据应跳过
			{
				if (i!=1)
				{
					sSql = sSql + ", " ;
					strH = strH + ", " ;
				}
				sSql = sSql + " " + columnData.pszText +" TEXT";//数据暂时全部是以文本类型写入Excel
				strH = strH + " " + columnData.pszText +" ";
			}
			columnName.ReleaseBuffer ();
			columnNum = i;
			
			sSql = "CREATE TABLE " + tableName + " ( " + sSql +  " ) ";
			database.ExecuteSQL(sSql);
			
			// 插入数据项
			int nItemIndex;
			for (nItemIndex=0;nItemIndex < pList[tableIndex]->GetItemCount(); nItemIndex++)
			{
				strV = "";
				for(i=1;i<columnNum;i++)//第一列没有数据应跳过
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
		}//end for创建多个Excel表的循环

		// 关闭数据库
		database.Close();
			
		warningStr.Format("Excel file is saved as %s!",sExcelFile);
		AfxMessageBox(warningStr);
	}//end for遍历每一行的循环
}