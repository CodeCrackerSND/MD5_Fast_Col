// MD5_Fast_Col.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMD5_Fast_ColApp:
// See MD5_Fast_Col.cpp for the implementation of this class
//

class CMD5_Fast_ColApp : public CWinApp
{
public:
	CMD5_Fast_ColApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMD5_Fast_ColApp theApp;