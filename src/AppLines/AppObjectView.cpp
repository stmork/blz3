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

#include "AppLines.h"
#include "AppObjectView.h"
#include "MainFrm.h"
#include "b3Action.h"
#include <sys/timeb.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.17  2003/04/05 13:57:21  sm
**	- Fixed ticket no. 6. Problem fixed when enlarging the draw area.
**
**	Revision 1.16  2003/02/26 16:36:16  sm
**	- Sorted drawing colors and added configuration support
**	  to dialog.
**	
**	Revision 1.15  2003/02/25 15:56:20  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**	
**	Revision 1.14  2003/02/24 17:32:38  sm
**	- Added further picking support.
**	- Fixed geometry update delay.
**	
**	Revision 1.13  2003/02/23 21:15:41  sm
**	- First shape picking
**	
**	Revision 1.12  2003/02/08 14:04:18  sm
**	- Started support for document wise bar state
**	
**	Revision 1.11  2003/01/11 12:30:29  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.10  2002/08/01 15:02:56  sm
**	- Found texture missing bug when printing. There weren't any
**	  selected textures inside an other OpenGL rendering context.
**	  Now fixed!
**	
**	Revision 1.9  2002/02/03 21:42:30  sm
**	- Added measurement printing. The measure itself is missing yet.
**	  The support is done in b3RenderView and CAppRenderView.
**	- Added support for units in b3ModellerInfo
**	- Cleaned up some accelerators. Now arrow keys are working
**	  again. The del key is working correctly inside edit controls again.
**	
**	Revision 1.8  2002/01/25 16:34:46  sm
**	- Added printer support (not running yet)
**	
**	Revision 1.7  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**	
**	Revision 1.6  2002/01/18 16:49:35  sm
**	- Further development of the object edit from scene branch. This needs
**	  much more logics for handling scenes and open object edits properly.
**	
**	Revision 1.5  2002/01/16 16:17:12  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.4  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
**	Revision 1.3  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.2  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.1  2002/01/12 18:14:39  sm
**	- Created object document template
**	- Some menu fixes done
**	
**
*/

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
		doInvalidate |= m_PickList.b3SetShape(GetDocument()->b3GetSelectedShape());
	}

	if (doInvalidate)
	{
		Invalidate();
	}

	CAppRenderView::OnUpdate(pSender,lHint,pHint);
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
	CDC   *dc = CDC::FromHandle(hDC);
	CPen  *old;

	// Setup view first
	m_RenderView.b3SetupView(xSize,ySize,xOffset,yOffset);

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
	b3UndoOperation *op = m_PickList.b3GetOperation();

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
			GetDocument()->b3AddOp(op);
		}
		GetDocument()->b3ComputeBounds();
	}
}
