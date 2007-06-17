/*
**
**	$Filename:	AppObjectView.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CView part for objects
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "DlgObjectPreview.h"
#include "b3Action.h"
#include <sys/timeb.h>

/*************************************************************************
**                                                                      **
**                        CAppObjectView implementation                 **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppObjectView, CAppRenderView)

BEGIN_MESSAGE_MAP(CAppObjectView, CAppRenderView)
	//{{AFX_MSG_MAP(CAppObjectView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_RAYTRACE, OnRaytrace)
	ON_UPDATE_COMMAND_UI(ID_RAYTRACE, OnUpdateRaytrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView construction/destruction

CAppObjectView::CAppObjectView() : m_PickList(&m_RenderView)
{
	// TODO: add construction code here
	b3_index i;

	m_PreviousMode =
	m_SelectMode   = B3_SHAPE_MOVE;
	for (i = 0;i < B3_MODE_MAX;i++)
	{
		m_Action[i] = null;
	}
	m_BBox   = null;
	m_Camera = new b3CameraPart(CAMERA);
	m_xAngle = 225 * M_PI / 180;
	m_yAngle =  30 * M_PI / 180;
}

CAppObjectView::~CAppObjectView()
{
	delete m_Camera;
}

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView drawing

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView diagnostics

#ifdef _DEBUG
void CAppObjectView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAppObjectView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAppObjectDoc* CAppObjectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAppObjectDoc)));
	return (CAppObjectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppObjectView message handlers

void CAppObjectView::OnInitialUpdate()
{
	// Do necessary Blizzard III stuff!
	CAppObjectDoc *pDoc = GetDocument();

	m_Action[B3_SELECT_MAGNIFICATION] = new CB3ActionMagnify(this);
	m_Action[B3_SHAPE_MOVE]           = new CB3ActionShapeMove(this);
	m_Action[B3_SHAPE_ROTATE_POINT]   = new CB3ActionShapeRotatePoint(this);
	m_Action[B3_SHAPE_ROTATE_AXIS]    = new CB3ActionShapeRotateAxis(this);
	m_Action[B3_SHAPE_SCALE]          = new CB3ActionShapeScale(this);
	m_Action[B3_SHAPE_MIRROR_POINT]   = new CB3ActionShapeMirrorPoint(this);
	m_Action[B3_SHAPE_MIRROR_AXIS]    = new CB3ActionShapeMirrorAxis(this);
	m_Action[B3_SHAPE_MIRROR_AREA]    = new CB3ActionShapeMirrorArea(this);

	CAppRenderView::OnInitialUpdate();

	// TODO: calculate the total size of this view
//	OnUpdate(this,B3_UPDATE_ALL,0);
}

void CAppObjectView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	b3_bool     doInvalidate = false;
	b3_vector   center;

	if (lHint & B3_UPDATE_OBJECT)
	{
		m_BBox = GetDocument()->m_BBox;
	}

	if ((lHint & B3_UPDATE_CAMERA) && (m_BBox != null))
	{
		center.x = m_BBox->m_DimBase.x + 0.5 * m_BBox->m_DimSize.x;
		center.y = m_BBox->m_DimBase.y + 0.5 * m_BBox->m_DimSize.y;
		center.z = m_BBox->m_DimBase.z + 0.5 * m_BBox->m_DimSize.z;
		m_Camera->b3Overview(&center,&m_BBox->m_DimSize,m_xAngle,m_yAngle);
		doInvalidate = true;
	}

	if (lHint & B3_UPDATE_PICK)
	{
		CAppObjectDoc *pDoc    = GetDocument();
		b3_bool        result;

		result = m_PickList.b3SetShape(pDoc->b3GetSelectedShape());
		m_PickList.b3SetupVertexMemory(&pDoc->m_Context);
		if (result)
		{
			doInvalidate = true;
		}
	}

	if (doInvalidate)
	{
		Invalidate();
	}

	CAppRenderView::OnUpdate(pSender,lHint,pHint);
}

b3_bool CAppObjectView::b3GetDimension(
	b3_f64 &xSize,
	b3_f64 &ySize,
	b3_f64 &unit,
	b3_f64 &measure)
{
	b3_bool success;

	success = m_RenderView.b3GetDimension(xSize,ySize);
	if (success)
	{
		CAppObjectDoc *pDoc = GetDocument();
		
		unit    = pDoc->m_Info->b3ScaleUnitToMM();
		measure = pDoc->m_Info->b3GetMeasure(false);
	}
	return success;
}

void CAppObjectView::b3Draw(
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
	// We have already an HDC, you remember?
	// So we don't need OnDraw();

	if (m_BBox != null)
	{
		CAppObjectDoc   *pDoc    = GetDocument();
		b3RenderContext *context = &pDoc->m_Context;

		// Setup view first
		m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

		// then setup light(s)
		m_RenderLight.b3SetupLight(&pDoc->m_Context);

		// Clear buffer
		context->b3StartDrawing();

		// Then draw objects
		m_BBox->b3Draw(context);

		// Draw fulcrum
		pDoc->b3DrawFulcrum();

		// Draws pick controls
		m_PickList.b3RenderObject::b3Draw(context);
	}
	else
	{
		ValidateRect(NULL);
	}
}

void CAppObjectView::b3DrawDC(
	HDC    hDC,
	b3_res xSize,
	b3_res ySize,
	b3_f64 xOffset,
	b3_f64 yOffset)
{
	CDC  *dc = CDC::FromHandle(hDC);
	CPen *old;

	// Setup view first
//	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

	// Set attributes to DC
	old = dc->SelectObject(&m_RedDash);
	dc->SetROP2(R2_COPYPEN);
	dc->SetTextColor(b3PickPoint::m_PickColor);
	dc->SetBkMode(TRANSPARENT);

	// Draw pick points
	m_PickList.b3PickBase::b3Draw(dc);

	// Puhh! And only to draw stippled lines...
	dc->SelectObject(old);
}

void CAppObjectView::b3DrawLegend(CDC *pDC)
{
	CString         text;
	CAppObjectDoc  *pDoc   = GetDocument();
	b3ModellerInfo *info   = pDoc->m_Info;
	b3Scene        *parent = pDoc->b3GetParentScene();
	time_t          timecode;

	text.LoadString(IDS_PRT_LEGEND);
	b3DrawText(pDC,text);

	if (parent != null)
	{
		text.Format(IDS_PRT_FILENAME,parent->b3GetName());
		b3DrawText(pDC,text);
	}

	text.Format(IDS_PRT_OBJECTNAME,pDoc->m_BBox->b3GetName());
	b3DrawText(pDC,text);

	time(&timecode);
	text.Format(IDS_PRT_DATE,asctime(localtime(&timecode)));
	b3DrawText(pDC,text);

	text.Format(IDS_PRT_MEASURE,info->b3GetMeasure(false));
	b3DrawText(pDC,text);

	text.Format(IDS_PRT_UNIT,info->b3GetUnitDescr());
	b3DrawText(pDC,text);
}

void CAppObjectView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3App     *app  = CB3GetApp();
	CMainFrame *main = CB3GetMainFrame();

	CAppRenderView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CB3LinesDocument::b3HandleBarState(this,bActivate);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL,"CAppObjectView::OnActivateView(%s,%p,%p)\n",
		bActivate ? "TRUE " : "FALSE",
		pActivateView, pDeactiveView);
#endif
	
	if (bActivate)
	{
/*
		main->b3UpdateCameraBox(m_Scene,m_Camera);
		main->b3UpdateLightBox(m_Scene,m_Light);
		m_Scene->b3SetCamera(m_Camera,true);
*/
		main->b3UpdateModellerInfo(GetDocument());
		app->b3SetData();
	}
	else
	{
//		main->b3Clear();
//		main->b3UpdateModellerInfo();
		app->b3GetData();
	}
}

