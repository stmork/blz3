/*
**
**	$Filename:	AppLinesView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for scenes
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "AppLinesDoc.h"
#include "AppLinesView.h"
#include "MainFrm.h"
#include "b3Action.h"
#include <sys/timeb.h>

#include "DlgCreateItem.h"
#include "DlgCamera.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.47  2002/02/13 16:13:08  sm
**	- Created spotlight view
**	- Changed camera properties dialog to reflect scene units
**	  on example camera settings.
**
**	Revision 1.46  2002/02/12 18:39:02  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.45  2002/02/10 20:03:18  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.44  2002/02/05 20:04:12  sm
**	- Added legend to print hard copy.
**	
**	Revision 1.43  2002/02/04 17:18:00  sm
**	- Added Measurement to modeller info.
**	
**	Revision 1.42  2002/02/03 21:42:30  sm
**	- Added measurement printing. The measure itself is missing yet.
**	  The support is done in b3RenderView and CAppRenderView.
**	- Added support for units in b3ModellerInfo
**	- Cleaned up some accelerators. Now arrow keys are working
**	  again. The del key is working correctly inside edit controls again.
**	
**	Revision 1.41  2002/01/31 11:50:53  sm
**	- Now we can print OpenGL scenes (Note: We have to do basic
**	  initialization prior to render a scene. Then we can see the scene
**	  on paper)
**	
**	Revision 1.40  2002/01/25 16:34:46  sm
**	- Added printer support (not running yet)
**	
**	Revision 1.39  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.38  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.37  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.36  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.35  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.34  2002/01/09 17:47:53  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**	
**	Revision 1.33  2002/01/08 15:45:50  sm
**	- Added support for repeating CButtons for button movement/rotation mode.
**	
**	Revision 1.32  2002/01/05 22:17:47  sm
**	- Recomputing bounding boxes correctly
**	- Found key input bug: The accelerator are the problem
**	- Code cleanup
**	
**	Revision 1.31  2002/01/04 17:53:53  sm
**	- Added new/delete object.
**	- Added deactive rest of all scene objects.
**	- Changed icons to reflect object activation.
**	- Sub object insertion added.
**	- Fixed update routines to reflect correct state in hierarchy.
**	- Better hierarchy update coded.
**	
**	Revision 1.30  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.29  2001/12/30 18:24:35  sm
**	- Added missing b3AnimControl class
**	- Some minor bug fixes done:
**	  o Missed some SetModifiedFlag()
**	  o b3Scene::b3SetCamera() calls added which now puts the
**	    selected camera in front of the b3Special list so that Lines III
**	    select it when reloading.
**	
**	Revision 1.28  2001/12/28 15:17:44  sm
**	- Added clipboard-copy to raytraced view
**	- Added printing to raytraced view
**	- Much minor UI tuning done:
**	  o added accelerators
**	  o open maximized window
**	  o fixed some UpdateUI methods
**	  o changed exception handling in CB3ScrollView and CB3BitmapDxB
**	
**	Revision 1.27  2001/12/27 21:33:35  sm
**	- Further docking handling done
**	- CDocument cleanups done
**	
**	Revision 1.26  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.25  2001/12/26 12:00:36  sm
**	- Fixed modeller info dialog
**	
**	Revision 1.24  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.23  2001/12/22 21:08:35  sm
**	- Tidied up some dialogs
**	- Designed new icons for document templates
**	- Toolbars got adjusted and beautified
**	- Introduced b3Scene::b3IsObscured() for faster Phong illumination
**	- Found and fixed some minor bugs
**	
**	Revision 1.22  2001/12/21 16:46:16  sm
**	- New dialog for camera properties
**	- Done some bugfixes concerning CB3FloatEdit
**	
**	Revision 1.21  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.20  2001/11/30 18:08:00  sm
**	- View to fulcrum implemented
**	- Some menus updated
**	
**	Revision 1.19  2001/11/25 19:20:32  sm
**	- Added new acting methods:
**	  o Camera move
**	  o Camera turn around itself
**	  o Camera rotate around fulcrum
**	
**	Revision 1.18  2001/11/11 11:51:20  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.17  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	Revision 1.16  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.15  2001/11/01 13:22:43  sm
**	- Introducing performance meter
**	
**	Revision 1.14  2001/09/05 15:21:34  sm
**	- Now object moving/rotating on perspective view.
**	
**	Revision 1.13  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**	
**	Revision 1.12  2001/08/21 14:24:14  sm
**	- New selecting code added.
**	
**	Revision 1.11  2001/08/20 19:35:08  sm
**	- Index correction introduced (This is a hack!)
**	- Some OpenGL cleanups
**	
**	Revision 1.10  2001/08/20 14:16:48  sm
**	- Putting data into cmaera and light combobox.
**	- Selecting camera and light.
**	
**	Revision 1.9  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**	
**	Revision 1.8  2001/08/14 13:34:39  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**	
**	Revision 1.7  2001/08/13 15:05:01  sm
**	- Now we can scale and move around with stacked views.
**	
**	Revision 1.6  2001/08/12 19:47:47  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**	
**	Revision 1.5  2001/08/11 19:59:15  sm
**	- Added orthogonal projection
**	
**	Revision 1.4  2001/08/11 16:29:07  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**	
**	Revision 1.3  2001/08/11 15:59:58  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**	
**	Revision 1.2  2001/08/10 18:28:58  sm
**	- Some bug fixing
**	- Update functions per view inserted. Now with redraw when loading.
**	
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesView implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppLinesView, CAppRenderView)

BEGIN_MESSAGE_MAP(CAppLinesView, CAppRenderView)
	//{{AFX_MSG_MAP(CAppLinesView)
	ON_COMMAND(ID_OBJECT_SELECT, OnObjSelect)
	ON_COMMAND(ID_OBJECT_MOVE, OnObjMove)
	ON_COMMAND(ID_OBJECT_ROTATE, OnObjRotate)
	ON_COMMAND(ID_OBJECT_SCALE, OnObjScale)
	ON_COMMAND(ID_CAMERA_MOVE, OnCamMove)
	ON_COMMAND(ID_CAMERA_TURN, OnCamTurn)
	ON_COMMAND(ID_CAMERA_ROTATE, OnCamRotate)
	ON_COMMAND(ID_CAMERA_VIEW, OnCamView)
	ON_CBN_SELCHANGE(ID_CAMERA_SELECT, OnCamSelect)
	ON_COMMAND(ID_LIGHT_TURN, OnLightTurn)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SELECT, OnUpdateObjSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVE, OnUpdateObjMove)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ROTATE, OnUpdateObjRotate)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SCALE, OnUpdateObjScale)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_MOVE, OnUpdateCamMove)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_TURN, OnUpdateCamTurn)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROTATE, OnUpdateCamRotate)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_VIEW, OnUpdateCamView)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_TURN, OnUpdateLightTurn)
	ON_COMMAND(ID_VIEW_TO_FULCRUM, OnViewToFulcrum)
	ON_COMMAND(ID_CAMERA_NEW, OnCameraNew)
	ON_COMMAND(ID_CAMERA_DELETE, OnCameraDelete)
	ON_COMMAND(ID_CAMERA_PROPERTIES, OnCameraProperties)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_DELETE, OnUpdateCameraDelete)
	ON_COMMAND(ID_CAMERA_ENABLE, OnCameraEnable)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ENABLE, OnUpdateCameraEnable)
	ON_BN_CLICKED(IDC_MOVE_LEFT, OnMoveLeft)
	ON_BN_CLICKED(IDC_MOVE_RIGHT, OnMoveRight)
	ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
	ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_ROT_LEFT, OnRotateLeft)
	ON_BN_CLICKED(IDC_ROT_RIGHT, OnRotateRight)
	ON_UPDATE_COMMAND_UI(IDC_MOVE_LEFT, OnUpdateMovement)
	ON_UPDATE_COMMAND_UI(IDC_MOVE_RIGHT, OnUpdateMovement)
	ON_UPDATE_COMMAND_UI(IDC_MOVE_UP, OnUpdateMovement)
	ON_UPDATE_COMMAND_UI(IDC_MOVE_DOWN, OnUpdateMovement)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView construction/destruction

CAppLinesView::CAppLinesView()
{
	// TODO: add construction code here
}

CAppLinesView::~CAppLinesView()
{
	m_PickBaseLight.b3Free();
}

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView drawing

/////////////////////////////////////////////////////////////////////////////
// CAppLinesView diagnostics

#ifdef _DEBUG
void CAppLinesView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppLinesView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppLinesDoc* CAppLinesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)));
	return (CAppLinesDoc*)m_pDocument;
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        Update handling                               **
**                                                                      **
*************************************************************************/

