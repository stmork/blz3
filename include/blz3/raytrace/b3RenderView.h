/*
**
**	$Filename:	b3RenderView.h $ 
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Render view mode handling
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_RENDERVIEW_H
#define B3_RAYTRACE_RENDERVIEW_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3Render.h"
#include "blz3/raytrace/b3Special.h"

/**
 * This enumeration lists the possible view modes.
 */
enum b3_view_mode
{
	B3_VIEW_3D = 0,   //!< Perspective projection.
	B3_VIEW_TOP,      //!< Parallel projection on top.
	B3_VIEW_FRONT,    //!< Parallel projection front view.
	B3_VIEW_RIGHT,    //!< Parallel projection right view.
	B3_VIEW_LEFT,     //!< Parallel projection left view.
	B3_VIEW_BACK,     //!< Parallel projection back view.

	B3_VIEW_MAX       //!< Maximum number of valid view modes. 
};

/**
 * This class represents one view. Several views can be stacked depending on
 * the view mode.
 */
class b3RenderViewItem : public b3Link<b3RenderViewItem>
{
public:
	b3_vector m_Size;        //!< View dimension.
	b3_vector m_Mid;         //!< View center.
	b3_f64    m_xRelation;   //!< Scaling factor from horizontal camera width to real width.
	b3_f64    m_yRelation;   //!< Scaling factor from vertical camera height to real height.

public:
	/**
	 * This constructor initializes this view item.
	 */
	b3RenderViewItem();

	/**
	 * This constructor initializes this view item with the given view bounds.
	 *
	 * @param lower The lower corner.
	 * @param upper The upper corner.
	 */
	b3RenderViewItem(b3_vector *lower,b3_vector *upper);

	/**
	 * This method sets a new boundary to this view item.
	 *
	 * @param lower The lower corner.
	 * @param upper The upper corner.
	 */
	void b3Set(b3_vector *lower,b3_vector *upper);
};

/**
 * This enumeration lists the action modes.
 */
enum b3_action_mode
{
	B3_ACTION_MOVE_RIGHT, //!< Move right.
	B3_ACTION_MOVE_LEFT,  //!< Move left.
	B3_ACTION_MOVE_UP,    //!< Move up.
	B3_ACTION_MOVE_DOWN,  //!< Move down.
	B3_ACTION_ROT_LEFT,   //!< Rotate left.
	B3_ACTION_ROT_RIGHT   //!< Rotate right.
};

/**
 * This class handles view rendering.
 */
class b3RenderView
{
	// Camera description
	b3_view_mode              m_ViewMode;
	b3_vector                 m_EyePoint;
	b3_vector                 m_ViewPoint;
	b3_vector                 m_Width;
	b3_vector                 m_Height;

	// Dimension of whole scene
	b3_vector                 m_Lower;
	b3_vector                 m_Upper;

	// View volume dimensions (from mid point to border)
	b3_render_view_info       m_ViewInfo;

	b3Base<b3RenderViewItem>  m_ViewStack[B3_VIEW_MAX];
	b3Base<b3RenderViewItem>  m_Depot;
	b3RenderViewItem         *m_Actual;

public:
	b3_bool                   m_AntiAliased; //!< Use anti aliasing.
	b3_bool                   m_AspectRatio; //!< Use correct aspect ratio.

	// Viewport size
	b3_res                    m_xRes;        //!< The view port width.
	b3_res                    m_yRes;        //!< The view port height.

public:
	                  b3RenderView();
	                 ~b3RenderView();

	/**
	 * This method sets the actual view mode. It switches the correct view stack.
	 *
	 * @param mode The new view mode to set.
	 */
	void              b3SetViewMode(b3_view_mode mode);

	/**
	 * This method checks wether the given view mode is actually used.
	 *
	 * @param mode The view mode to test.
	 * @return True if the given view mode is used.
	 */
	b3_bool           b3IsViewMode(b3_view_mode mode);

	/**
	 * This method switches the perspective view to the given camera.
	 *
	 * @param camera The camera to set.
	 */
	void              b3SetCamera(b3CameraPart *camera);
	void              b3SetCamera(b3Scene *scene);
	b3_bool           b3SetBounds(b3Scene *scene);
	void              b3SetBounds(b3_vector *lower,b3_vector *upper);
	b3_bool           b3GetDimension(b3_f64 &xSize,b3_f64 &ySize);
	void              b3PopView();
	b3_bool           b3ViewStackNotEmpty();
	void              b3Original();
	void              b3Scale(b3_f64 scale);
	void              b3Move(b3_f64 xDir,b3_f64 yDir);
	void              b3Project(const b3_vector *point,b3_f64 &xRel,b3_f64 &yRel,b3_f64 &zRel);
	void              b3GetProjectionBase(b3_vector *eye);
	void              b3GetViewDirection(b3_vector *direction);
	b3_f64            b3GetPositionAngle(b3_vector *center,b3_vector *pos);
	void              b3Select(b3_f64 xStart,b3_f64 yStart,b3_f64 xEnd,b3_f64 yEnd);
	void              b3SetupView(b3_res xSize,b3_res ySize,b3_f64 xOffset = 0.0,b3_f64 yOffset = 0.0);
	void              b3DrawRaster(b3_f64 grid,b3Color &color);
	void              b3SetTranslationStepper(b3_vector *steps,b3_vector *mover,b3_action_mode mode);
	b3_f64            b3SetRotationStepper(b3_vector *steps,b3_vector *axis_dir,b3_action_mode mode);

	inline void       b3Project(const b3_vector *point,b3_coord &x,b3_coord &y,b3_f64 &z)
	{
		b3_f64 xRel,yRel;

		b3Project(point,xRel,yRel,z);
		x = (b3_coord)floor(xRel * m_xRes + 0.5);
		y = (b3_coord)floor(yRel * m_yRes + 0.5);
	}

	inline void       b3Unproject(const b3_coord x, const b3_coord y, b3_vector *point)
	{
		b3Unproject(x, y, 0.5, point);
	}

	inline void       b3Unproject(const b3_f64 xRel,const b3_f64 yRel,b3_vector *point)
	{
		b3Unproject(xRel, yRel, 0.5, point);
	}

	inline void       b3Unproject(const b3_coord x, const b3_coord y, const b3_f64 z,b3_vector *point)
	{
		b3UnprojectInternal(
			(b3_f64)x,
			(b3_f64)(m_yRes - y),
			z,point);
	}

	/**
	 * This method unprojects a point on the projection plane into world coordinates.
	 * The xRelParam and yRelParam are relative coordinates in the range of [0..1[ where
	 * the coordinate 0,0 is at the upper left corner of the view port screen.
	 *
	 * @param xRelParam The relative horizontal view port coordinate.
	 * @param yRelParam The relative vertical view port coordinate.
	 * @param zRelParam The view depth.
	 * @param point The resulting point in world coordinates.
	 */
	inline void       b3Unproject(const b3_f64 xRelParam, const b3_f64 yRelParam, const b3_f64 zRelParam,b3_vector *point)
	{
		b3UnprojectInternal(
			xRelParam * m_xRes,
			m_yRes - yRelParam * m_yRes,
			zRelParam,point);
	}

private:
	b3RenderViewItem *b3NewRenderViewItem(b3RenderViewItem *lastItem = null);
	b3_f64            b3ComputeFarClippingPlane();
	void              b3UnprojectInternal(const b3_f64 xRel,const b3_f64 yRel,const b3_f64 z, b3_vector *point);
};

#endif
