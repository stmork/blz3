; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgHierarchy
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "applines.h"
LastPage=0

ClassCount=27
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

ResourceCount=25
Resource1=IDD_CAMERA
Resource2=IDD_SEARCH_PATH_LIST
Resource3=IDD_DISTRIBUTED
Resource4=IDD_NEW_ITEM
Resource5=IDD_FULCRUM
Resource6=IDD_ABOUTBOX
Resource7=IDD_SCENE
Resource8=IDD_LENS_FLARE
Resource9=IDR_TOOLBAR_DISPLAY
Resource10=IDR_TOOLBAR_VIEW
Resource11=IDD_NEBULAR
Resource12=IDR_TOOLBAR_LIGHT
Resource13=IDD_LIGHT
Class12=CDlgDistributed
Class13=CDlgLensFlare
Class14=CDlgNebular
Class15=CDlgSuperSampling
Class16=CB3FloatEdit
Resource14=IDR_TOOLBAR_OBJECT
Class17=CDlgModellerInfo
Resource15=IDD_STEP_MOVE
Resource16=IDR_MAINFRAME
Resource17=IDD_HIERARCHY
Resource18=IDR_TOOLBAR_CAMERA
Class18=CDlgCreateItem
Resource19=IDD_MODELLER_INFO
Class19=CDlgLight
Class20=CB3ControlLDC
Resource20=IDD_SUPER_SAMPLE
Class21=CDlgLDC
Resource21=IDR_TOOLBAR_ACTION
Class22=CDlgCamera
Class23=CB3ShowImage
Resource22=IDR_DISPLAYTYPE
Class24=CDlgFulcrum
Resource23=IDR_BLZ3TYPE
Resource24=IDD_LDC
Class25=CDlgStepRotate
Class26=CDlgStepMove
Class27=CB3Dialogbar
Resource25=IDD_STEP_ROTATE

[CLS:CAppLinesApp]
Type=0
BaseClass=CWinApp
HeaderFile=AppLines.h
ImplementationFile=AppLines.cpp
Filter=N
VirtualFilter=AC
LastObject=ID_FILE_OPEN

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=AppLines.cpp
ImplementationFile=AppLines.cpp
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[CLS:CAppLinesDoc]
Type=0
BaseClass=CDocument
HeaderFile=AppLinesDoc.h
ImplementationFile=AppLinesDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=CAppLinesDoc

[CLS:CAppLinesView]
Type=0
BaseClass=CScrollView
HeaderFile=AppLinesView.h
ImplementationFile=AppLinesView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CAppLinesView

[CLS:CAppRaytraceDoc]
Type=0
BaseClass=CDocument
HeaderFile=AppRaytraceDoc.h
ImplementationFile=AppRaytraceDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=CAppRaytraceDoc

[CLS:CAppRaytraceView]
Type=0
BaseClass=CB3ScrollView
HeaderFile=AppRaytraceView.h
ImplementationFile=AppRaytraceView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_EDIT_COPY

[CLS:CB3ShowRaytrace]
Type=0
BaseClass=CB3ShowImage
HeaderFile=b3ShowRaytrace.h
ImplementationFile=b3ShowRaytrace.cpp
Filter=W
LastObject=CB3ShowRaytrace
VirtualFilter=WC

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC
LastObject=CChildFrame

[CLS:CDlgHierarchy]
Type=0
BaseClass=CDialog
HeaderFile=DlgHierarchy.h
ImplementationFile=DlgHierarchy.cpp
LastObject=CDlgHierarchy

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
Filter=T
VirtualFilter=fWC
LastObject=ID_HIERACHY

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_VERSION,static,1342308480
Control3=IDC_COPYRIGHT,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HIERARCHY]
Type=1
Class=CDlgHierarchy
ControlCount=1
Control1=IDC_HIERARCHY,SysTreeView32,1342242831

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
Control16=IDC_PREVIEW_RAYTRACE,static,1342181639
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
Command9=ID_LIGHT_TURN
CommandCount=9