void CAppLinesView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppLinesDoc *pDoc = GetDocument();

	m_Scene      = pDoc->m_Scene;
	B3_ASSERT(m_Scene != null);
	m_Camera     = m_Scene->b3GetCamera(true);

	m_Action[B3_SELECT_MAGNIFICATION] = new CB3ActionMagnify(this);
	m_Action[B3_OBJECT_SELECT]        = new CB3ActionObjectSelect(this);
	m_Action[B3_OBJECT_MOVE]          = new CB3ActionObjectMove(this);
	m_Action[B3_OBJECT_ROTATE]        = new CB3ActionObjectRotate(this);
	m_Action[B3_OBJECT_SCALE]         = new CB3ActionObjectScale(this);
	m_Action[B3_CAMERA_MOVE]          = new CB3ActionCameraMove(this);
	m_Action[B3_CAMERA_TURN]          = new CB3ActionCameraTurn(this);
	m_Action[B3_CAMERA_ROTATE]        = new CB3ActionCameraRotate(this);
	m_Action[B3_CAMERA_VIEW]          = new CB3ActionCameraView(this);
	m_Action[B3_LIGHT_TURN]           = new CB3ActionLightTurn(this);

	CAppRenderView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppLinesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAppLinesDoc *pDoc   = GetDocument();
	b3CameraPart *camera = pDoc->m_CameraLight;
	b3Item       *item;
	b3Light      *light;
	b3Pick       *pick;
	b3_bool       doInvalidate = false;

	if (lHint & B3_UPDATE_PICK)
	{
		m_PickBaseLight.b3Update();
		doInvalidate = true;
	}

	if (lHint & B3_UPDATE_LIGHT)
	{
		b3_vector  look;
		b3_f64     len;

		light = pDoc->m_Light;
		len   = b3Vector::b3Length(&light->m_Direction);
		b3Vector::b3Add(&light->m_Position,&light->m_Direction,&look);
		camera->b3Orientate(&light->m_Position,&look,len,len,len);
		lHint |= B3_UPDATE_LIGHTVIEW;
		doInvalidate = true;
	}

	if(lHint & B3_UPDATE_LIGHTVIEW)
	{
		if (m_SelectMode == B3_LIGHT_TURN)
		{
			// Supercede camera handling
			lHint &= (~B3_UPDATE_CAMERA);

			// Setup display
			m_RenderView.b3SetCamera(camera);

			// Update light itself
			b3Vector::b3Sub(&camera->m_ViewPoint,&camera->m_EyePoint,&pDoc->m_Light->m_Direction);
		}
		doInvalidate = true;
	}

	CAppRenderView::OnUpdate(pSender,lHint,pHint);

	// We have to update the pick list if we change view.
	if ((lHint & (B3_UPDATE_LIGHT|B3_UPDATE_VIEW)) && !m_PickBaseLight.b3IsActive())
	{
		// Update on changed light _or_ view!
		m_PickBaseLight.b3Free();
		if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
		{
			B3_FOR_BASE(m_Scene->b3GetLightHead(),item)
			{
				light = (b3Light *)item;
				if (light->m_LightActive)
				{
					pick = new b3PickPoint(
						&m_RenderView,
						&light->m_Position,light->b3GetName(),
						pDoc->m_Info);
					m_PickBaseLight.b3Append(pick);

					if (light->m_SpotActive)
					{
						pick = new b3PickDir(
							&m_RenderView,
							&light->m_Position,&light->m_Direction,null,
							pDoc->m_Info);
						m_PickBaseLight.b3Append(pick);
					}
				}
			}
		}
		doInvalidate = true;
	}

	if (doInvalidate)
	{
		Invalidate();
	}
}