/*************************************************************************
**                                                                      **
**                        Handle picking                                **
**                                                                      **
*************************************************************************/

void CAppObjectView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_PickList.b3Down(point.x,point.y))
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

void CAppObjectView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_PickList.b3IsActive())
	{
		// Do standard action
		CAppRenderView::OnMouseMove(nFlags, point);
	}
	else
	{
		// Do MFC mouse handling when picking
		if (m_PickList.b3Move(point.x,point.y))
		{
			CAppObjectDoc *pDoc = GetDocument();

			m_PickList.b3Modified();
			pDoc->UpdateAllViews(NULL,B3_UPDATE_VIEW|B3_UPDATE_GEOMETRY);
			pDoc->SetModifiedFlag();
		}
		CScrollView::OnMouseMove(nFlags, point);
	}
}

void CAppObjectView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CAppObjectDoc *pDoc = GetDocument();

	b3UndoOperation *op = m_PickList.b3GetOperation(pDoc->b3GetSelectedShape());

	if (!m_PickList.b3Up(point.x,point.y))
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
			pDoc->b3AddOp(op);
		}
		pDoc->b3ComputeBounds();
	}
}

void CAppObjectView::OnRaytrace() 
{
	// TODO: Add your command handler code here
	CDlgObjectPreview preview(GetDocument()->b3GetParentShading(), m_BBox,m_Camera);

	preview.DoModal();
}

void CAppObjectView::OnUpdateRaytrace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}
