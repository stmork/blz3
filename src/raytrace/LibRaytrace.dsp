# Microsoft Developer Studio Project File - Name="LibRaytrace" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibRaytrace - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibRaytrace.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibRaytrace.mak" CFG="LibRaytrace - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibRaytrace - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibRaytrace - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibRaytrace - Win32 Release"

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
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /Ob2 /I "..\..\include" /I "..\..\include_win32" /D "NDEBUG" /D "_LIB" /D "BLZ3_USE_OPENGL" /D "WIN32" /D "_MBCS" /D "_AFXDLL" /D "_B3_EXPORT" /YX /FD /TP /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibRaytrace.lib"

!ELSEIF  "$(CFG)" == "LibRaytrace - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\lib\win32_i386\LibRaytrace_dbg.lib"

!ENDIF 

# Begin Target

# Name "LibRaytrace - Win32 Release"
# Name "LibRaytrace - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=b3Animation.cc
# End Source File
# Begin Source File

SOURCE=.\b3AnimElement.cc
# End Source File
# Begin Source File

SOURCE=.\b3Area.cc
# End Source File
# Begin Source File

SOURCE=b3BBox.cc
# End Source File
# Begin Source File

SOURCE=.\b3Box.cc
# End Source File
# Begin Source File

SOURCE=b3Bump.cc
# End Source File
# Begin Source File

SOURCE=.\b3BumpSampler.cc
# End Source File
# Begin Source File

SOURCE=b3Condition.cc
# End Source File
# Begin Source File

SOURCE=.\b3Cone.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGBox.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGCone.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGCylinder.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGEllipsoid.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGShape.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGSphere.cc
# End Source File
# Begin Source File

SOURCE=.\b3CSGTorus.cc
# End Source File
# Begin Source File

SOURCE=.\b3Cylinder.cc
# End Source File
# Begin Source File

SOURCE=.\b3Disk.cc
# End Source File
# Begin Source File

SOURCE=.\b3Ellipsoid.cc
# End Source File
# Begin Source File

SOURCE=b3Init.cc
# End Source File
# Begin Source File

SOURCE=.\b3Intersect.cc
# End Source File
# Begin Source File

SOURCE=b3Light.cc
# End Source File
# Begin Source File

SOURCE=b3Material.cc
# End Source File
# Begin Source File

SOURCE=.\b3MaterialSampler.cc
# End Source File
# Begin Source File

SOURCE=.\b3Normal.cc
# End Source File
# Begin Source File

SOURCE=.\b3NormalDeriv.cc
# End Source File
# Begin Source File

SOURCE=.\b3PickInfo.cc
# End Source File
# Begin Source File

SOURCE=.\b3PrepareInfo.cc
# End Source File
# Begin Source File

SOURCE=.\b3RayRow.cc
# End Source File
# Begin Source File

SOURCE=.\b3Raytrace.cc
# End Source File
# Begin Source File

SOURCE=.\b3ReadCOB.cc
# End Source File
# Begin Source File

SOURCE=.\b3ReadTGF.cc
# End Source File
# Begin Source File

SOURCE=.\b3RenderShape.cc
# End Source File
# Begin Source File

SOURCE=.\b3RenderView.cc
# End Source File
# Begin Source File

SOURCE=b3Scene.cc
# End Source File
# Begin Source File

SOURCE=.\b3Shade.cc
# End Source File
# Begin Source File

SOURCE=.\b3ShadeMork.cc
# End Source File
# Begin Source File

SOURCE=.\b3ShadePhong.cc
# End Source File
# Begin Source File

SOURCE=b3Shape.cc
# End Source File
# Begin Source File

SOURCE=b3Special.cc
# End Source File
# Begin Source File

SOURCE=.\b3Sphere.cc
# End Source File
# Begin Source File

SOURCE=.\b3SplineArea.cc
# End Source File
# Begin Source File

SOURCE=.\b3SplineCylinder.cc
# End Source File
# Begin Source File

SOURCE=.\b3SplineRing.cc
# End Source File
# Begin Source File

SOURCE=.\b3SplineRot.cc
# End Source File
# Begin Source File

SOURCE=.\b3SplineShape.cc
# End Source File
# Begin Source File

SOURCE=.\b3Torus.cc
# End Source File
# Begin Source File

SOURCE=.\b3Triangles.cc
# End Source File
# Begin Source File

SOURCE=.\b3TriangleShape.cc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Animation.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Base.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3BBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Bump.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3BumpSampler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Condition.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Light.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Material.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3MaterialSampler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3PickInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3PrepareInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Raytrace.h
# End Source File
# Begin Source File

SOURCE=.\b3ReadCOB.h
# End Source File
# Begin Source File

SOURCE=.\b3ReadTGF.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3RenderView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Scene.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Shape.h
# End Source File
# Begin Source File

SOURCE=..\..\include\blz3\raytrace\b3Special.h
# End Source File
# End Group
# End Target
# End Project