/*************************************************************************
**                                                                      **
**                        Handle picking                                **
**                                                                      **
*************************************************************************/

void CAppLinesView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_PickBaseLight.b3Down(point.x,point.y))
	{
		// Do standard action
		CAppRenderView::OnLButtonDown(nFlags, point);
	}
	else
	{
		// Do MFC mouse handling when picking
		SetCapture();
		CScrollView::OnLButtonDown(nFlags, point);
	}
}

void CAppLinesView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_PickBaseLight.b3IsActive())
	{
		// Do standard action
		CAppRenderView::OnMouseMove(nFlags, point);
	}
	else
	{
		// Do MFC mouse handling when picking
		if (m_PickBaseLight.b3Move(point.x,point.y))
		{
			CAppLinesDoc *pDoc = GetDocument();

			pDoc->UpdateAllViews(NULL,B3_UPDATE_PICK | B3_UPDATE_LIGHT);
			pDoc->SetModifiedFlag();
		}
		CScrollView::OnMouseMove(nFlags, point);
	}
}

void CAppLinesView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_PickBaseLight.b3Up(point.x,point.y))
	{
		// Do standard action
		CAppRenderView::OnLButtonUp(nFlags, point);
	}
	else
	{
		// Do MFC mouse handling when picking
		::ReleaseCapture();
		CScrollView::OnLButtonUp(nFlags, point);
	}
}

