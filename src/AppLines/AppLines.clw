; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAppLinesView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "applines.h"
LastPage=0

ClassCount=17
Class1=CAppLinesApp
Class2=CAboutDlg
Class3=CAppLinesDoc
Class4=CAppLinesView
Class5=CAppRaytraceDoc
Class6=CAppRaytraceView
Class7=CB3ShowRaytrace
Class8=CChildFrame
Class9=CDlgHierarchy
Class10=CDlgScene
Class11=CMainFrame

ResourceCount=15
Resource1=IDD_SCENE
Resource2=IDR_TOOLBAR_DISPLAY
Resource3=IDR_BLZ3TYPE
Resource4=IDD_SUPER_SAMPLE
Resource5=IDR_TOOLBAR_ACTION
Resource6=IDR_DISPLAYTYPE
Resource7=IDD_LENS_FLARE
Resource8=IDR_TOOLBAR_VIEW
Resource9=IDD_DISTRIBUTED
Resource10=IDD_HIERARCHY
Resource11=IDD_NEBULAR
Resource12=IDR_MAINFRAME
Resource13=IDR_TOOLBAR_OBJECT
Class12=CDlgDistributed
Class13=CDlgLensFlare
Class14=CDlgNebular
Class15=CDlgSuperSampling
Class16=CB3FloatEdit
Resource14=IDD_ABOUTBOX
Class17=CDlgModellerInfo
Resource15=IDD_MODELLER_INFO

[CLS:CAppLinesApp]
Type=0
BaseClass=CWinApp
HeaderFile=AppLines.h
ImplementationFile=AppLines.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=AppLines.cpp
ImplementationFile=AppLines.cpp
LastObject=CAboutDlg

[CLS:CAppLinesDoc]
Type=0
BaseClass=CDocument
HeaderFile=AppLinesDoc.h
ImplementationFile=AppLinesDoc.cpp
Filter=N
VirtualFilter=DC

[CLS:CAppLinesView]
Type=0
BaseClass=CScrollView
HeaderFile=AppLinesView.h
ImplementationFile=AppLinesView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CAppRaytraceDoc]
Type=0
BaseClass=CDocument
HeaderFile=AppRaytraceDoc.h
ImplementationFile=AppRaytraceDoc.cpp

[CLS:CAppRaytraceView]
Type=0
BaseClass=CB3ScrollView
HeaderFile=AppRaytraceView.h
ImplementationFile=AppRaytraceView.cpp

[CLS:CB3ShowRaytrace]
Type=0
BaseClass=CB3ShowImage
HeaderFile=b3ShowRaytrace.h
ImplementationFile=b3ShowRaytrace.cpp
LastObject=CB3ShowRaytrace

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CDlgHierarchy]
Type=0
BaseClass=CDialog
HeaderFile=DlgHierarchy.h
ImplementationFile=DlgHierarchy.cpp

[CLS:CDlgScene]
Type=0
BaseClass=CDialog
HeaderFile=DlgScene.h
ImplementationFile=DlgScene.cpp
LastObject=CDlgScene
Filter=D
VirtualFilter=dWC

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HIERARCHY]
Type=1
Class=CDlgHierarchy
ControlCount=5
Control1=IDC_HIERARCHY,SysTreeView32,1342242831
Control2=IDC_ACTIVATE,button,1342242816
Control3=IDC_DEACTIVATE,button,1342242816
Control4=IDC_ALL_ACTIVATE,button,1342242816
Control5=IDC_ALL_DEACTIVATE,button,1342242816

