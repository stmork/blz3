/*
**
**	$Filename:	b3Special.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for specials
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_SPECIAL_H
#define B3_RAYTRACE_SPECIAL_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Clouds.h"
#include "blz3/base/b3Filter.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Procedure.h"

/*************************************************************************
**                                                                      **
**                        specials                                      **
**                                                                      **
*************************************************************************/

#define CLASS_SPECIAL           0x7f000000 // for diverse elements
#define TYPE_SUPERSAMPLE4       0x00000001
#define TYPE_NEBULAR            0x00000002
#define TYPE_CAMERA             0x00000003
#define TYPE_MODELLERINFO       0x00000004
#define TYPE_ANIMATION          0x00000005
#define TYPE_DISTRIBUTE         0x00000006
#define TYPE_LENSFLARE          0x00000007
#define TYPE_CAUSTIC            0x00000008
#define TYPE_CLOUDS             0x00000009
#define SUPERSAMPLE4            (CLASS_SPECIAL|TYPE_SUPERSAMPLE4)
#define NEBULAR                 (CLASS_SPECIAL|TYPE_NEBULAR)
#define CAMERA                  (CLASS_SPECIAL|TYPE_CAMERA)
#define LINES_INFO              (CLASS_SPECIAL|TYPE_MODELLERINFO)
#define ANIMATION               (CLASS_SPECIAL|TYPE_ANIMATION)
#define DISTRIBUTE              (CLASS_SPECIAL|TYPE_DISTRIBUTE)
#define LENSFLARE               (CLASS_SPECIAL|TYPE_LENSFLARE)
#define CAUSTIC                 (CLASS_SPECIAL|TYPE_CAUSTIC)
#define CLOUDS                  (CLASS_SPECIAL|TYPE_CLOUDS)

/**
 * This class represents a special effect which is scene global.
 *
 * @see b3Scene
 */
class B3_PLUGIN b3Special : public b3Item
{
protected:
	B3_ITEM_BASE(b3Special); //!< This is a base class deserialization constructor.

public:
	B3_ITEM_INIT(b3Special); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Special); //!< This constructor handles deserialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static void b3Register();
};

/**
 * This class describes super sampling. If the pixel color differences to the neighbour
 * colors exceeds the color limit the pixel is super sampled with three additional rays.
 */
class B3_PLUGIN b3SuperSample : public b3Special
{
	bool        m_Active;

public:
	b3Color     m_Limit; //!< The color distance as limit.

public:
	B3_ITEM_INIT(b3SuperSample); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3SuperSample); //!< This constructor handles deserialization.

	void    b3Write() override;

	/**
	 * This method returns the activations state of super sampling.
	 *
	 * @return The activation state.
	 */
	bool    b3IsActive() const;

	/**
	 * This method sets the new activation state of super sampling.
	 *
	 * @param activate The new activation state.
	 */
	void    b3Activate(bool activate = true);
};

/**
 * This class handles one camera.
 */