b3_bool CAppLinesView::b3GetDimension(
	b3_f64 &xSize,
	b3_f64 &ySize,
	b3_f64 &unit,
	b3_f64 &measure)
{
	b3_bool success;

	success = m_RenderView.b3GetDimension(xSize,ySize);
	if (success)
	{
		CAppLinesDoc *pDoc = GetDocument();
		
		unit    = pDoc->m_Info->b3ScaleUnitToMM();
		measure = pDoc->m_Info->b3GetMeasure(false);
	}
	return success;
}

void CAppLinesView::b3Draw(
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
	CAppLinesDoc   *pDoc = GetDocument();
	b3ModellerInfo *info = pDoc->m_Info;

	pDoc->m_Context.b3StartDrawing();

	// Setup view first
	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);
	m_RenderView.b3DrawRaster(info->b3ScaleUnitToMM() * info->b3GetMeasure(false),0.7);
	if (pDoc->m_Info->m_GridActive)
	{
		m_RenderView.b3DrawRaster(pDoc->m_Info->m_GridMove);
	}

	// Then draw objects
	m_Scene->b3Draw();
	if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_CameraVolume.b3Draw();
	}
	pDoc->b3DrawFulcrum();
}

void CAppLinesView::b3DrawDC(
	HDC    hDC,
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
	CDC      *dc = CDC::FromHandle(hDC);
	CPen     *old,red;
	LOGBRUSH  logbrush;
	DWORD     stipple[2];

	// Setup view first
	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

	// Create pen fpr dashed line (pattern: **..**..**)
	stipple[0] = 1; // Set 2 pixel (2 - 1)
	stipple[1] = 3; // Unset 2 pixel (2 + 1) - and so on

	// Create brush entry for color
	logbrush.lbColor = RGB(0xff,0x11,0x44);
	logbrush.lbHatch = 0;
	logbrush.lbStyle = BS_SOLID;

	// Create pen from brush and pattern (I don't know what is Windows is doing here...)
	red.CreatePen(PS_USERSTYLE|PS_SOLID|PS_GEOMETRIC,1,&logbrush,2,stipple);

	// Set attributes to DC
	old = dc->SelectObject(&red);
	dc->SetROP2(R2_COPYPEN);
	dc->SetTextColor(RGB(0xff,0x11,0x44));
	dc->SetBkMode(TRANSPARENT);

	// Draw pick points
	m_PickBaseLight.b3Draw(dc);

	// Puhh! And only to draw stippled lines...
	dc->SelectObject(old);
}

void CAppLinesView::b3DrawLegend(CDC *pDC)
{
	CString         text;
	b3ModellerInfo *info = GetDocument()->m_Info;
	time_t          timecode;

	text.LoadString(IDS_PRT_LEGEND);
	b3DrawText(pDC,text);

	text.Format(IDS_PRT_FILENAME,m_Scene->b3GetName());
	b3DrawText(pDC,text);

	if (m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		text.Format(IDS_PRT_VIEWNAME,m_Camera->b3GetName());
		b3DrawText(pDC,text);
	}

	time(&timecode);
	text.Format(IDS_PRT_DATE,asctime(localtime(&timecode)));
	b3DrawText(pDC,text);

	text.Format(IDS_PRT_MEASURE,info->b3GetMeasure(false));
	b3DrawText(pDC,text);

	text.Format(IDS_PRT_UNIT,info->b3GetUnitDescr());
	b3DrawText(pDC,text);
}

void CAppLinesView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3App     *app  = CB3GetApp();
	CMainFrame *main = CB3GetMainFrame();

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"CAppLinesView::OnActivateView(%s,%p,%p)\n",
		bActivate ? "TRUE " : "FALSE",
		pActivateView, pDeactiveView);
