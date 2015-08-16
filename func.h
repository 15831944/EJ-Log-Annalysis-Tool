// Func.h: interface for the CFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNC_H__B2D8353D_8818_4F34_B459_82D7E590D4D4__INCLUDED_)
#define AFX_FUNC_H__B2D8353D_8818_4F34_B459_82D7E590D4D4__INCLUDED_

#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXLINE					2048

class CFunc  
{
public:
	CFunc();
	virtual ~CFunc();
	
	int       TrimAll(char *Src);
	void      TrimLeft(char *Src);
	void      TrimRight(char *Src);
	int       MyInifetch(const char *file, const char *sect, const char *key, char *value);
    void	  UnicodToAnsi(const wchar_t *wchar, char *chr, int length);
	char*     UnicodToAnsi(wchar_t *wstr);
	wchar_t*  AnsiToUnicode(char *str);
	bool	  IsUnicode(char* fileName);

	//µ¼³öExcel
	CString            GetExcelDriver();
	BOOL		       MakeSurePathExists( CString &Path,
		bool FilenameIncluded);
	BOOL		       GetDefaultXlsFileName(CString& sExcelFile);
	void		       ExportListToExcel(CListCtrl* pList[], CString strTitle[], int number);
};

#endif // !defined(AFX_FUNC_H__B2D8353D_8818_4F34_B459_82D7E590D4D4__INCLUDED_)
