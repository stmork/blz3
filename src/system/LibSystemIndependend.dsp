# Microsoft Developer Studio Project File - Name="LibSystemInd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibSystemInd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibSystemIndependend.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibSystemIndependend.mak" CFG="LibSystemInd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibSystemInd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibSystemInd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibSystemInd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\Release"
# PROP BASE Intermediate_Dir "..\..\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Release"
# PROP Intermediate_Dir "..\..\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /Ob2 /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_AFXDLL" /D "_LIB" /D "BLZ3_USE_OPENGL" /YX /FD /TP /TP /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSystemInd.lib"

!ELSEIF  "$(CFG)" == "LibSystemInd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Debug"
# PROP Intermediate_Dir "..\..\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_LIB" /D "BLZ3_USE_OPENGL" /FR /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibSystemInd_dbg.lib"

!ENDIF 

# Begin Target

# Name "LibSystemInd - Win32 Release"
# Name "LibSystemInd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\b3Date.cc
# End Source File
# Begin Source File

SOURCE=.\b3Exception.cc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\blz3\system\b3Date.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\system\b3DirAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\system\b3Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\system\b3FileAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\system\b3Memory.h
# End Source File
# End Group
# End Target
# End Project
