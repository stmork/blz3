# Microsoft Developer Studio Project File - Name="LibSmartGUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibSmartGUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibSmartGUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibSmartGUI.mak" CFG="LibSmartGUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibSmartGUI - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibSmartGUI - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibSmartGUI - Win32 Release"

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
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /Ob2 /I "..\..\include" /I "..\..\include_win32" /D "NDEBUG" /D "_LIB" /D "BLZ3_USE_OPENGL" /D "WIN32" /D "_MBCS" /D "_AFXDLL" /D "_B3_EXPORT" /YX /FD /TP /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSmartGUI.lib"

!ELSEIF  "$(CFG)" == "LibSmartGUI - Win32 Debug"

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
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include_win32" /D "_DEBUG" /D "_LIB" /D "BLZ3_USE_OPENGL" /D "WIN32" /D "_MBCS" /D "_AFXDLL" /D "_B3_EXPORT" /FR /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSmartGUI_dbg.lib"

!ENDIF 

# Begin Target

# Name "LibSmartGUI - Win32 Release"
# Name "LibSmartGUI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CJFlatComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolControlsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
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
# Begin Source File

SOURCE=.\CJFlatComboBox.h
# End Source File
# Begin Source File

SOURCE=.\CoolControlsManager.h
# End Source File
# Begin Source File

SOURCE=.\CoolMenu.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# End Group
# End Target
# End Project
