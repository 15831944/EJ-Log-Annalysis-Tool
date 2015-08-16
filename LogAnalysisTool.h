// LogAnalysisTool.h : main header file for the LOGANALYSISTOOL application
//

#if !defined(AFX_LOGANALYSISTOOL_H__B0EDB3D7_661A_4D41_A0C2_47AE8D4BFB10__INCLUDED_)
#define AFX_LOGANALYSISTOOL_H__B0EDB3D7_661A_4D41_A0C2_47AE8D4BFB10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogAnalysisToolApp:
// See LogAnalysisTool.cpp for the implementation of this class
//

class CLogAnalysisToolApp : public CWinApp
{
public:
	CLogAnalysisToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogAnalysisToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLogAnalysisToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGANALYSISTOOL_H__B0EDB3D7_661A_4D41_A0C2_47AE8D4BFB10__INCLUDED_)
