# Microsoft Developer Studio Project File - Name="AppLines" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AppLines - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AppLines.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AppLines.mak" CFG="AppLines - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AppLines - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AppLines - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AppLines - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Release"
# PROP Intermediate_Dir "..\..\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "BLZ3_USE_OPENGL" /YX /FD /TP /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 LibSystem.lib LibBase.lib LibRaytrace.lib opengl32.lib glu32.lib /nologo /version:3.0 /subsystem:windows /incremental:yes /machine:I386 /out:"..\..\bin\win32\AppLines.exe" /libpath:"..\..\lib\win32_i386"

!ELSEIF  "$(CFG)" == "AppLines - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Debug"
# PROP Intermediate_Dir "..\..\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /D "BLZ3_USE_OPENGL" /FR /YX /FD /GZ /TP /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 LibSystem_dbg.lib LibBase_dbg.lib LibRaytrace_dbg.lib opengl32.lib glu32.lib /nologo /version:3.0 /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\lib\win32_i386"

!ENDIF 

# Begin Target

# Name "AppLines - Win32 Release"
# Name "AppLines - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AppLines.cpp
# End Source File
# Begin Source File

SOURCE=.\AppLines.odl
# End Source File
# Begin Source File

SOURCE=.\AppLines.rc
# End Source File
# Begin Source File

SOURCE=.\AppLinesDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AppLinesView.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceView.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Action.cpp
# End Source File
# Begin Source File

SOURCE=.\b3CameraVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ControlLDC.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ExampleScene.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Fulcrum.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SelectTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ShowRaytrace.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDistributed.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHierarchy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLensFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLight.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModellerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNebular.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScene.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSearchPathList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuperSampling.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AppLines.h
# End Source File
# Begin Source File

SOURCE=.\AppLinesDoc.h
# End Source File
# Begin Source File

SOURCE=.\AppLinesView.h
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceDoc.h
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceView.h
# End Source File
# Begin Source File

SOURCE=.\b3Action.h
# End Source File
# Begin Source File

SOURCE=.\b3CameraVolume.h
# End Source File
# Begin Source File

SOURCE=.\b3ControlLDC.h
# End Source File
# Begin Source File

SOURCE=..\..\include_win32\blz3\system\b3floatedit.h
# End Source File
# Begin Source File

SOURCE=.\b3Fulcrum.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgDistributed.h
# End Source File
# Begin Source File

SOURCE=.\DlgHierarchy.h
# End Source File
# Begin Source File

SOURCE=.\DlgLensFlare.h
# End Source File
# Begin Source File

SOURCE=.\DlgLight.h
# End Source File
# Begin Source File

SOURCE=.\DlgModellerInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgNebular.h
# End Source File
# Begin Source File

SOURCE=.\DlgScene.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuperSampling.h
# End Source File
# Begin Source File

SOURCE=.\DlgTexturePathList.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AppLines.ico
# End Source File
# Begin Source File

SOURCE=.\res\AppLines.rc2
# End Source File
# Begin Source File

SOURCE=.\res\AppLinesDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_empty.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_s_s.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sel_shape.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sel_sub_shape.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sha.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_shape.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sub.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sub_shape.ico
# End Source File
# Begin Source File

SOURCE=.\res\bboxworld.ico
# End Source File
# Begin Source File

SOURCE=.\res\Display.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_action.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_camera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_display.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_light.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_object.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AppLines.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
