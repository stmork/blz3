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
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include_win32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /D "BLZ3_USE_OPENGL" /FA /FR /YX /FD /GZ /TP /c
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

SOURCE=.\AppObject.odl
# End Source File
# Begin Source File

SOURCE=.\AppObjectDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AppObjectView.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceView.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRenderDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AppRenderView.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Action.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ActionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ActionScene.cpp
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

SOURCE=.\b3ImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ImagePreviewFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\b3PickVector.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ProfileBevelSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ProfileBevelStumpSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ProfileShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SelectColor.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SelectObject.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SelectTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\b3ShowRaytrace.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SpanningShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\b3Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\b3SplinePreview.cpp
# End Source File
# Begin Source File

SOURCE=.\b3VectorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateSplineShape.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateStencil.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateTriangles.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSG1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSG3.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSGMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCSGTorus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDistributed.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFulcrum.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHierarchy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLDC.cpp
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

SOURCE=.\DlgNewObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgObjectCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProfileBevelSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProfileBevelStumpSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScene.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSearchPathList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShape1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShape2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShape3.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSpinTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStepMove.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStepRotate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuperSampling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTorus.cpp
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

SOURCE=.\AppObjectDoc.h
# End Source File
# Begin Source File

SOURCE=.\AppObjectView.h
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceDoc.h
# End Source File
# Begin Source File

SOURCE=.\AppRaytraceView.h
# End Source File
# Begin Source File

SOURCE=.\AppRenderDoc.h
# End Source File
# Begin Source File

SOURCE=.\AppRenderView.h
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

SOURCE=.\b3ExampleScene.h
# End Source File
# Begin Source File

SOURCE=.\b3Fulcrum.h
# End Source File
# Begin Source File

SOURCE=.\b3ImageList.h
# End Source File
# Begin Source File

SOURCE=.\b3ImagePreviewFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\b3PickVector.h
# End Source File
# Begin Source File

SOURCE=.\b3Profile.h
# End Source File
# Begin Source File

SOURCE=.\b3ProfileBevelSpline.h
# End Source File
# Begin Source File

SOURCE=.\b3ProfileBevelStumpSpline.h
# End Source File
# Begin Source File

SOURCE=.\b3ProfileShapeDialog.h
# End Source File
# Begin Source File

SOURCE=.\b3SelectColor.h
# End Source File
# Begin Source File

SOURCE=.\b3SelectObject.h
# End Source File
# Begin Source File

SOURCE=.\b3SelectTexture.h
# End Source File
# Begin Source File

SOURCE=.\b3ShapeDialog.h
# End Source File
# Begin Source File

SOURCE=.\b3ShowRaytrace.h
# End Source File
# Begin Source File

SOURCE=.\b3SpanningShapeDialog.h
# End Source File
# Begin Source File

SOURCE=.\b3Splash.h
# End Source File
# Begin Source File

SOURCE=.\b3SplinePreview.h
# End Source File
# Begin Source File

SOURCE=.\b3VectorEdit.h
# End Source File
# Begin Source File

SOURCE=.\b3View.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DlgCamera.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateMaterial.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateSplineShape.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateStencil.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateTriangles.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSG1.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSG3.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSGMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgCSGTorus.h
# End Source File
# Begin Source File

SOURCE=.\DlgDistributed.h
# End Source File
# Begin Source File

SOURCE=.\DlgFulcrum.h
# End Source File
# Begin Source File

SOURCE=.\DlgHierarchy.h
# End Source File
# Begin Source File

SOURCE=.\DlgLDC.h
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

SOURCE=.\DlgNewObject.h
# End Source File
# Begin Source File

SOURCE=.\DlgObjectCopy.h
# End Source File
# Begin Source File

SOURCE=.\DlgProfileBevelSpline.h
# End Source File
# Begin Source File

SOURCE=.\DlgProfileBevelStumpSpline.h
# End Source File
# Begin Source File

SOURCE=.\DlgProperties.h
# End Source File
# Begin Source File

SOURCE=.\DlgScene.h
# End Source File
# Begin Source File

SOURCE=.\DlgSearchPathList.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectProfile.h
# End Source File
# Begin Source File

SOURCE=.\DlgShape1.h
# End Source File
# Begin Source File

SOURCE=.\DlgShape2.h
# End Source File
# Begin Source File

SOURCE=.\DlgShape3.h
# End Source File
# Begin Source File

SOURCE=.\DlgSpinTest.h
# End Source File
# Begin Source File

SOURCE=.\DlgStepMove.h
# End Source File
# Begin Source File

SOURCE=.\DlgStepRotate.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuperSampling.h
# End Source File
# Begin Source File

SOURCE=.\DlgTexturePathList.h
# End Source File
# Begin Source File

SOURCE=.\DlgTorus.h
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

SOURCE=.\res\bbox_sel_shape.ico
# End Source File
# Begin Source File

SOURCE=.\res\bbox_sel_sub_shape.ico
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

SOURCE=.\res\bbox_world.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_box.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_cone.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_cylinder.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_ellipsoid.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_intersect.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_sphere.ico
# End Source File
# Begin Source File

SOURCE=.\res\csg_torus.ico
# End Source File
# Begin Source File

SOURCE=.\res\csgmode_intersect.ico
# End Source File
# Begin Source File

SOURCE=.\res\csgmode_sub.ico
# End Source File
# Begin Source File

SOURCE=.\res\csgmode_union.ico
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=.\res\H_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\left.ico
# End Source File
# Begin Source File

SOURCE=.\res\object.ico
# End Source File
# Begin Source File

SOURCE=.\res\profile_bevel_spline.ico
# End Source File
# Begin Source File

SOURCE=.\res\profile_bevelstump_spline.ico
# End Source File
# Begin Source File

SOURCE=.\res\raytrace.ico
# End Source File
# Begin Source File

SOURCE=.\res\right.ico
# End Source File
# Begin Source File

SOURCE=.\res\rot_left.ico
# End Source File
# Begin Source File

SOURCE=.\res\rot_right.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_area.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_bo.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_box.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_bspline_area.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_bspline_cylinder.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_bspline_ring.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_bspline_rot.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_cone.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_cylinder.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_disk.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_ellipsoid.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_sphere.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_torus.ico
# End Source File
# Begin Source File

SOURCE=.\res\shape_triangles.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
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
# Begin Source File

SOURCE=.\res\up.ico
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
