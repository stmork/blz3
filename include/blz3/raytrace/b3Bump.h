/*
**
**	$Filename:	b3Bump.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for bumps
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_BUMP_H
#define B3_RAYTRACE_BUMP_H

#include "blz3/raytrace/b3Scaling.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/base/b3Wood.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        bump mapping                                  **
**                                                                      **
*************************************************************************/

#define CLASS_BUMP          0x28000000
#define TYPE_NOISE          0x00000001
#define TYPE_MARBLE         0x00000002
#define TYPE_TEXTURE        0x00000003
#define TYPE_WATER          0x00000004
#define TYPE_WAVE           0x00000005
#define TYPE_GLOSSY         0x00000006
#define TYPE_GROOVE         0x00000007
#define TYPE_BUMP_WOOD      0x00000008
#define TYPE_BUMP_OAKPLANK  0x00000009

#define BUMP_NOISE          (CLASS_BUMP|TYPE_NOISE)
#define BUMP_MARBLE         (CLASS_BUMP|TYPE_MARBLE)
#define BUMP_TEXTURE        (CLASS_BUMP|TYPE_TEXTURE)
#define BUMP_WATER          (CLASS_BUMP|TYPE_WATER)
#define BUMP_WAVE           (CLASS_BUMP|TYPE_WAVE)
#define BUMP_GLOSSY         (CLASS_BUMP|TYPE_GLOSSY)
#define BUMP_GROOVE         (CLASS_BUMP|TYPE_GROOVE)
#define BUMP_WOOD           (CLASS_BUMP|TYPE_BUMP_WOOD)
#define BUMP_OAKPLANK       (CLASS_BUMP|TYPE_BUMP_OAKPLANK)

class B3_PLUGIN b3Bump : public b3Item
{
public:
	b3_f32        m_Amplitude;           // amplitude

protected:
	B3_ITEM_BASE(b3Bump);

public:
	B3_ITEM_INIT(b3Bump);
	B3_ITEM_LOAD(b3Bump);

	static         void    b3Register();
	virtual        b3_bool b3Prepare()
	{
		return true;
	}
	virtual inline void    b3BumpNormal(b3_ray *ray)
	{
	}
	virtual inline b3_bool b3NeedDeriv()
	{
		return false;
	}
};

// BUMP_NOISE
class B3_PLUGIN b3BumpNoise : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpNoise);
	B3_ITEM_LOAD(b3BumpNoise);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

// BUMP_MARBLE
class B3_PLUGIN b3BumpMarble : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpMarble);
	B3_ITEM_LOAD(b3BumpMarble);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_TEXTURE
class B3_PLUGIN b3BumpTexture : public b3Bump
{
public:
	b3_f32      m_xStart,m_yStart;     // base of bump texture
	b3_f32      m_xScale,m_yScale;     // scale of bump texture
	b3_s32      m_xTimes,m_yTimes;     // repetition
	b3_s32      m_Flags;
	b3Path      m_Name;
	b3Tx       *m_Texture;

public:
	B3_ITEM_INIT(b3BumpTexture);
	B3_ITEM_LOAD(b3BumpTexture);

	       void    b3Write();
	       b3_bool b3Prepare();
	       void    b3SetTexture(const char *name);
	       void    b3BumpNormal(b3_ray *ray);
	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

private:
	b3_bool b3GetNormalDeriv(b3_f64 lx,b3_f64 ly,b3_vector *deriv);
};

// BUMP_WATER
class B3_PLUGIN b3BumpWater : public b3Bump, public b3Water, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpWater);
	B3_ITEM_LOAD(b3BumpWater);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_WAVE
class B3_PLUGIN b3BumpWave : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpWave);
	B3_ITEM_LOAD(b3BumpWave);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_GROOVE
class B3_PLUGIN b3BumpGroove : public b3Bump, public b3Scaling
{
public:
	B3_ITEM_INIT(b3BumpGroove);
	B3_ITEM_LOAD(b3BumpGroove);

	void    b3Write();
	b3_bool b3Prepare();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_GLOSSY */
class B3_PLUGIN b3BumpGlossy : public b3Bump
{
public:
	b3_s32      m_Flags;

public:
	B3_ITEM_INIT(b3BumpGlossy);
	B3_ITEM_LOAD(b3BumpGlossy);

	void b3Write();
	void b3BumpNormal(b3_ray *ray);
};

class B3_PLUGIN b3BumpWooden : public b3Bump, public b3Scaling
{
protected:
	b3_f64      m_dX;
	b3_f64      m_dY;

protected:
	B3_ITEM_BASE(b3BumpWooden);

public:
	B3_ITEM_INIT(b3BumpWooden);
	B3_ITEM_LOAD(b3BumpWooden);
};

// BUMP_WOOD
class B3_PLUGIN b3BumpWood : public b3BumpWooden, public b3Wood
{
public:
	B3_ITEM_INIT(b3BumpWood);
	B3_ITEM_LOAD(b3BumpWood);

	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

	b3_bool b3Prepare();
	void    b3Write();
	void    b3BumpNormal(b3_ray *ray);
};

// BUMP_OAKPLANK
class B3_PLUGIN b3BumpOakPlank : public b3BumpWooden, public b3OakPlank
{
	b3Array<b3_f64> m_Amplitudes;

public:
	B3_ITEM_INIT(b3BumpOakPlank);
	B3_ITEM_LOAD(b3BumpOakPlank);

	inline b3_bool b3NeedDeriv()
	{
		return true;
	}

	b3_bool b3Prepare();
	void    b3Write();
	void    b3BumpNormal(b3_ray *ray);
};

//flags for WaterBump, WaveBump
#define BUMP_IPOINT          1
#define BUMP_U_SUPPRESS_WAVE 2
#define BUMP_V_SUPPRESS_WAVE 4

#endif