class B3_PLUGIN b3CameraPart : public b3Special
{
public:
	b3_vector        m_Width;     //!< The horizontal direction of the projection plane.
	b3_vector        m_Height;    //!< The vertical direction of the projection plane.
	b3_vector        m_EyePoint;  //!< The eye point.
	b3_vector        m_ViewPoint; //!< The look point and the center of the projection plane.
	b3_s32           m_Flags;     //!< Activation flags.
	char             m_CameraName[B3_CAMERANAMELEN]; //!< The camera name.

public:
	B3_ITEM_INIT(b3CameraPart); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CameraPart); //!< This constructor handles deserialization.

	void     b3Write() override;
	bool     b3Prepare(b3_preparation_info * prep_info) override;

	/**
	 * This method orientates the camera to the given values. The internal vectors
	 * are adjusted appropriate.
	 *
	 * @param eye The new eye point.
	 * @param view The new view point.
	 * @param focalLength The new focal length. The internal view point is adjusted appropriate.
	 * @param width The new projection width. This value is a half value noted from the center to the horizontal border.
	 * @param height The new projection height.This value is a half value noted from the center to the vertical border.
	 */
	void     b3Orientate(
		const b3_vector * eye,
		b3_vector    *    view,
		const b3_f64      focal_length,
		const b3_f64      width,
		const b3_f64      height);

	/**
	 * This method orientates the camera around a center point. The eye point is computed
	 * from spherical polar values given as angles.
	 *
	 * @note The angles are given as radians.
	 * @param center The center point of the sphere.
	 * @param size The dimension to overview.
	 * @param xAngle The longitude.
	 * @param yAngle The latitide.
	 */
	void     b3Overview(
		b3_vector    *    center,
		const b3_vector * size,
		const b3_f64      xAngle,
		const b3_f64      yAngle);

	/**
	 * This method converts the camera in longitude and latitude. These values are computed
	 * from an imaginary sphere where the view point is the center and the eye point may
	 * move onto the sphere. The position of the eye point can be expressed in longitude
	 * and latitude.
	 *
	 * @param xAngle The longitude.
	 * @param yAngle The latitude.
	 */
	void     b3ComputeAngles(b3_f64 & xAngle, b3_f64 & yAngle) const;

	/**
	 * This method adjusts the focal length of the camera.
	 *
	 * @param focalLength The new focal length.
	 */
	void     b3ComputeFocalLength(const b3_f64 focalLength);

	/**
	 * This method returns the distance between the eye point and the view point. This
	 * value is also knowns as focal length.
	 *
	 * @return The focal length of this camera.
	 */
	b3_f64   b3GetFocalLength() const;

	/**
	 * This method returns the camera twirl.
	 *
	 * @return The camera twirl.
	 */
	b3_f64   b3GetTwirl() const;

	/**
	 * This method adjusts the twirl of the camera.
	 *
	 * @param twirl The new twirl.
	 */
	void     b3SetTwirl(const b3_f64 twirl);

	/**
	 * This method scales the focal length.
	 *
	 * @param factor The scaling factor for the focal length.
	 */
	void     b3ScaleFocalLength(const b3_f64 factor);

	/**
	 * This method transforms the camera with the given transformation matrix.
	 *
	 * @param transformation The transformation matrix.
	 */
	void     b3Transform(b3_matrix * transformation);

	/**
	 * This method returns the camera name.
	 *
	 * @return The camera name.
	 */
	const char  *  b3GetName() const override;

	/**
	 * This method sets a new camera name.
	 *
	 * @param name The new camera name.
	 */
	void     b3SetName(const char * name);

	/**
	 * This method returns the activation state of this camera.
	 *
	 * @return True if camera is active.
	 */
	bool     b3IsActive() const;

	/**
	 * This method sets the activation state of this camera.
	 *
	 * @param activate The new activation state.
	 */
	void     b3Activate(const bool activate = true);
};

#define CAMERA_TITLE  1
#define CAMERA_ACTIVE 2

/**
 * This class handles scene global nebular.
 */
class B3_PLUGIN b3Nebular : public b3Special
{
	b3_f64           m_NebularDenom;

public:
	b3Color          m_NebularColor; //!< The nebular color.
	b3_f32           m_NebularVal;   //!< The relative distance at which the transmittance is half from original.

public:
	B3_ITEM_INIT(b3Nebular); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Nebular); //!< This constructor handles deserialization.

	void    b3Write() override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;

	/**
	 * This method returns the activation state.
	 *
	 * @return True if nebular is active.
	 */
	bool    b3IsActive() const;

	/**
	 * This method activates the nebular.
	 *
	 * @param activate The activation state of the nebular.
	 */
	void    b3Activate(bool activate = true);

	/**
	 * This method returns the nebular filter color.
	 *
	 * @param result The nebular filter color.
	 */
	void    b3GetNebularColor(b3Color & result) const;

	/**
	 * This method computes the nebular color depending on the input color, the
	 * nebular filter color the relative transmittance and the real distance.
	 * If the distance is near infinity it results in the nebular filter color.
	 *
	 * @param input The input color to filter.
	 * @param result The resulting nebulated color.
	 * @param distance The real distance.
	 */
	void    b3ComputeNebular(
		const b3Color & input, b3Color & result,
		const b3_f64    distance) const;
};