#endif
	
	if (bActivate)
	{
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		main->b3UpdateLightBox(m_Scene,GetDocument()->m_Light);
		main->b3UpdateModellerInfo(GetDocument());
		m_Scene->b3SetCamera(m_Camera,true);
		app->b3SetData();
	}
	else
	{
//		main->b3Clear();
//		main->b3UpdateModellerInfo();
		app->b3GetData();
	}
}

b3_bool CAppLinesView::b3IsMouseActionAllowed()
{
	return !GetDocument()->b3IsRaytracing();
}

void CAppLinesView::OnObjSelect() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_SELECT;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnObjMove() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_MOVE;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnObjRotate() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_ROTATE;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnObjScale() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_OBJECT_SCALE;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnCamMove() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_MOVE;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnCamTurn() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_TURN;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnCamRotate() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_ROTATE;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnCamView() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_CAMERA_VIEW;
	OnUpdate(this,B3_UPDATE_CAMERA,NULL);
}

void CAppLinesView::OnLightTurn() 
{
	// TODO: Add your command handler code here
	m_SelectMode = B3_LIGHT_TURN;
	m_RenderView.b3SetViewMode(B3_VIEW_3D);
	OnUpdate(this,B3_UPDATE_CAMERA | B3_UPDATE_LIGHT,NULL);
}

void CAppLinesView::OnUpdateObjSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_SELECT);
}

void CAppLinesView::OnUpdateObjMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_MOVE);
}

void CAppLinesView::OnUpdateObjRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_ROTATE);
}

void CAppLinesView::OnUpdateObjScale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_OBJECT_SCALE);
}

void CAppLinesView::OnUpdateCamMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_MOVE);
}

void CAppLinesView::OnUpdateCamTurn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_TURN);
}

void CAppLinesView::OnUpdateCamRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_ROTATE);
}

void CAppLinesView::OnUpdateCamView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_CAMERA_VIEW);
}

void CAppLinesView::OnUpdateLightTurn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->SetRadio(m_SelectMode == B3_LIGHT_TURN);
}

void CAppLinesView::OnCamSelect() 
{
	// TODO: Add your command handler code here
	CMainFrame   *main = CB3GetMainFrame();
	b3CameraPart *new_camera;

	new_camera = main->b3GetSelectedCamera();
	if (m_Camera != new_camera)
	{
		m_Camera = new_camera;
		m_Scene->b3SetCamera(m_Camera,true);
		GetDocument()->SetModifiedFlag();
		OnUpdate(this,B3_UPDATE_CAMERA,NULL);
	}
}

void CAppLinesView::OnViewToFulcrum() 
{
	// TODO: Add your command handler code here
	CAppLinesDoc *pDoc = GetDocument();

	m_Camera->b3Orientate(
		&m_Camera->m_EyePoint,
		pDoc->b3GetFulcrum(),
		b3Vector::b3Distance(&m_Camera->m_ViewPoint,&m_Camera->m_EyePoint),
		b3Vector::b3Length(&m_Camera->m_Width),
		b3Vector::b3Length(&m_Camera->m_Height));
	if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_RenderView.b3SetViewMode(B3_VIEW_3D);
		b3UnsetMagnification();
		OnUpdate(this,B3_UPDATE_VIEW|B3_UPDATE_CAMERA,0);
	}
	else
	{

		OnUpdate(this,B3_UPDATE_CAMERA,0);
	}
	pDoc->SetModifiedFlag();
}

void CAppLinesView::OnCameraNew() 
{
	// TODO: Add your command handler code here
	CDlgCreateItem  dlg;
	CMainFrame     *main;
	b3CameraPart   *camera;

	dlg.m_Label.LoadString(IDS_NEW_CAMERA);
	dlg.m_ItemBase   = m_Scene->b3GetSpecialHead();
	dlg.m_ClassType  = CAMERA;
	dlg.m_MaxNameLen = B3_CAMERANAMELEN;
	dlg.m_Suggest    = m_Camera->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		camera = new b3CameraPart(CAMERA);
		camera->m_EyePoint  = m_Camera->m_EyePoint;
		camera->m_ViewPoint = m_Camera->m_ViewPoint;
		camera->m_Width     = m_Camera->m_Width;
		camera->m_Height    = m_Camera->m_Height;
		camera->m_Flags     = CAMERA_ACTIVE;
		strcpy (camera->b3GetName(),dlg.m_NewName);
		m_Scene->b3GetSpecialHead()->b3Append(m_Camera = camera);
		m_Scene->b3SetCamera(m_Camera,true);

		GetDocument()->SetModifiedFlag();
		main = CB3GetMainFrame();
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		OnUpdate(this,B3_UPDATE_CAMERA,0);
	}
}

