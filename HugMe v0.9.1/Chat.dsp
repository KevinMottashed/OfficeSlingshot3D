# Microsoft Developer Studio Project File - Name="Chat" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Chat - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chat.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chat.mak" CFG="Chat - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chat - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Chat - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chat - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "include\ARToolKit-2.72.1" /I "include\chai3d" /I "include/Z-Cam" /I "include/HandTracker" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 strmiids.lib opengl32.lib glu32.lib glut32.lib libAR.lib libARvideo.lib libARgsub_MFC.lib vfw32.lib chai3d_complete.lib winmm.lib hdl.lib ConnectDM_VC80.lib tinyxmld.lib depthCameraLibrary2.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcd" /libpath:"lib" /libpath:"lib\ARToolKit-2.72.1" /libpath:"lib\chai3d\msvc6" /libpath:"lib/Z-Cam" /libpath:"lib/HandTracker"

!ELSEIF  "$(CFG)" == "Chat - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /I "include\ARToolKit-2.72.1" /I "..\chai3d[v1.61.1]\include" /I "include\Z-Cam" /I "include\HandTracker" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MSVC" /D "_AFXDLL" /Fr /FD /D /NODEFAULTLIB:library" /GZ " /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 strmiids.lib opengl32.lib glu32.lib glut32.lib libARd.lib libARvideod.lib libARgsub_MFCd.lib vfw32.lib chai3d_complete.lib winmm.lib hdl.lib ConnectDM_VC80.lib tinyxmld.lib TDVHandsTracking.lib depthCameraLibrary2.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"lib\ARToolKit-2.72.1" /libpath:"..\chai3d[v1.61.1]\lib\msvc6" /libpath:"lib/Z-Cam" /libpath:"lib" /libpath:"lib\HandTracker"

!ENDIF 

# Begin Target

# Name "Chat - Win32 Release"
# Name "Chat - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ART.cpp
# End Source File
# Begin Source File

SOURCE=.\CellphoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CellphoneSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFinger3dofPointer.cpp
# End Source File
# Begin Source File

SOURCE=.\CFingerDevices.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.rc
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\DepthCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\DepthImageComp.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceArmband.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceFinger.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceJacket.cpp
# End Source File
# Begin Source File

SOURCE=.\EmoticonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HugMe.cpp
# End Source File
# Begin Source File

SOURCE=.\HumanModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TactileArray.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInterfaceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualPadDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ART.h
# End Source File
# Begin Source File

SOURCE=.\CellphoneDlg.h
# End Source File
# Begin Source File

SOURCE=.\CellphoneSocket.h
# End Source File
# Begin Source File

SOURCE=.\CFinger3dofPointer.h
# End Source File
# Begin Source File

SOURCE=.\CFingerDevices.h
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChatPacket.h
# End Source File
# Begin Source File

SOURCE=.\ChatSocket.h
# End Source File
# Begin Source File

SOURCE=.\ConnectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Controller.h
# End Source File
# Begin Source File

SOURCE=.\DepthImageComp.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceArmband.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceFinger.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDeviceJacket.h
# End Source File
# Begin Source File

SOURCE=.\EmoticonDlg.h
# End Source File
# Begin Source File

SOURCE=.\HugMe.h
# End Source File
# Begin Source File

SOURCE=.\HumanModel.h
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.h
# End Source File
# Begin Source File

SOURCE=.\NetworkCodes.h
# End Source File
# Begin Source File

SOURCE=.\NetworkManager.h
# End Source File
# Begin Source File

SOURCE=.\NetworkSocket.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Pattern.h
# End Source File
# Begin Source File

SOURCE=.\PatternDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\StaticCanvas.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Sync.h
# End Source File
# Begin Source File

SOURCE=.\TactileArray.h
# End Source File
# Begin Source File

SOURCE=.\UserInterfaceManager.h
# End Source File
# Begin Source File

SOURCE=.\VirtualPadDlg.h
# End Source File
# Begin Source File

SOURCE="..\chai3d[v1.61.1]\src\devices\ZCAM_HEADER1.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Chat.ico
# End Source File
# Begin Source File

SOURCE=.\res\Chat.rc2
# End Source File
# Begin Source File

SOURCE=.\res\emoticon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\emoticon_sel.bmp
# End Source File
# End Group
# Begin Group "encoder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\encoder\coder.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\coder.h
# End Source File
# Begin Source File

SOURCE=.\encoder\config.h
# End Source File
# Begin Source File

SOURCE=.\encoder\countbit.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\countbit.h
# End Source File
# Begin Source File

SOURCE=.\encoder\ctables.h
# End Source File
# Begin Source File

SOURCE=.\encoder\dct.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\dct.h
# End Source File
# Begin Source File

SOURCE=.\encoder\def.h
# End Source File
# Begin Source File

SOURCE=.\encoder\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\Global.h
# End Source File
# Begin Source File

SOURCE=.\encoder\huffman.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\huffman.h
# End Source File
# Begin Source File

SOURCE=.\encoder\libr263.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\libr263.h
# End Source File
# Begin Source File

SOURCE=.\encoder\mot_est.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\mot_est.h
# End Source File
# Begin Source File

SOURCE=.\encoder\owntypes.h
# End Source File
# Begin Source File

SOURCE=.\encoder\pred.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\pred.h
# End Source File
# Begin Source File

SOURCE=.\encoder\quant.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\quant.h
# End Source File
# Begin Source File

SOURCE=.\encoder\rlib.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\rlib.h
# End Source File
# Begin Source File

SOURCE=.\encoder\stream.cpp
# End Source File
# Begin Source File

SOURCE=.\encoder\stream.h
# End Source File
# Begin Source File

SOURCE=.\encoder\struct.h
# End Source File
# End Group
# Begin Group "decoder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\decoder\convert.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\convert.h
# End Source File
# Begin Source File

SOURCE=.\decoder\decdef.h
# End Source File
# Begin Source File

SOURCE=.\decoder\decstruct.h
# End Source File
# Begin Source File

SOURCE=.\decoder\DGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\DGlobal.h
# End Source File
# Begin Source File

SOURCE=.\decoder\GetBits.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\GetBits.h
# End Source File
# Begin Source File

SOURCE=.\decoder\GetBlk.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\GetBlk.h
# End Source File
# Begin Source File

SOURCE=.\decoder\GetHdr.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\GetHdr.h
# End Source File
# Begin Source File

SOURCE=.\decoder\GetPic.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\GetPic.h
# End Source File
# Begin Source File

SOURCE=.\decoder\GetVlc.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\GetVlc.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Idct.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Idct.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Idctref.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Idctref.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Indices.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Indices.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Recon.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Recon.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Sac.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Sac.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Sactbls.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Sactbls.h
# End Source File
# Begin Source File

SOURCE=.\decoder\Tmndec.cpp
# End Source File
# Begin Source File

SOURCE=.\decoder\Tmndec.h
# End Source File
# Begin Source File

SOURCE=.\decoder\YuvToRgb.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