/**
 * This enumeration provides some units.
 */
enum b3_unit
{
	B3_UNIT_MM = 0,  //!< Millimeter.
	B3_UNIT_CM,      //!< Centimeter.
	B3_UNIT_IN,      //!< Inch.
	B3_UNIT_DM,      //!< Decimeter.
	B3_UNIT_FT,      //!< Foot.
	B3_UNIT_M,       //!< Meter.
	B3_UNIT_MAX      //!< Amount of usable units.
};

/**
 * This enumeration lists some measuring units.
 */
enum b3_measure
{
	B3_MEASURE_1 = 0,  //!< 1:1
	B3_MEASURE_10,     //!< 1:10
	B3_MEASURE_20,     //!< 1:20
	B3_MEASURE_50,     //!< 1:50
	B3_MEASURE_100,    //!< 1:100
	B3_MEASURE_200,    //!< 1:200
	B3_MEASURE_500,    //!< 1:500
	B3_MEASURE_1000,   //!< 1:1000
	B3_MEASURE_CUSTOM, //!< Custom measuring unit.
	B3_MEASURE_MAX     //!< Amount of usable measuring units.
};

/**
 * This enumeration lists type of snap to angle
 */
enum b3_angle_grid
{
	B3_ANGLE_GRID_OBJECT_BIT = 0,  //!< Activation for object snap to grid
	B3_ANGLE_GRID_CAMERA_BIT       //!< Activation for camera snap to grid
};

#define B3_ANGLE_GRID_OBJECT (1 << B3_ANGLE_GRID_OBJECT_BIT)
#define B3_ANGLE_GRID_CAMERA (1 << B3_ANGLE_GRID_CAMERA_BIT)

/**
 * This class represents some helping values to model with Lines III.
 */
class B3_PLUGIN b3ModellerInfo : public b3Special
{
	static const b3_f64   m_UnitScaleTable[B3_UNIT_MAX];
	static const char  *  m_UnitDescrTable[B3_UNIT_MAX];
	static const b3_u32   m_MeasureTable[B3_MEASURE_MAX - 1];

	b3_u32           m_Flags;
	b3_u32           m_AngleGrid;        //!< Scale to angular grid active.

public:
	b3_vector        m_Center;           //!< The position of the frustrum.
	b3_vector        m_StepMove;         //!< Moving stepper.
	b3_vector        m_StepRotate;       //!< Rotating stepper.
	b3_f32           m_GridMove;         //!< Scale to grid for moving.
	b3_f32           m_GridRot;          //!< Scale to angular grid for rotating.
	bool             m_ResizeFlag;
	bool             m_BBoxTitles;       //!< Draw object names into scene.
	bool             m_GridActive;       //!< Scale to grid active.
	bool             m_AngleGridCamera;  //!< Snap to camera angle.
	bool             m_AngleGridObjects; //!< Snap to object angle.
	bool             m_CameraActive;
	bool             m_UseSceneLights;   //!< Simple lighting (false) or scene lighting (true).
	b3_unit          m_Unit;             //!< Used unit.
	b3_measure       m_Measure;          //!< Used measuring unit.
	b3_u32           m_CustomMeasure;    //!< Custum measuring unit in case of m_Measure holds B3_MEASURE_CUSTOM.

public:
	B3_ITEM_INIT(b3ModellerInfo); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3ModellerInfo); //!< This constructor handles deserialization.

	void        b3Write() override;

	/**
	 * This method snaps the given translation point into the next grid position.
	 *
	 * @param translation The point to adjust to the grid.
	 */
	void        b3SnapToGrid(b3_vector * translation) const;

	/**
	 * This method snaps the given angle into the next angular grid position.
	 *
	 * @param angle The angle to adjust.
	 */
	void        b3SnapToCameraAngle(b3_f64 & angle) const;

	/**
	 * This method snaps the given angle into the next angular grid position.
	 *
	 * @param angle The angle to adjust.
	 */
	void        b3SnapToObjectAngle(b3_f64 & angle) const;

	/**
	 * This method returns the configured unit in milli meters.
	 *
	 * @return The length of the configured unit in milli meters.
	 */
	b3_f64      b3ScaleUnitToMM() const;

	/**
	 * This method returns the configured unit as clear text.
	 *
	 * @return The unit as clear text.
	 */
	const char * b3GetUnitDescr() const;

	/**
	 * This method scales an input value into the desired unit depending on
	 * the configured unit.
	 *
	 * @param value The value to scale.
	 * @param unit The desired unit to scale into.
	 * @return The scaled value in the desired unit.
	 */
	b3_f64       b3Scale(const b3_f64 value, const b3_unit unit = B3_UNIT_MM) const
	{
		return value * m_UnitScaleTable[m_Unit] / m_UnitScaleTable[unit];
	}

	/**
	 * This method sets a new custom measuring unit.
	 *
	 * @param measure The new custom measuring unit.
	 * @see b3_measure.
	 */
	void        b3SetMeasure(const b3_u32 measure);

	/**
	 * This method sets a new rpedefined measuring unit.
	 *
	 * @param measure The new measuring unit.
	 * @see b3_measure.
	 */
	void        b3SetMeasure(const b3_measure measure);

	/**
	 * This method returns the configured measing unit. If the
	 * unit is custom of the given flag is set the m_Measure flag
	 * is returned. Otherwise the predefined value from an internal
	 * table is returned.
	 *
	 * @param forceCustomValue If true the m_Measure value is returned.
	 * @return The measuring unit as value.
	 * @see b3_measure
	 */
	b3_u32      b3GetMeasure(const bool forceCustomValue = true) const;

