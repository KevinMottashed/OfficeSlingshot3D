// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8DA87143_D69F_476D_8838_9C9F90FCC5A2__INCLUDED_)
#define AFX_STDAFX_H__8DA87143_D69F_476D_8838_9C9F90FCC5A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//----------------------------------------------
// Windows Headers
//----------------------------------------------

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxole.h>
#include <afxmt.h>			// multi thread library
#include <Richole.h>


typedef DWORD DWORD_PTR;
typedef LONG LONG_PTR;

//----------------------------------------------
// Standard Template Library (STL)
//----------------------------------------------

// This is microsft's way of dealing with this warning in vc++6
// the warning occurs because the STL is used
#pragma warning (disable : 4786)

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

// renable the warning
#pragma warning (default : 4786)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__8DA87143_D69F_476D_8838_9C9F90FCC5A2__INCLUDED_)
