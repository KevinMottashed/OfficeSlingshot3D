// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef STD_AFX_H
#define STD_AFX_H

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
#include <stdio.h>
#include <vfw.h>

typedef DWORD DWORD_PTR;
typedef LONG LONG_PTR;

//----------------------------------------------
// Standard Template Library (STL)
//----------------------------------------------

// from some reason microsoft decided to deprecate certain algorithms
#pragma warning (disable : 4996)
#include <algorithm>
#pragma warning (default : 4996)

#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------
// Standard C Headers
//----------------------------------------------

#include <cassert>
#include <cstdlib>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif
 