[DLG:IDD_SCENE]
Type=1
Class=CDlgScene
ControlCount=23
Control1=IDC_BG_LIGHT,button,1342308361
Control2=IDC_BG_COLOR,button,1342177289
Control3=IDC_BG_SKY,button,1342177289
Control4=IDC_BG_IMAGE,button,1342177289
Control5=IDC_BG_IMAGE_SELECT,button,1342242816
Control6=IDC_BG_TOP_SELECT,button,1342242816
Control7=IDC_BG_BOTTOM_SELECT,button,1342242816
Control8=IDC_RES_VALID,button,1342242819
Control9=IDC_OPEN_GFX,button,1342242819
Control10=IDC_XRES,edit,1350631552
Control11=IDC_XRES_SPIN,msctls_updown32,1342177334
Control12=IDC_YRES,edit,1350631552
Control13=IDC_YRES_SPIN,msctls_updown32,1342177334
Control14=IDC_RAYDEPTH,msctls_trackbar32,1342242817
Control15=IDC_SHADOW_BRIGHTNESS,msctls_trackbar32,1342242817
Control16=IDC_PREVIEW_RAYTRACE,static,1342181383
Control17=IDC_PREVIEW_BGIMAGE,static,1342181383
Control18=IDC_RAYDEPTH_LEGEND,static,1342308352
Control19=IDC_SHADOW_BRIGHTNESS_LEGEND,static,1342308352
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_TOOLBAR_VIEW]
Type=1
Class=?
Command1=ID_VIEW_PERSPECTIVE
Command2=ID_VIEW_TOP
Command3=ID_VIEW_FRONT
Command4=ID_VIEW_RIGHT
Command5=ID_VIEW_BACK
Command6=ID_VIEW_LEFT
Command7=ID_VIEW_BIGGER
Command8=ID_VIEW_SMALLER
Command9=ID_VIEW_SELECT
Command10=ID_VIEW_POP
Command11=ID_VIEW_ORIGINAL
CommandCount=11

[TB:IDR_TOOLBAR_ACTION]
Type=1
Class=CAppLinesView
Command1=ID_OBJ_SELECT
Command2=ID_OBJ_MOVE
Command3=ID_OBJ_ROTATE
Command4=ID_OBJ_SCALE
Command5=ID_CAM_MOVE
Command6=ID_CAM_TURN
Command7=ID_CAM_ROTATE
Command8=ID_CAM_VIEW
Command9=ID_VIEW_TO_FULCRUM
Command10=ID_CAM_SELECT
Command11=ID_LIGHT_TURN
Command12=ID_LIGHT_SELECT
CommandCount=12

[TB:IDR_TOOLBAR_OBJECT]
Type=1
Class=?
Command1=ID_HIERACHY
Command2=ID_RAYTRACE
Command3=ID_DLG_SCENE
Command4=ID_MODELLER_INFO
CommandCount=4

[TB:IDR_TOOLBAR_DISPLAY]
Type=1
Class=?
Command1=ID_B3_UNFILTERED
Command2=ID_B3_FILTERED
Command3=ID_B3_ORIGINAL
Command4=ID_B3_WIDTH
Command5=ID_B3_HEIGHT
Command6=ID_B3_FULL
Command7=ID_B3_MORE
Command8=ID_B3_LESS
Command9=ID_B3_MAGNIFY
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_PREF_SAVE
Command4=ID_PREF_AUTOSAVE
Command5=ID_FILE_PRINT_SETUP
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_VIEW_TOOLBAR
Command9=IDM_BAR_VIEW
Command10=IDM_BAR_ACTION
Command11=IDM_BAR_DISPLAY
Command12=IDM_BAR_SCENE
Command13=ID_CUST_MAIN
Command14=ID_CUST_VIEW
Command15=ID_CUST_ACTION
Command16=ID_CUST_DISPLAY
Command17=ID_CUST_SCENE
Command18=ID_VIEW_STATUS_BAR
Command19=ID_APP_ABOUT
CommandCount=19