[TB:IDR_TOOLBAR_OBJECT]
Type=1
Class=?
Command1=ID_HIERACHY
Command2=ID_RAYTRACE
Command3=ID_DLG_SCENE
Command4=ID_MODELLER_INFO
Command5=ID_ACTIVATE
Command6=ID_DEACTIVATE
Command7=ID_DEACTIVATE_REST
Command8=ID_ALL_ACTIVATE
Command9=ID_ALL_DEACTIVATE
CommandCount=9

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
Class=CAppLinesApp
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_PREF_SAVE
Command4=ID_PREF_AUTOSAVE
Command5=ID_FILE_PRINT_SETUP
Command6=ID_CHANGE_TEXTURE_PATH
Command7=ID_FILE_MRU_FILE1
Command8=ID_APP_EXIT
Command9=ID_VIEW_TOOLBAR
Command10=IDM_BAR_VIEW
Command11=IDM_BAR_ACTION
Command12=IDM_BAR_CAMERA
Command13=IDM_BAR_LIGHT
Command14=IDM_BAR_DISPLAY
Command15=IDM_BAR_SCENE
Command16=IDM_BAR_FULCRUM
Command17=IDM_BAR_STEP_MOVE
Command18=IDM_BAR_STEP_ROTATE
Command19=ID_CUST_MAIN
Command20=ID_CUST_VIEW
Command21=ID_CUST_ACTION
Command22=ID_CUST_CAMERA
Command23=ID_CUST_LIGHT
Command24=ID_CUST_DISPLAY
Command25=ID_CUST_SCENE
Command26=ID_VIEW_STATUS_BAR
Command27=ID_APP_ABOUT
CommandCount=27

[MNU:IDR_BLZ3TYPE]
Type=1
Class=CMainFrame
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
Command11=ID_CHANGE_TEXTURE_PATH
Command12=ID_FILE_MRU_FILE1
Command13=ID_APP_EXIT
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_CUT
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_ACTIVATE
Command19=ID_DEACTIVATE
Command20=ID_DEACTIVATE_REST
Command21=ID_ALL_ACTIVATE
Command22=ID_OBJ_SELECT
Command23=ID_OBJ_MOVE
Command24=ID_OBJ_ROTATE
Command25=ID_OBJ_SCALE
Command26=ID_CAM_MOVE
Command27=ID_CAM_TURN
Command28=ID_CAM_ROTATE
Command29=ID_CAM_VIEW
Command30=ID_VIEW_TO_FULCRUM
Command31=ID_LIGHT_TURN
Command32=ID_CAMERA_NEW
Command33=ID_CAMERA_DELETE
Command34=ID_CAMERA_PROPERTIES
Command35=ID_VIEW_TO_FULCRUM
Command36=ID_CAMERA_ENABLE
Command37=ID_LIGHT_NEW
Command38=ID_LIGHT_DELETE
Command39=ID_LIGHT_PROPERTIES
Command40=ID_LIGHT_LDC
Command41=ID_LIGHT_ENABLE
Command42=ID_LIGHT_SOFT
Command43=ID_LIGHT_SPOT
Command44=ID_VIEW_TOOLBAR
Command45=IDM_BAR_VIEW
Command46=IDM_BAR_ACTION
Command47=IDM_BAR_CAMERA
Command48=IDM_BAR_LIGHT
Command49=IDM_BAR_DISPLAY
Command50=IDM_BAR_SCENE
Command51=IDM_BAR_FULCRUM
Command52=IDM_BAR_STEP_MOVE
Command53=IDM_BAR_STEP_ROTATE
Command54=ID_CUST_MAIN
Command55=ID_CUST_VIEW
Command56=ID_CUST_ACTION
Command57=ID_CUST_CAMERA
Command58=ID_CUST_LIGHT
Command59=ID_CUST_DISPLAY
Command60=ID_CUST_SCENE
Command61=ID_VIEW_STATUS_BAR
Command62=ID_VIEW_ANTIALIAS
Command63=ID_VIEW_PERSPECTIVE
Command64=ID_VIEW_TOP
Command65=ID_VIEW_FRONT
Command66=ID_VIEW_RIGHT
Command67=ID_VIEW_BACK
Command68=ID_VIEW_LEFT
Command69=ID_VIEW_BIGGER
Command70=ID_VIEW_SMALLER
Command71=ID_VIEW_SELECT
Command72=ID_VIEW_POP
Command73=ID_VIEW_ORIGINAL
Command74=ID_HIERACHY
Command75=ID_RAYTRACE
Command76=ID_DLG_SCENE
Command77=ID_MODELLER_INFO
Command78=ID_WINDOW_NEW
Command79=ID_WINDOW_CASCADE
Command80=ID_WINDOW_TILE_VERT
Command81=ID_WINDOW_TILE_HORZ
Command82=ID_WINDOW_ARRANGE
Command83=ID_APP_ABOUT
CommandCount=83