void CAppLinesView::OnCameraDelete() 
{
	// TODO: Add your command handler code here
	CMainFrame   *main;
	b3CameraPart *camera,*select;

	if (AfxMessageBox(IDS_ASK_DELETE_CAMERA,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		main = CB3GetMainFrame();
		
		camera = main->b3GetSelectedCamera();
		select = m_Scene->b3GetNextCamera(camera);
		if (select == null)
		{
			select = m_Scene->b3GetCamera();
		}
		m_Scene->b3GetSpecialHead()->b3Remove(camera);
		delete camera;

		GetDocument()->SetModifiedFlag();
		main->b3UpdateCameraBox(m_Scene,m_Camera = select);
		m_Scene->b3SetCamera(m_Camera,true);
		OnUpdate(this,B3_UPDATE_CAMERA,0);
	}
}

void CAppLinesView::OnCameraProperties() 
{
	// TODO: Add your command handler code here
	CMainFrame *main;
	CDlgCamera  dlg;

	dlg.m_Scene  = m_Scene;
	dlg.m_Camera = m_Camera;
	if (dlg.DoModal() == IDOK)
	{
		main = CB3GetMainFrame();

		GetDocument()->SetModifiedFlag();
		m_Camera = dlg.m_Camera;
		m_Scene->b3SetCamera(m_Camera,true);
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		OnUpdate(this,B3_UPDATE_CAMERA,0);
	}
}

void CAppLinesView::OnCameraEnable() 
{
	// TODO: Add your command handler code here
	m_Camera->b3Activate(!m_Camera->b3IsActive());
	GetDocument()->SetModifiedFlag();
}

void CAppLinesView::OnUpdateCameraDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	b3Item   *item;
	b3_count  count= 0;

	B3_FOR_BASE(m_Scene->b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			count++;
		}
	}
	pCmdUI->Enable((count > 1) && (!GetDocument()->b3IsRaytracing()));
}

void CAppLinesView::OnUpdateCameraEnable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_Camera->b3IsActive());
	pCmdUI->Enable(!GetDocument()->b3IsRaytracing());
}

void CAppLinesView::b3Move(b3_action_mode mode)
{
	CAppLinesDoc *pDoc = GetDocument();
	b3_vector     stepper;
	b3_matrix     transformation;

	m_RenderView.b3SetTranslationStepper(
		pDoc->b3GetStepMove(),&stepper,mode);
	b3MatrixMove(null,&transformation,&stepper);
	m_Scene->b3Transform(&transformation);
	pDoc->b3ComputeBounds();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(null,B3_UPDATE_GEOMETRY);
}

void CAppLinesView::b3Rotate(b3_action_mode mode)
{
	CAppLinesDoc *pDoc = GetDocument();
	b3_line       axis;
	b3_matrix     transformation;
	b3_f64        angle;

	axis.pos = *pDoc->b3GetFulcrum();
	angle    = m_RenderView.b3SetRotationStepper(
		pDoc->b3GetStepRotate(),&axis.dir,mode);
	b3MatrixRotVec(null,&transformation,&axis,angle * M_PI / 180);
	m_Scene->b3Transform(&transformation);
	pDoc->b3ComputeBounds();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(null,B3_UPDATE_GEOMETRY);
}

void CAppLinesView::OnMoveLeft() 
{
	// TODO: Add your control notification handler code here
	b3Move(B3_ACTION_MOVE_LEFT);
}

void CAppLinesView::OnMoveRight() 
{
	// TODO: Add your control notification handler code here
	b3Move(B3_ACTION_MOVE_RIGHT);
}

void CAppLinesView::OnMoveDown() 
{
	// TODO: Add your control notification handler code here
	b3Move(B3_ACTION_MOVE_DOWN);
}

void CAppLinesView::OnMoveUp() 
{
	// TODO: Add your control notification handler code here
	b3Move(B3_ACTION_MOVE_UP);
}

void CAppLinesView::OnRotateLeft() 
{
	// TODO: Add your control notification handler code here
	b3Rotate(B3_ACTION_ROT_LEFT);
}

void CAppLinesView::OnRotateRight() 
{
	// TODO: Add your control notification handler code here
	b3Rotate(B3_ACTION_ROT_RIGHT);
}

void CAppLinesView::OnUpdateMovement(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_RenderView.b3IsViewMode(B3_VIEW_3D));
}