[MNU:IDR_BLZ3TYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_PREF_SAVE
Command7=ID_PREF_AUTOSAVE
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_OBJ_SELECT
Command18=ID_OBJ_MOVE
Command19=ID_OBJ_ROTATE
Command20=ID_OBJ_SCALE
Command21=ID_CAM_MOVE
Command22=ID_CAM_TURN
Command23=ID_CAM_ROTATE
Command24=ID_CAM_VIEW
Command25=ID_VIEW_TO_FULCRUM
Command26=ID_LIGHT_TURN
Command27=ID_VIEW_TOOLBAR
Command28=IDM_BAR_VIEW
Command29=IDM_BAR_ACTION
Command30=IDM_BAR_DISPLAY
Command31=IDM_BAR_SCENE
Command32=ID_CUST_MAIN
Command33=ID_CUST_VIEW
Command34=ID_CUST_ACTION
Command35=ID_CUST_DISPLAY
Command36=ID_CUST_SCENE
Command37=ID_VIEW_STATUS_BAR
Command38=ID_VIEW_ANTIALIAS
Command39=ID_VIEW_PERSPECTIVE
Command40=ID_VIEW_TOP
Command41=ID_VIEW_FRONT
Command42=ID_VIEW_RIGHT
Command43=ID_VIEW_BACK
Command44=ID_VIEW_LEFT
Command45=ID_VIEW_BIGGER
Command46=ID_VIEW_SMALLER
Command47=ID_VIEW_SELECT
Command48=ID_VIEW_POP
Command49=ID_VIEW_ORIGINAL
Command50=ID_DLG_SCENE
Command51=ID_HIERACHY
Command52=ID_RAYTRACE
Command53=ID_MODELLER_INFO
Command54=ID_WINDOW_NEW
Command55=ID_WINDOW_CASCADE
Command56=ID_WINDOW_TILE_VERT
Command57=ID_WINDOW_TILE_HORZ
Command58=ID_WINDOW_ARRANGE
Command59=ID_APP_ABOUT
CommandCount=59

[MNU:IDR_DISPLAYTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_PREF_SAVE
Command7=ID_PREF_AUTOSAVE
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_VIEW_TOOLBAR
Command14=IDM_BAR_VIEW
Command15=IDM_BAR_ACTION
Command16=IDM_BAR_DISPLAY
Command17=IDM_BAR_SCENE
Command18=ID_CUST_MAIN
Command19=ID_CUST_VIEW
Command20=ID_CUST_ACTION
Command21=ID_CUST_DISPLAY
Command22=ID_CUST_SCENE
Command23=ID_VIEW_STATUS_BAR
Command24=ID_VIEW_THUMB
Command25=ID_B3_UNFILTERED
Command26=ID_B3_FILTERED
Command27=ID_B3_ORIGINAL
Command28=ID_B3_WIDTH
Command29=ID_B3_HEIGHT
Command30=ID_B3_FULL
Command31=ID_B3_MORE
Command32=ID_B3_LESS
Command33=ID_B3_MAGNIFY
Command34=ID_WINDOW_CASCADE
Command35=ID_WINDOW_TILE_VERT
Command36=ID_WINDOW_TILE_HORZ
Command37=ID_WINDOW_ARRANGE
Command38=ID_APP_ABOUT
CommandCount=38

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_VIEW_BIGGER
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_VIEW_MOVE_BOTTOM
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_MOVE_LEFT
Command16=ID_VIEW_MOVE_RIGHT
Command17=ID_VIEW_SMALLER
Command18=ID_VIEW_MOVE_TOP
Command19=ID_EDIT_CUT
Command20=ID_EDIT_UNDO
CommandCount=20

[DLG:IDD_LENS_FLARE]
Type=1
Class=CDlgLensFlare
ControlCount=6
Control1=IDC_ACT_LENS_FLARE,button,1342242819
Control2=IDC_LF_VALUE,edit,1350631552
Control3=IDC_LF_CHANGE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_LF_COLOR,static,1342181376
Control6=IDC_LF_PREVIEW,static,1342181383

[DLG:IDD_SUPER_SAMPLE]
Type=1
Class=CDlgSuperSampling
ControlCount=7
Control1=IDC_SS4_RED,msctls_trackbar32,1342242817
Control2=IDC_SS4_GREEN,msctls_trackbar32,1342242817
Control3=IDC_SS4_BLUE,msctls_trackbar32,1342242817
Control4=IDC_SS4_RED_LEGEND,static,1342308352
Control5=IDC_SS4_GREEN_LEGEND,static,1342308352
Control6=IDC_SS4_BLUE_LEGEND,static,1342308352
Control7=IDC_SS4_ACTIVE,button,1342242819

[DLG:IDD_NEBULAR]
Type=1
Class=CDlgNebular
ControlCount=6
Control1=IDC_ACT_NEBULAR,button,1342242819
Control2=ID_NEB_CHANGE,button,1342242816
Control3=IDC_NEB_COLOR,static,1342181376
Control4=IDC_NEB_PREVIEW,static,1342181383
Control5=IDC_NEB_DISTANCE,edit,1350631552
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DISTRIBUTED]
Type=1
Class=CDlgDistributed
ControlCount=21
Control1=IDC_ACT_DISTRIBUTED,button,1342242819
Control2=IDC_ACT_MOTION_BLUR,button,1342242819
Control3=IDC_SMP_REGULAR,button,1342308361
Control4=IDC_SMP_RANDOM,button,1342177289
Control5=IDC_SMP_JITTER,button,1342177289
Control6=IDC_SMP_SEMI,button,1342177289
Control7=IDC_SMP_SEPARATED,button,1342177289
Control8=IDC_PFLTR_BOX,button,1342308361
Control9=IDC_PFLTR_GAUSS,button,1342177289
Control10=IDC_TFLTR_BOX,button,1342308361
Control11=IDC_TFLTR_GAUSS,button,1342177289
Control12=IDC_TFLTR_SHUTTER,button,1342177289
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_SPP,edit,1350631552
Control17=IDC_SPP_SPIN,msctls_updown32,1342177334
Control18=IDC_SPF,edit,1350631552
Control19=IDC_SPF_SPIN,msctls_updown32,1342177334
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352

[CLS:CDlgDistributed]
Type=0
HeaderFile=DlgDistributed.h
ImplementationFile=DlgDistributed.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgDistributed
VirtualFilter=dWC

[CLS:CDlgLensFlare]
Type=0
HeaderFile=DlgLensFlare.h
ImplementationFile=DlgLensFlare.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LF_VALUE
VirtualFilter=dWC

[CLS:CDlgNebular]
Type=0
HeaderFile=DlgNebular.h
ImplementationFile=DlgNebular.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_NEB_CHANGE
VirtualFilter=dWC

[CLS:CDlgSuperSampling]
Type=0
HeaderFile=DlgSuperSampling.h
ImplementationFile=DlgSuperSampling.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SS4_BLUE_LEGEND
VirtualFilter=dWC

[CLS:CB3FloatEdit]
Type=0
HeaderFile=..\..\include_win32\blz3\system\b3floatedit.h
ImplementationFile=..\system_win32\b3floatedit.cpp
BaseClass=CEdit
Filter=W
LastObject=CB3FloatEdit

[DLG:IDD_MODELLER_INFO]
Type=1
Class=CDlgModellerInfo
ControlCount=15
Control1=IDC_FULCRUM_X,edit,1350631552
Control2=IDC_FULCRUM_Y,edit,1350631552
Control3=IDC_FULCRUM_Z,edit,1350631552
Control4=IDC_FULCRUM_CLR,button,1342242816
Control5=IDC_SNAP_TO_GRID,button,1342242819
Control6=IDC_SNAP_TO_ANGLE,button,1342242819
Control7=IDC_STEP_GRID,edit,1350631552
Control8=IDC_STEP_ANGLE,edit,1350631552
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDOK,button,1342242817
Control15=IDCANCEL,button,1342242816

[CLS:CDlgModellerInfo]
Type=0
HeaderFile=DlgModellerInfo.h
ImplementationFile=DlgModellerInfo.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_VIEW_TO_FULCRUM

