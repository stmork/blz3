/*
**
**	$Filename:	b3ScrollView.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - scroll view control through Blizzard III
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_SCROLLVIEW_H
#define B3_SYSTEM_SCROLLVIEW_H

#include "blz3/image/b3Tx.h"
#include "blz3/system/b3ViewParam.h"
#include "blz3/system/b3Document.h"
#include "blz3/system/b3View.h"
#include "blz3/system/b3Print.h"

#define B3_UPDATE_TX                1
#define B3_UPDATE_TRACKER1          2
#define B3_UPDATE_TRACKER2          4
#define B3_UPDATE_CHECK_SCALING     8
#define B3_UPDATE_PREVENT_RESCALE  16
#define B3_UPDATE_TRACKER_LIST     32
#define B3_UPDATE_SIZE             64
#define B3_UPDATE_TOOLTIP         128

class CB3ScrollView : public CScrollView
{
	b3Mutex        m_MutexScaling;
	b3_bool        m_PaletteDisplay;
	b3_bool        m_DoMagnify;

	HCURSOR        m_CurMagnify;

protected: // create from serialization only
	DECLARE_DYNCREATE(CB3ScrollView)

	// Attributes
	// Cursor
	b3_bool          m_InitialUpdate;
	b3_f64           m_xScale;
	b3_f64           m_yScale;

	// Panning
	CPoint           m_OldAct;
	CPoint           m_Position;
	CPoint           m_Start;
	CB3View     *    m_View;
	CB3Print    *    m_Print;
	b3_bool          m_Panning;

	// Draw mode
	b3Tx      *      m_TxScale;
	b3_bool          m_ViewScan;
	b3_bool          m_Filtered;
	b3_display_mode  m_Mode;

public:
	HCURSOR          m_Cursor;
	b3_bool          m_UseCursor;

	// Operations
public:
	CDocument  *  GetDocument();
	void          OnRefresh(CView *, LPARAM, CObject *);
	void          b3GetViewParam(CB3ViewParam * param, CWnd * coord_reference = null);
	void          b3SetViewParam(CB3ViewParam * param);

protected:
	b3_bool       b3SetSize(b3Tx *, CSize &);
	void          b3UpdateTx(LPARAM lHint);
	void          b3UpdateSize(LPARAM lHint);
	void          b3Update(LPARAM lHint);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ScrollView)
public:
	virtual void OnDraw(CDC * pDC); // overridden to draw this view
	virtual void OnPrepareDC(CDC * pDC, CPrintInfo * pInfo = NULL);
protected:
	virtual int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView * pSender, LPARAM lHint, CObject * pHint);
	virtual void OnBeginPrinting(CDC * pDC, CPrintInfo * pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo * pInfo);
	virtual void OnEndPrinting(CDC * pDC, CPrintInfo * pInfo);
	virtual void OnEndPrintPreview(CDC * pDC, CPrintInfo * pInfo, POINT point, CPreviewView * pView);
	virtual void OnPrint(CDC * pDC, CPrintInfo * pInfo);
	//}}AFX_VIRTUAL

	// Implementation
public:
	CB3ScrollView();
	virtual ~CB3ScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dc) const;
#endif
	virtual void          b3Activate(b3_bool was_deactive = true);
	virtual b3Document  * b3GetDocument();

protected:
	virtual void          b3ViewParamChanged();
	virtual b3_bool       b3IsMagnifying();
	virtual void          b3SetMagnifying(b3_bool DoMagnify = true);
	void          b3ShowView();
	void          b3SetViewMode(b3_display_mode mode);
	void          b3SetFilterMode(b3_bool filter_enable);
	void          b3SetScale(b3_f64 xScale, b3_f64 yScale);
	void          b3MagnifyMore();
	void          b3MagnifyLess();
	void          b3ViewRubber();

	// Generated message map functions
protected:
	//{{AFX_MSG(CB3ScrollView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	b3_bool b3CorrectScaleCenter(CPoint & position, b3_f64 factor);
};

#ifndef _DEBUG  // debug version in scanView.cpp
inline CDocument * CB3ScrollView::GetDocument()
{
	return m_pDocument;
}
#endif

#endif