[MNU:IDR_DISPLAYTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_IMG_SAVE
Command4=ID_PREF_SAVE
Command5=ID_PREF_AUTOSAVE
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_CHANGE_TEXTURE_PATH
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_VIEW_TOOLBAR
Command13=IDM_BAR_VIEW
Command14=IDM_BAR_ACTION
Command15=IDM_BAR_CAMERA
Command16=IDM_BAR_LIGHT
Command17=IDM_BAR_DISPLAY
Command18=IDM_BAR_SCENE
Command19=IDM_BAR_FULCRUM
Command20=IDM_BAR_STEP_MOVE
Command21=IDM_BAR_STEP_ROTATE
Command22=ID_CUST_MAIN
Command23=ID_CUST_VIEW
Command24=ID_CUST_ACTION
Command25=ID_CUST_CAMERA
Command26=ID_CUST_LIGHT
Command27=ID_CUST_DISPLAY
Command28=ID_CUST_SCENE
Command29=ID_VIEW_STATUS_BAR
Command30=ID_B3_UNFILTERED
Command31=ID_B3_FILTERED
Command32=ID_B3_ORIGINAL
Command33=ID_B3_WIDTH
Command34=ID_B3_HEIGHT
Command35=ID_B3_FULL
Command36=ID_B3_MORE
Command37=ID_B3_LESS
Command38=ID_B3_MAGNIFY
Command39=ID_WINDOW_CASCADE
Command40=ID_WINDOW_TILE_VERT
Command41=ID_WINDOW_TILE_HORZ
Command42=ID_WINDOW_ARRANGE
Command43=ID_APP_ABOUT
CommandCount=43

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_ACTIVATE_ALL
Command2=ID_DEACTIVATE_ALL
Command3=ID_EDIT_COPY
Command4=ID_VIEW_ORIGINAL
Command5=ID_FILE_NEW
Command6=ID_FILE_OPEN
Command7=ID_FILE_PRINT
Command8=ID_RAYTRACE
Command9=ID_DEACTIVATE_REST
Command10=ID_FILE_SAVE
Command11=ID_EDIT_PASTE
Command12=ID_VIEW_BIGGER
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_VIEW_MOVE_BOTTOM
Command16=ID_NEXT_PANE
Command17=ID_PREV_PANE
Command18=ID_EDIT_COPY
Command19=ID_EDIT_PASTE
Command20=ID_VIEW_MOVE_LEFT
Command21=ID_VIEW_MOVE_RIGHT
Command22=ID_VIEW_SMALLER
Command23=ID_VIEW_MOVE_TOP
Command24=ID_EDIT_CUT
Command25=ID_EDIT_UNDO
CommandCount=25

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
HeaderFile=..\..\include_win32\blz3\system\b3FloatEdit.h
ImplementationFile=..\system_win32\b3FloatEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=CB3FloatEdit
VirtualFilter=WC

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

[DLG:IDD_SEARCH_PATH_LIST]
Type=1
Class=?
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_PATH_NEW,button,1342242816
Control3=IDC_PATH_DELETE,button,1342242816
Control4=IDC_PATH_FIRST,button,1342242816
Control5=IDC_PATH_LAST,button,1342242816
Control6=IDC_PATH_PREV,button,1342242816
Control7=IDC_PATH_SUCC,button,1342242816
Control8=IDC_PATH_LIST,combobox,1344340033
Control9=IDC_PATH_EDIT,button,1342242816

[TB:IDR_TOOLBAR_CAMERA]
Type=1
Class=?
Command1=ID_CAMERA_NEW
Command2=ID_CAMERA_DELETE
Command3=ID_CAMERA_PROPERTIES
Command4=ID_VIEW_TO_FULCRUM
Command5=ID_CAMERA_ENABLE
Command6=ID_CAM_SELECT
CommandCount=6

[TB:IDR_TOOLBAR_LIGHT]
Type=1
Class=CAppLinesDoc
Command1=ID_LIGHT_NEW
Command2=ID_LIGHT_DELETE
Command3=ID_LIGHT_PROPERTIES
Command4=ID_LIGHT_LDC
Command5=ID_LIGHT_ENABLE
Command6=ID_LIGHT_SOFT
Command7=ID_LIGHT_SPOT
Command8=ID_LIGHT_SELECT
CommandCount=8

[DLG:IDD_NEW_ITEM]
Type=1
Class=CDlgCreateItem
ControlCount=4
Control1=IDC_NEW_ITEM_NAME,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_ITEM_CREATE_LABEL,static,1342308352

[CLS:CDlgCreateItem]
Type=0
HeaderFile=DlgCreateItem.h
ImplementationFile=DlgCreateItem.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgCreateItem

[DLG:IDD_LIGHT]
Type=1
Class=CDlgLight
ControlCount=36
Control1=IDC_LIGHT_ENABLE,button,1342242819
Control2=IDC_LIGHT_SOFT,button,1342242819
Control3=IDC_LIGHT_LDC,button,1342242819
Control4=IDC_LIGHT_POS_X,edit,1350631552
Control5=IDC_LIGHT_POS_Y,edit,1350631552
Control6=IDC_LIGHT_POS_Z,edit,1350631552
Control7=IDC_LIGHT_DIR_X,edit,1350631552
Control8=IDC_LIGHT_DIR_Y,edit,1350631552
Control9=IDC_LIGHT_DIR_Z,edit,1350631552
Control10=IDC_LIGHT_LIST,combobox,1344340289
Control11=IDC_LIGHT_NEW,button,1342242816
Control12=IDC_LIGHT_DELETE,button,1342242816
Control13=IDC_LIGHT_COLOR_CHANGE,button,1342242816
Control14=IDC_LIGHT_DISTANCE,edit,1350631552
Control15=IDC_LIGHT_SOFT_SIZE,edit,1350631552
Control16=IDC_LIGHT_DISTR,msctls_trackbar32,1342242821
Control17=IDOK,button,1342242817
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,button,1342177287
Control28=IDC_LIGHT_COLOR,static,1342181376
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,button,1342177287
Control32=IDC_STATIC,static,1342308352
Control33=IDC_LIGHT_SAMPLE_LABEL,static,1342308352
Control34=IDC_LIGHT_LDC_CONTROL,static,1342181639
Control35=IDC_LIGHT_PREVIEW,static,1342181383
Control36=IDC_LIGHT_LDC_DIAGRAM,static,1342181639

[CLS:CDlgLight]
Type=0
HeaderFile=DlgLight.h
ImplementationFile=DlgLight.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIGHT_COLOR

[CLS:CB3ControlLDC]
Type=0
HeaderFile=b3ControlLDC.h
ImplementationFile=b3ControlLDC.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CB3ControlLDC

[DLG:IDD_LDC]
Type=1
Class=CDlgLDC
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_LIGHT_LDC_CONTROL,static,1342181639
Control3=IDC_LIGHT_PREVIEW,static,1342181383
Control4=IDC_LIGHT_LDC_DIAGRAM,static,1342181639

[CLS:CDlgLDC]
Type=0
HeaderFile=DlgLDC.h
ImplementationFile=DlgLDC.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgLDC

[DLG:IDD_CAMERA]
Type=1
Class=CDlgCamera
ControlCount=28
Control1=IDC_CAMERA_LIST,combobox,1344340289
Control2=IDC_CAMERA_POS_X,edit,1350631552
Control3=IDC_CAMERA_POS_Y,edit,1350631552
Control4=IDC_CAMERA_POS_Z,edit,1350631552
Control5=IDC_CAMERA_VIEW_X,edit,1350631552
Control6=IDC_CAMERA_VIEW_Y,edit,1350631552
Control7=IDC_CAMERA_VIEW_Z,edit,1350631552
Control8=IDC_CAMERA_FOCAL_LENGTH,edit,1350631552
Control9=IDC_CAMERA_TYPES,combobox,1344339971
Control10=IDC_CAMERA_WIDTH,edit,1350631552
Control11=IDC_CAMERA_HEIGHT,edit,1350631552
Control12=IDC_CAMERA_ACTIVATION,button,1342242819
Control13=IDC_CAMERA_NEW,button,1342242816
Control14=IDC_CAMERA_DELETE,button,1342242816
Control15=IDOK,button,1342242817
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,button,1342177287

[CLS:CDlgCamera]
Type=0
HeaderFile=DlgCamera.h
ImplementationFile=DlgCamera.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgCamera
VirtualFilter=dWC


[CLS:CB3ShowImage]
Type=0
HeaderFile=..\..\include_win32\blz3\system\b3ShowImage.h
ImplementationFile=..\system_win32\b3ShowImage.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CB3ShowImage

[DLG:IDD_FULCRUM]
Type=1
Class=CDlgFulcrum
ControlCount=7
Control1=IDC_FULCRUM_X,edit,1350631552
Control2=IDC_FULCRUM_Y,edit,1350631552
Control3=IDC_FULCRUM_Z,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[CLS:CDlgFulcrum]
Type=0
HeaderFile=DlgFulcrum.h
ImplementationFile=DlgFulcrum.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDM_BAR_STEP_ROTATE
VirtualFilter=dWC

[DLG:IDD_STEP_MOVE]
Type=1
Class=CDlgStepMove
ControlCount=7
Control1=IDC_STEP_MOVE_X,edit,1350631552
Control2=IDC_STEP_MOVE_Y,edit,1350631552
Control3=IDC_STEP_MOVE_Z,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_STEP_ROTATE]
Type=1
Class=CDlgStepRotate
ControlCount=7
Control1=IDC_STEP_ROT_X,edit,1350631552
Control2=IDC_STEP_ROT_Y,edit,1350631552
Control3=IDC_STEP_ROT_Z,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[CLS:CDlgStepRotate]
Type=0
HeaderFile=DlgStepRotate.h
ImplementationFile=DlgStepRotate.cpp
BaseClass=CB3Dialogbar
Filter=D
LastObject=CDlgStepRotate
VirtualFilter=dWC

[CLS:CDlgStepMove]
Type=0
HeaderFile=DlgStepMove.h
ImplementationFile=DlgStepMove.cpp
BaseClass=CB3Dialogbar
Filter=D
LastObject=CDlgStepMove
VirtualFilter=dWC

[ACL:IDR_DISPLAYTYPE]
Type=1
Class=?
Command1=ID_IMG_SAVE
Command2=ID_B3_MORE
Command3=ID_B3_LESS
CommandCount=3

[ACL:IDR_BLZ3TYPE]
Type=1
Class=?
Command1=ID_WINDOW_NEW
CommandCount=1

[CLS:CB3Dialogbar]
Type=0
HeaderFile=\blz3\include_win32\blz3\system\b3toolbar.h
ImplementationFile=\blz3\src\system_win32\b3toolbar.cpp
BaseClass=CDialogBar
Filter=D
VirtualFilter=dWC
LastObject=CB3Dialogbar

