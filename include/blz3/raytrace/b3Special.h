/*
**
**	$Filename:	b3Special.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
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

#ifndef B3_RAYTRACE_SPECIAL_H
#define B3_RAYTRACE_SPECIAL_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/base/b3Array.h"
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

class B3_PLUGIN b3Special : public b3Item
{
protected:
	B3_ITEM_BASE(b3Special);

public:
	B3_ITEM_INIT(b3Special);
	B3_ITEM_LOAD(b3Special);

	static void b3Register();
};

// SUPERSAMPLE4
class B3_PLUGIN b3SuperSample : public b3Special
{
	b3_bool     m_Active;

public:
	b3Color     m_Limit;

public:
	B3_ITEM_INIT(b3SuperSample);
	B3_ITEM_LOAD(b3SuperSample);

	       void    b3Write();
	       b3_bool b3IsActive();
	       void    b3Activate(b3_bool activate=true);
};

// CAMERA
class B3_PLUGIN b3CameraPart : public b3Special
{
public:
	b3_vector        m_Width;
	b3_vector        m_Height;
	b3_vector        m_EyePoint;
	b3_vector        m_ViewPoint;
	b3_s32           m_Flags;
	char             m_CameraName[B3_CAMERANAMELEN];

public:
	B3_ITEM_INIT(b3CameraPart);
	B3_ITEM_LOAD(b3CameraPart);

	void     b3Write();
	void     b3Orientate(b3_vector *eye,b3_vector *view,b3_f64 focal_length,b3_f64 width,b3_f64 height);
	void     b3Overview(b3_vector *center,b3_vector *size,b3_f64 xAngle,b3_f64 yAngle);
	void     b3ComputeAngles(b3_f64 &xAngle,b3_f64 &yAngle);
	void     b3ComputeFocalLength(b3_f64 length);
	b3_f64   b3GetFocalLength();
	b3_f64   b3GetTwirl();
	void     b3SetTwirl(b3_f64 twirl);
	void     b3ScaleFocalLength(b3_f64 factor);
	void     b3Transform(b3_matrix *transformation);
	char    *b3GetName();
	void     b3SetName(const char *name);
	b3_bool  b3Prepare();
	b3_bool  b3IsActive();
	void     b3Activate(b3_bool activate = true);
};

#define CAMERA_TITLE  1
#define CAMERA_ACTIVE 2

// NEBULAR
class B3_PLUGIN b3Nebular : public b3Special
{
	b3_f64           m_NebularDenom;
public:
	b3Color          m_NebularColor;
	b3_f32           m_NebularVal;

public:
	B3_ITEM_INIT(b3Nebular);
	B3_ITEM_LOAD(b3Nebular);

	void    b3Write();
	b3_bool b3Prepare();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate=true);
	void    b3GetNebularColor(b3Color &result);
	void    b3ComputeNebular(b3Color &input,b3Color &result,b3_f64 distance);
};

// LINES_INFO
enum b3_unit
{
	B3_UNIT_MM = 0,
	B3_UNIT_CM,
	B3_UNIT_IN,
	B3_UNIT_DM,
	B3_UNIT_FT,
	B3_UNIT_M,
	B3_UNIT_MAX
};

enum b3_measure
{
	B3_MEASURE_1 = 0,
	B3_MEASURE_10,
	B3_MEASURE_20,
	B3_MEASURE_50,
	B3_MEASURE_100,
	B3_MEASURE_200,
	B3_MEASURE_500,
	B3_MEASURE_1000,
	B3_MEASURE_CUSTOM,
	B3_MEASURE_MAX
};

class B3_PLUGIN b3ModellerInfo : public b3Special
{
	b3_u32           m_Flags;

public:
	b3_vector        m_Center;
	b3_vector        m_StepMove;
	b3_vector        m_StepRotate;
	b3_f32           m_GridMove;
	b3_f32           m_GridRot;
	b3_bool          m_ResizeFlag;
	b3_bool          m_BBoxTitles;
	b3_bool          m_GridActive;
	b3_bool          m_AngleActive;
	b3_bool          m_CameraActive;
	b3_bool          m_UseSceneLights;
	b3_unit          m_Unit;
	b3_measure       m_Measure;
	b3_u32           m_CustomMeasure;

public:
	B3_ITEM_INIT(b3ModellerInfo);
	B3_ITEM_LOAD(b3ModellerInfo);

	void        b3Write();
	void        b3SnapToGrid(b3_vector *translation);
	void        b3SnapToAngle(b3_f64 &angle);
	b3_f64      b3ScaleUnitToMM();
	const char *b3GetUnitDescr();
	void        b3SetMeasure(b3_u32 measure);
	void        b3SetMeasure(b3_measure measure);
	b3_u32      b3GetMeasure(b3_bool force_custom_value=true);
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

// ANIMATION
class B3_PLUGIN b3Animation : public b3Special
{
	// OK, the following values are only for "Lines"
	b3_count        m_Frames;          // computed number of frames
	b3_count        m_Tracks;          // number of visible tracks
	b3_index        m_TrackIndex;      // start track in window 
	b3_index        m_FrameIndex;      // start frame in window
	b3_count        m_WTracks;         // actual number of tracks
	b3_count        m_WFrames;         // whole of frames
	b3AnimElement  *m_Element;         // actual animation element
	b3_vector       m_AnimCenter;

public:
	b3_f64   m_Start;           // start time (one unit per frame)
	b3_f64   m_End;             // end time (one unit per frame)
	b3_f64   m_Time;            // time point
	b3_f64   m_Neutral;         // neutral point
	b3_count m_FramesPerSecond;
	b3_u32   m_Flags;

public:
	B3_ITEM_INIT(b3Animation);
	B3_ITEM_LOAD(b3Animation);

	static void            b3Register();
	       void            b3Write();

public:
	       void            b3SetAnimElement (b3AnimElement *Element);
	       b3_bool         b3IsActive();
	       void            b3Activate(b3_bool activate = true);

	       void            b3SetAnimation (b3Scene *Global,b3_f64 t);
	       void            b3ResetAnimation (b3Scene *Global);
	       b3_bool         b3ActivateAnimation(b3Scene *scene,b3_bool activate = true);
		   void            b3RecomputeCenter (b3AnimElement *Element,b3_vector *center,b3_f64 t);
	       
		   inline b3_f64   b3AnimTimeCode (b3_index index)
		   {
	           return m_Start + (b3_f64)index / m_FramesPerSecond;
		   }

	       inline b3_index b3AnimFrameIndex (b3_f64 t)
		   {
			   return (b3_index)((t - m_Start) * m_FramesPerSecond);
		   }

	       inline b3_f64   b3ClipTimePoint(b3_f64 val)
		   {
			   	return b3Math::b3Limit(val,m_Start,m_End);
		   }

	inline b3Base<b3Item> *b3GetAnimElementHead()
	{
		return &m_Heads[0];
	}

private:
	       void            b3RecomputeNeutralInverse (b3AnimElement *Element);
		   void            b3GetNeutralPosition(b3AnimElement *Element,b3_vector *neutral);
		   void            b3ApplyTransformation (b3Scene *Global,b3AnimElement *Anim,b3_matrix *transform,b3_f64 t);
	static b3AnimElement  *b3FindSameTrack(b3AnimElement *Element);
};

#define ANIMB_ON     1

#define ANIMF_ON     (1<<ANIMB_ON)

// DISTRIBUTE

enum b3_sample
{
	SAMPLE_REGULAR     = 0x0000,
	SAMPLE_RANDOM      = 0x0100,
	SAMPLE_JITTER      = 0x0200,
	SAMPLE_SEMI_JITTER = 0x0300,
	SAMPLE_SEPARATED   = 0x0400
};

class B3_PLUGIN b3Distribute : public b3Special
{
public:
	b3Array<b3_f64>    m_MotionBlur;
	b3Array<b3_index>  m_TimeIndex;
	b3_u32             m_Type;
	b3_count           m_SamplesPerPixel;
	b3_count           m_SamplesPerFrame;
	b3_f32             m_DepthOfField;
	b3_filter          m_PixelAperture;
	b3_filter          m_FrameAperture;
	b3Filter          *m_FilterPixel;
	b3Filter          *m_FilterFrame;
	b3_f32            *m_Samples;
	b3_count           m_SPP;

public:
	B3_ITEM_INIT(b3Distribute);
	B3_ITEM_LOAD(b3Distribute);

	virtual ~b3Distribute();
	void     b3Write();
	b3_bool  b3IsActive();
	b3_bool  b3IsMotionBlur();
	void     b3PrepareAnimation(b3_res xSize,b3Animation *animation=null);
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

// LENSFLARE
class B3_PLUGIN b3LensFlare : public b3Special
{
public:
	b3_s32         m_Flags;
	b3Color        m_Color;
	b3_f32         m_Expon;

public:
	B3_ITEM_INIT(b3LensFlare);
	B3_ITEM_LOAD(b3LensFlare);

	void    b3Write();
	b3_bool b3IsActive();
	void    b3Activate(b3_bool activate=true);
};

#define LENSFLARE_ACTIVE 1

// CAUSTIC
class B3_PLUGIN b3Caustic : public b3Special
{
public:
	b3_u32   m_Flags;
	b3_count m_NumPhotons;
	b3_count m_TraceDepth;

public:
	B3_ITEM_INIT(b3Caustic);
	B3_ITEM_LOAD(b3Caustic);

	void b3Write();
};

#define CAUSTIC_ENABLE_B 0
#define CAUSTIC_ENABLE   (1 << CAUSTIC_ENABLE_B)

// CLOUDS
class b3CloudBackground : public b3Special, public b3Clouds
{
public:
	B3_ITEM_INIT(b3CloudBackground);
	B3_ITEM_LOAD(b3CloudBackground);

	void    b3Write();
	b3_bool b3Prepare();
};

#endif
