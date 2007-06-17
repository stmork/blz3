/*
**
**	$Filename:	AppLinesView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"
#include "b3Action.h"
#include <sys/timeb.h>

#include "DlgCamera.h"

#include "b3UndoAction.h"
#include "b3UndoCamera.h"
#include "b3UndoPick.h"

/*************************************************************************
**                                                                      **
**                        CAppLinesView implementation                  **
**                                                                      **
*************************************************************************/

b3Color CAppLinesView::m_GridColorUnit(0.7f, 0.7f, 0.7f);
b3Color CAppLinesView::m_GridColorMove(0.75f,0.75f,0.75f);

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
	m_Camera     = m_Scene->b3GetFirstCamera(true);

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

void CAppLinesView::b3UpdateLight()
{
	CAppLinesDoc *pDoc = GetDocument();

	pDoc->m_Context.b3LightReset();
	if (pDoc->m_Scene->b3GetModellerInfo()->m_UseSceneLights)
	{
		m_RenderLight.b3SetScene(pDoc->m_Scene);
		m_RenderLight.b3SetLightMode(B3_LIGHT_SCENE_SPOT);
	}
	else
	{
		m_RenderLight.b3SetLightMode(B3_LIGHT_SIMPLE);
	}
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
	b3UndoOperation *op = m_PickBaseLight.b3GetOperation(null);

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

		if (op != null)
		{
			GetDocument()->b3AddOp(op);
		}
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

	// Setup view first
	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

	// then setup light(s)
	m_RenderLight.b3SetupLight(&pDoc->m_Context);

	// Clear buffer
	pDoc->m_Context.b3StartDrawing();

	// Draw background grid
	m_RenderView.b3DrawRaster(info->b3ScaleUnitToMM() * info->b3GetMeasure(false),m_GridColorUnit);
	if (pDoc->m_Info->m_GridActive)
	{
		m_RenderView.b3DrawRaster(pDoc->m_Info->m_GridMove,m_GridColorMove);
	}

	// Then draw objects
	m_Scene->b3Draw(&pDoc->m_Context);

	// Draw camera volume
	if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_CameraVolume.b3Draw(&pDoc->m_Context);
	}

	// Draw fulcrum
	pDoc->b3DrawFulcrum();
}

void CAppLinesView::b3DrawDC(
	HDC    hDC,
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
	CDC        *dc = CDC::FromHandle(hDC);
	CPen       *old;

	// Setup view first
	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

	// Set attributes to DC
	old = dc->SelectObject(&m_RedDash);
	dc->SetROP2(R2_COPYPEN);
	dc->SetTextColor(b3PickPoint::m_PickColor);
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

	CAppRenderView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CB3LinesDocument::b3HandleBarState(this,bActivate);

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

/*************************************************************************
**                                                                      **
**                        Camera actions                                **
**                                                                      **
*************************************************************************/

void CAppLinesView::OnCamSelect() 
{
	// TODO: Add your command handler code here
	CMainFrame   *main = CB3GetMainFrame();
	b3CameraPart *new_camera;

	new_camera = main->b3GetSelectedCamera();
	if (m_Camera != new_camera)
	{
		m_Camera = new_camera;
		m_Scene->b3SetCamera(m_Camera,true); // set and resort
		GetDocument()->SetModifiedFlag();
		OnUpdate(this,B3_UPDATE_CAMERA,NULL);
	}
}

void CAppLinesView::OnViewToFulcrum() 
{
	// TODO: Add your command handler code here
	CAppLinesDoc *pDoc = GetDocument();

	pDoc->b3AddOp(new b3OpCameraOrientate(m_Scene,m_Camera,pDoc->b3GetFulcrum()));
	if (!m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_RenderView.b3SetViewMode(B3_VIEW_3D);
		b3UnsetMagnification();
		OnUpdate(this,B3_UPDATE_VIEW,0);
	}
}

void CAppLinesView::OnCameraNew() 
{
	// TODO: Add your command handler code here
	CAppLinesDoc    *pDoc = GetDocument();
	b3UndoOperation *op = new b3OpCameraCreate(m_Scene,m_Camera);

	pDoc->b3AddOp(op);
}

void CAppLinesView::OnCameraDelete() 
{
	// TODO: Add your command handler code here
	CAppLinesDoc    *pDoc = GetDocument();
	b3UndoOperation *op = new b3OpCameraDelete(m_Scene,m_Camera);

	pDoc->b3AddOp(op);
}

void CAppLinesView::OnCameraProperties() 
{
	// TODO: Add your command handler code here
	CDlgCamera     dlg;

	dlg.m_Scene  = m_Scene;
	dlg.m_Camera = m_Camera;
	if (dlg.DoModal() == IDOK)
	{
		CMainFrame    *main = CB3GetMainFrame();
		CAppRenderDoc *pDoc = GetDocument();

		pDoc->b3ClearOp();
		pDoc->SetModifiedFlag();
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		OnUpdate(this,B3_UPDATE_CAMERA,0);
	}
}

void CAppLinesView::OnCameraEnable() 
{
	// TODO: Add your command handler code here
	GetDocument()->b3AddOp(new b3OpCameraEnable(m_Scene,m_Camera));
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

/*************************************************************************
**                                                                      **
**                        Object moving/rotating via                    **
**                        separated buttons                             **
**                                                                      **
*************************************************************************/

void CAppLinesView::b3Move(b3_action_mode mode)
{
	CAppLinesDoc *pDoc = GetDocument();
	b3_vector     stepper;

	m_RenderView.b3SetTranslationStepper(
		pDoc->b3GetStepMove(),&stepper,mode);
	pDoc->b3AddOp(new b3OpMove(m_Scene,&stepper));
}

void CAppLinesView::b3Rotate(b3_action_mode mode)
{
	CAppLinesDoc *pDoc = GetDocument();
	b3_line       axis;
	b3_f64        angle;

	axis.pos = *pDoc->b3GetFulcrum();
	angle    = m_RenderView.b3SetRotationStepper(
		pDoc->b3GetStepRotate(),&axis.dir,mode);
	pDoc->b3AddOp(new b3OpRotate(m_Scene,&axis,angle));
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