private:
	void        b3Snap(b3_f64 & angle, const bool activation) const;
};

#define B3_UNIT_MASK           0x0000f
#define B3_MEASURE_MASK        0x000f0
#define B3_CUSTOM_MEASURE_MASK 0x3ff00
#define B3_USE_SCENE_LIGHTS    0x40000

#define B3_UNIT_SHIFT                0
#define B3_MEASURE_SHIFT             4
#define B3_CUSTOM_MEASURE_SHIFT      8

class b3AnimElement;
class b3Scene;

/**
 * This class provides a complete animation flow control.
 */
class B3_PLUGIN b3Animation : public b3Special
{
	// OK, the following values are only for "Lines"
	b3_count        m_Frames;          //!< Computed number of frames.
	b3_count        m_Tracks;          //!< Number of visible tracks.
	b3_index        m_TrackIndex;      //!< Start track in window .
	b3_index        m_FrameIndex;      //!< Start frame in window.
	b3_count        m_WTracks;         //!< Actual number of tracks.
	b3_count        m_WFrames;         //!< Whole of frames.
	b3AnimElement * m_Element;         //!< Actual animation element.
	b3_vector       m_AnimCenter;      //!< Actual animation center.

public:
	b3_f64          m_Start;           //!< Start time (one unit per frame)
	b3_f64          m_End;             //!< End time (one unit per frame)
	b3_f64          m_Time;            //!< Actual time point
	b3_f64          m_Neutral;         //!< Neutral point for resetting animation
	b3_count        m_FramesPerSecond; //!< Frames per second.
	b3_u32          m_Flags;           //!< Some flags.

public:
	B3_ITEM_INIT(b3Animation); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Animation); //!< This constructor handles deserialization.

	/**
	 * Method for registering the shapes into the item registry.
	 */
	static void            b3Register();
	void            b3Write() override;

