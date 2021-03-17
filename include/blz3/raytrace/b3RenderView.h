/*
**
**	$Filename:	b3RenderView.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

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
	b3RenderViewItem(b3_vector * lower, b3_vector * upper);

	/**
	 * This method sets a new boundary to this view item.
	 *
	 * @param lower The lower corner.
	 * @param upper The upper corner.
	 */
	void b3Set(b3_vector * lower, b3_vector * upper);
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
 *
 * @see b3RenderViewItem
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
	b3RenderViewItem     *    m_Actual;

public:
	b3_bool                   m_AntiAliased; //!< Use anti aliasing.
	b3_bool                   m_AspectRatio; //!< Use correct aspect ratio.

	// Viewport size
	b3_res                    m_xRes;        //!< The view port width.
	b3_res                    m_yRes;        //!< The view port height.

public:
	/**
	 * This constructor initializes this render view instance.
	 */
	b3RenderView();

	/**
	 * This destructor deinitializes this render view instance.
	 */
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
	void              b3SetCamera(b3CameraPart * camera);

	/**
	 * This method switches the perspective to the camera set as
	 * actual camera in the given scene.
	 *
	 * @param scene The scene with the actual camera.
	 */
	void              b3SetCamera(b3Scene * scene);

	/**
	 * This method resizes the view section so that the whole scene
	 * fits in the view.
	 *
	 * @param scene The scene to use.
	 * @return True on success.
	 */
	b3_bool           b3SetBounds(b3Scene * scene);

	/**
	 * This method sets the view section to the given bounds.
	 *
	 * @param lower The lower corner.
	 * @param upper The upper corner.
	 */
	void              b3SetBounds(const b3_vector * lower, const b3_vector * upper);

	/**
	 * This method returns the dimensions of the view section in world
	 * coordinates.
	 *
	 * @param xSize The view width in world coordinates.
	 * @param ySize The view height in world coordinates.
	 * @return True on success.
	 */
	b3_bool           b3GetDimension(b3_f64 & xSize, b3_f64 & ySize);

	/**
	 * This method pops the actual view item from the actual view mode.
	 */
	void              b3PopView();

	/**
	 * This method returns true if there are any view items in the actual
	 * view stack.
	 *
	 * @return True if the actual view stack is not empty.
	 */
	b3_bool           b3ViewStackNotEmpty();

	/**
	 * This method empties the actual view stack and put a new one
	 * on top the stack where the scene fits into the view.
	 *
	 * @note This works only for parallel projection modes.
	 */
	void              b3Original();

	/**
	 * This method scales the actual parallel view by the given factor.
	 *
	 * @param scale The scale factor.
	 */
	void              b3Scale(b3_f64 scale);

	/**
	 * This method moves the current view section. The coordinates are in view port
	 * relative coordinates.
	 *
	 * @param xDir The relative horizontal movement.
	 * @param yDir The relative vertical movement.
	 */
	void              b3Move(b3_f64 xDir, b3_f64 yDir);

	/**
	 * This method returns the eye point of the selected view.
	 *
	 * @param eye The resulting eye point.
	 */
	void              b3GetProjectionBase(b3_vector * eye);

	/**
	 * This method returns the view direction.
	 *
	 * @param direction The resulting view direction.
	 */
	void              b3GetViewDirection(b3_vector * direction);

	/**
	 * This method returns a position angle between a given center and a given
	 * position. The resulting angles is expressed in radians.
	 *
	 * @param center The center point.
	 * @param position The cursor position.
	 * @return The resulting position angle.
	 */
	b3_f64            b3GetPositionAngle(b3_vector * center, b3_vector * position);

	/**
	 * This Method selects a new view section and puts the new view onto
	 * the actual view stack.
	 *
	 * @param xStart The lower x coordinate of the new bound.
	 * @param yStart The lower y coordinate of the new bound.
	 * @param xEnd The upper x coordinate of the new bound.
	 * @param yEnd The upper y coordinate of the new bound.
	 */
	void              b3Select(b3_f64 xStart, b3_f64 yStart, b3_f64 xEnd, b3_f64 yEnd);

	/**
	 * This method initializes the real OpenGL view.
	 *
	 * @param xSize The view port width.
	 * @param ySize The view port height.
	 * @param xOffset The horizontal view port offset.
	 * @param yOffset The vertical view port offset.
	 */
	void              b3SetupView(b3_res xSize, b3_res ySize, b3_f64 xOffset = 0.0, b3_f64 yOffset = 0.0);

	/**
	 * This method draws the snap grid.
	 *
	 * @param grid The snap to grid grid distance.
	 * @param color The grid color.
	 */
	void              b3DrawRaster(b3_f64 grid, b3Color & color);

	/**
	 * This method initializes a real move direction vector from an initial stepper
	 * vector depending on the actual view mode.
	 *
	 * @param steps The vector with the steps for three different components.
	 * @param mover The resulting mover.
	 * @param mode The view mode to use.
	 */
	void              b3SetTranslationStepper(b3_vector * steps, b3_vector * mover, b3_action_mode mode);

	/**
	 * This method initializes a rotation axis and a step vector which contains
	 * the angle steps around the three different axis.
	 *
	 * @param steps The angular step vector.
	 * @param axisDir The resulting direction vector of the axis.
	 * @param mode The view mode to use.
	 */
	b3_f64            b3SetRotationStepper(b3_vector * steps, b3_vector * axisDir, b3_action_mode mode);

	/**
	 * This method projects a point onto the projection plane of the actually
	 * configured view. The xRelParam and yRelParam are relative coordinates
	 * in the range of [0..1[ where
	 * the coordinate 0,0 is at the upper left corner of the view port screen.
	 *
	 * @param point The point to project.
	 * @param xRel The resulting relative x view port coordinate.
	 * @param yRel The resulting relative y view port coordinate.
	 * @param zRel The resulting view depth.
	 */
	void              b3Project(const b3_vector * point, b3_f64 & xRel, b3_f64 & yRel, b3_f64 & zRel);

	/**
	 * This method projects a point onto the projection plane of the actually
	 * configured view.
	 *
	 * @param point The point to project.
	 * @param x The resulting x view port coordinate.
	 * @param y The resulting y view port coordinate.
	 * @param z The resulting view depth.
	 */
	inline void       b3Project(const b3_vector * point, b3_coord & x, b3_coord & y, b3_f64 & z)
	{
		b3_f64 xRel, yRel;

		b3Project(point, xRel, yRel, z);
		x = (b3_coord)floor(xRel * m_xRes + 0.5);
		y = (b3_coord)floor(yRel * m_yRes + 0.5);
	}

	/**
	 * This method unprojects a point on the projection plane into world coordinates.
	 * The coordinate 0,0 is at the upper left corner of the view port screen.
	 * The view depth is assumed as 0.5.
	 *
	 * @param x The x view port coordinate.
	 * @param y The y view port coordinate.
	 * @param point The resulting point in world coordinates.
	 */
	inline void       b3Unproject(const b3_coord x, const b3_coord y, b3_vector * point)
	{
		b3Unproject(x, y, 0.5, point);
	}

	/**
	 * This method unprojects a point on the projection plane into world coordinates.
	 * The xRel and yRel are relative coordinates in the range of [0..1[ where
	 * the coordinate 0,0 is at the upper left corner of the view port screen. The view
	 * depth is assumed as 0.5.
	 *
	 * @param xRel The relative horizontal view port coordinate.
	 * @param yRel The relative vertical view port coordinate.
	 * @param point The resulting point in world coordinates.
	 */
	inline void       b3Unproject(const b3_f64 xRel, const b3_f64 yRel, b3_vector * point)
	{
		b3Unproject(xRel, yRel, 0.5, point);
	}

	/**
	 * This method unprojects a point on the projection plane into world coordinates.
	 * The coordinate 0,0 is at the upper left corner of the view port screen.
	 *
	 * @param x The x view port coordinate.
	 * @param y The y view port coordinate.
	 * @param z The view depth.
	 * @param point The resulting point in world coordinates.
	 */
	inline void       b3Unproject(const b3_coord x, const b3_coord y, const b3_f64 z, b3_vector * point)
	{
		b3UnprojectInternal(
			(b3_f64)x,
			(b3_f64)(m_yRes - y),
			z, point);
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
	inline void       b3Unproject(const b3_f64 xRelParam, const b3_f64 yRelParam, const b3_f64 zRelParam, b3_vector * point)
	{
		b3UnprojectInternal(
			xRelParam * m_xRes,
			m_yRes - yRelParam * m_yRes,
			zRelParam, point);
	}

private:
	b3RenderViewItem * b3NewRenderViewItem(b3RenderViewItem * lastItem = nullptr);
	b3_f64            b3ComputeFarClippingPlane();
	void              b3UnprojectInternal(const b3_f64 xRel, const b3_f64 yRel, const b3_f64 z, b3_vector * point);
};

#endif
