# Microsoft Developer Studio Project File - Name="LibBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibBase.mak" CFG="LibBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibBase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibBase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibBase - Win32 Release"

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
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibBase.lib"

!ELSEIF  "$(CFG)" == "LibBase - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibBase_dbg.lib"

!ENDIF 

# Begin Target

# Name "LibBase - Win32 Release"
# Name "LibBase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\b3Aux.cc
# End Source File
# Begin Source File

SOURCE=.\b3Cubic.cc
# End Source File
# Begin Source File

SOURCE=.\b3Display.cc
# End Source File
# Begin Source File

SOURCE=.\b3Endian.cc
# End Source File
# Begin Source File

SOURCE=.\b3FileList.cc
# End Source File
# Begin Source File

SOURCE=.\b3FileMem.cc
# End Source File
# Begin Source File

SOURCE=.\b3Item.cc
# End Source File
# Begin Source File

SOURCE=.\b3ItemRegister.cc
# End Source File
# Begin Source File

SOURCE=.\b3ItemRegister.h
# End Source File
# Begin Source File

SOURCE=.\b3ItemRegisterEntry.cc
# End Source File
# Begin Source File

SOURCE=.\b3Matrix.cc
# End Source File
# Begin Source File

SOURCE=.\b3Node.cc
# End Source File
# Begin Source File

SOURCE=.\b3Pick.cc
# End Source File
# Begin Source File

SOURCE=.\b3Procedure.cc
# End Source File
# Begin Source File

SOURCE=.\b3Render.cc
# End Source File
# Begin Source File

SOURCE=.\b3Row.cc
# End Source File
# Begin Source File

SOURCE=.\b3SearchPath.cc
# End Source File
# Begin Source File

SOURCE=.\b3Spline.cc
# End Source File
# Begin Source File

SOURCE=.\b3World.cc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Array.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Aux.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Compare.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Cubic.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Display.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Endian.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3FileAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3FileList.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3FileMem.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3List.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Node.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Nurbs.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Pick.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Procedure.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Render.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3SearchPath.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3Spline.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\base\b3World.h
# End Source File
# End Group
# End Target
# End Project