public:
	/**
	 * This method sets the given animation element for next computation.
	 *
	 * @param Element The animation element to compute.
	 */
	void            b3SetAnimElement(b3AnimElement * Element);

	/**
	 * This method returns the activation state of the animation.
	 *
	 * @return The activation state of the animation.
	 */
	bool            b3IsActive() const;

	/**
	 * This method activates the animation depending on the given flag.
	 *
	 * @param activate The activation flag.
	 */
	void            b3Activate(bool activate = true);

	/**
	 * This method computes all animation elements so that the animated objects
	 * transforms for the given time point.
	 *
	 * @param Global The scene to animate.
	 * @param t The time point to compute.
	 */
	void            b3SetAnimation(b3Scene * Global, b3_f64 t);

	/**
	 * This method resets the given scene to the given neutral time point.
	 *
	 * @param Global The scene to reset.
	 */
	void            b3ResetAnimation(b3Scene * Global);

	/**
	 * This method resets the animation and sets the activation state.
	 *
	 * @param scene The scene to reset.
	 * @param activate The activation flag.
	 * @return The old activation state.
	 */
	bool            b3ActivateAnimation(b3Scene * scene, bool activate = true);

	/**
	 * This method computes the animation center position for the given time point.
	 *
	 * @param Element The animation element.
	 * @param center The center point.
	 * @param t The time point to use.
	 */
	void            b3RecomputeCenter(
		const b3AnimElement * Element,
		b3_vector      *      center,
		const b3_f64          t);

	/**
	 * This method converts a frame index into a time point.
	 *
	 * @param index The frame index to convert.
	 * @return The converted time point.
	 */
	inline b3_f64   b3AnimTimeCode(const b3_index index) const
	{
		return m_Start + (b3_f64)index / m_FramesPerSecond;
	}

	/**
	 * This method converts a time point into a frame index.
	 *
	 * @param t The time point.
	 * @return The converted frame index.
	 */
	inline b3_index b3AnimFrameIndex(const b3_f64 t)
	{
		return (b3_index)((t - m_Start) * m_FramesPerSecond);
	}

	/**
	 * This method clips a time point at the animation bounds.
	 *
	 * @param val The time point to clip.
	 * @return The clipped time point.
	 */
	inline b3_f64   b3ClipTimePoint(const b3_f64 val)
	{
		return std::clamp(val, m_Start, m_End);
	}

	/**
	 * This method returns the list base of the animation elements.
	 *
	 * @return The list base of animation elements.
	 */
	inline b3Base<b3Item> * b3GetAnimElementHead()
	{
		return &m_Heads[0];
	}

private:
	void            b3RecomputeNeutralInverse(b3AnimElement * Element);
	void            b3GetNeutralPosition(b3AnimElement * Element, b3_vector * neutral);
	void            b3ApplyTransformation(b3Scene * Global, b3AnimElement * Anim, b3_matrix * transform, b3_f64 t);
	static b3AnimElement * b3FindSameTrack(b3AnimElement * Element);
};

#define ANIMB_ON     1

#define ANIMF_ON     (1<<ANIMB_ON)

/**
 * This enumeration lists the possible randomization types of samples.
 */
enum b3_sample
{
	SAMPLE_REGULAR     = 0x0000,
	SAMPLE_RANDOM      = 0x0100,
	SAMPLE_JITTER      = 0x0200,
	SAMPLE_SEMI_JITTER = 0x0300,
	SAMPLE_SEPARATED   = 0x0400
};

/**
 * This class describes distributed raytraincing. The distributed raytracing uses some
 * filtering techniques to enhance image quality and provide effects such motion blur
 * or depth of field.
 *
 * @see b3Filter.
 * @see b3_filter.
 */
