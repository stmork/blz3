# Microsoft Developer Studio Project File - Name="LibSystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibSystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibSystem.mak" CFG="LibSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibSystem - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibSystem - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibSystem - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Release"
# PROP Intermediate_Dir "..\..\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_AFXDLL" /D "_LIB" /D "BLZ3_USE_OPENGL" /YX /FD /TP /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSystem.lib"

!ELSEIF  "$(CFG)" == "LibSystem - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Debug"
# PROP Intermediate_Dir "..\..\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_LIB" /D "BLZ3_USE_OPENGL" /FR /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSystem_dbg.lib"

!ENDIF 

# Begin Target

# Name "LibSystem - Win32 Release"
# Name "LibSystem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\b3App.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ColorField.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Config.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Date.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Dir.cpp
# End Source File
# Begin Source File

SOURCE=.\b3DisplayView.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Document.cpp
# End Source File
# Begin Source File

SOURCE=.\b3File.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Log.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Mem.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Print.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Reg.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ScrollView.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ShowColor.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ShowImage.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Time.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Toolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Tracker.cpp
# End Source File
# Begin Source File

SOURCE=.\b3View.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ViewParam.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include_win32\blz3\b3Config.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Date.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Dir.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Display.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Document.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3File.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Log.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\b3Main.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Mem.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Print.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3ScrollView.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Thread.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Time.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3Tracker.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3View.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3ViewParam.h
# End Source File
# End Group
# End Target
# End Project