class B3_PLUGIN b3Distribute : public b3Special
{
public:
	b3Array<b3_f64>    m_MotionBlur;       //!< The motion blur time points.
	b3Array<b3_index>  m_TimeIndex;        //!< A randomized array of sample index references.
	b3_count           m_SamplesPerPixel;  //!< Samples per pixel as power of two.
	b3_count           m_SamplesPerFrame;  //!< Samples per frame.
	b3_f32             m_DepthOfField;     //!< A place holder for depth of field but unused, yet.
	b3_filter          m_PixelAperture;    //!< The pixel aperture type.
	b3_filter          m_FrameAperture;    //!< The frame aperture type.
	b3Filter     *     m_FilterPixel;      //!< The pixel aperture instance.
	b3Filter     *     m_FilterFrame;      //!< The frame aperture instance.
	b3_f32      *      m_Samples;          //!< This pointer points to an array of sampling positions inside one pixel.
	b3_count           m_SPP;              //!< The real amount of samples per pixel.
	b3_u32             m_Type;             //!< Flags of activated effects.

public:
	B3_ITEM_INIT(b3Distribute); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Distribute); //!< This constructor handles deserialization.

	/**
	 * This destructor deinitializes the distributed raytracing.
	 */
	virtual ~b3Distribute();
	void     b3Write() override;

	/**
	 * This method returns the activation state of distributed raytracing in general.
	 *
	 * @return True if distributed raytracing is active.
	 */
	bool     b3IsActive();

	/**
	 * This method returns the activation state of motion blur.
	 *
	 * @return True if motion blur is active.
	 */
	bool     b3IsMotionBlur();

	/**
	 * This method initializes the distributed raytracing with the given
	 * animation for motion blur if any.
	 *
	 * @param xSize The image width to raytrace.
	 * @param animation The animation for motion blur if any.
	 * @throws b3WorldException
	 */
	void     b3PrepareAnimation(b3_res xSize, b3Animation * animation = nullptr);
};

#define SAMPLE_MOTION_BLUR_B     0
#define SAMPLE_SUPERSAMPLE_B     1
#define SAMPLE_DEPTH_OF_FIELD_B  2

#define SAMPLE_MOTION_BLUR      (1 << SAMPLE_MOTION_BLUR_B)
#define SAMPLE_SUPERSAMPLE      (1 << SAMPLE_SUPERSAMPLE_B)
#define SAMPLE_DEPTH_OF_FIELD   (1 << SAMPLE_DEPTH_OF_FIELD_B)

#define SAMPLE_GET_FLAGS(d)     ((d)->m_Type & 0x00ff)
#define SAMPLE_GET_TYPE(d)      (b3_sample)((d)->m_Type & 0xff00)
#define SAMPLE_SET_FLAGS(d,v)   ((d)->m_Type = ((d)->m_Type & 0xffffff00) | (v))
#define SAMPLE_SET_TYPE(d,v)    ((d)->m_Type = ((d)->m_Type & 0xffff00ff) | (v))

/**
 * This class provides information about lens flares.
 */
class B3_PLUGIN b3LensFlare : public b3Special
{
public:
	b3_s32         m_Flags; //!< Some activation flags
	b3Color        m_Color; //!< The lens flare color.
	b3_f32         m_Expon; //!< The exponent.

public:
	B3_ITEM_INIT(b3LensFlare); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3LensFlare); //!< This constructor handles deserialization.

	void    b3Write() override;

	/**
	 * This method returns the activation state of this lens flare.
	 *
	 * @return True if lens flares are active.
	 */
	bool    b3IsActive();

	/**
	 * This method sets the activation state of the lens flares.
	 *
	 * @param activate The new activation state.
	 */
	void    b3Activate(bool activate = true);
};

#define LENSFLARE_ACTIVE 1

/**
 * This class represents information of caustic computation. This is
 * in experimental state in Blizzard II and not implemented in Blizzard III, yet.
 *
 * @todo Simply implement this.
 */
class B3_PLUGIN b3Caustic : public b3Special
{
public:
	b3_u32   m_Flags;      //!< Some flags.
	b3_count m_NumPhotons; //!< Number of photons to shoot.
	b3_count m_TraceDepth; //!< Ray trace depth of forward raytracing.

public:
	B3_ITEM_INIT(b3Caustic); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3Caustic); //!< This constructor handles deserialization.

	void b3Write() override;
};

#define CAUSTIC_ENABLE_B 0
#define CAUSTIC_ENABLE   (1 << CAUSTIC_ENABLE_B)

/**
 * This class provides clouds for backgrounding where rays hit infinity.
 *
 * @see b3Clouds.
 */
class b3CloudBackground : public b3Special, public b3Clouds
{
public:
	B3_ITEM_INIT(b3CloudBackground); //!< This constructor handles default initialization.
	B3_ITEM_LOAD(b3CloudBackground); //!< This constructor handles deserialization.

	void    b3Write() override;
	bool    b3Prepare(b3_preparation_info * prep_info) override;
};

#endif
