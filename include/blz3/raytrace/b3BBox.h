/*
**
**	$Filename:	b3BBox.h $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing class definitions for bounnding boxes
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_BBOX_H
#define B3_RAYTRACE_BBOX_H

#include "blz3/raytrace/b3Base.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3PrepareInfo.h"
#include "blz3/base/b3Render.h"

/*************************************************************************
**                                                                      **
**                        bounding boxes                                **
**                                                                      **
*************************************************************************/

#define CLASS_BBOX  0x60000000
#define BBOX        CLASS_BBOX

class B3_PLUGIN b3BBox : public b3Item, public b3RenderObject
{
	// Inherited from Blizzard II
	b3_u32           m_Type;               // texture type
	b3_count         m_ShapeCount;
	b3_count         m_CSGIntersectionCount;
	b3_matrix        m_Inverse;

public:
	b3_vector        m_DimBase;
	b3_vector        m_DimSize;
	b3_matrix        m_Matrix;             // all composed transformations
	char             m_BoxName[B3_BOXSTRINGLEN];   // object name
	char             m_BoxURL[B3_BOXSTRINGLEN]; // HTML link

	b3_gl_vertex     m_BBoxVertex[8];

	static b3Color   m_GridColor;
	static b3_bool   m_GridVisible;

public:
	B3_ITEM_INIT(b3BBox);
	B3_ITEM_LOAD(b3BBox);

	static void            b3Register();
	       void            b3Write();
	       void            b3Dump(b3_count level);
		   void            b3SetName(const char *name);
		   void            b3SetupVertexMemory(b3RenderContext *context);
	       void            b3FreeVertexMemory();
	       void            b3Draw(b3RenderContext *context);
		   b3_bool         b3Transform(b3_matrix *transformation,b3_bool is_affine,b3_bool force_action = false);
		   b3_bool         b3Inverse(b3_matrix *);
		   b3_bool         b3Reverse(b3_matrix *);
		   void            b3ResetTransformation();
		   void            b3Activate(b3_bool activate=true,b3_bool recurse=true);
		   void            b3Animate(b3Activation::b3_anim_activation animate = b3Activation::B3_ANIM_ACTIVE,b3_bool recurse=true);
		   b3_bool         b3IsActive();
		   void            b3Expand(b3_bool expand=true);
		   b3_bool         b3IsExpanded();
		   void            b3Update();
		   b3_bool         b3ComputeBounds(b3_vector *lower,b3_vector *upper,b3_f64 tolerance);
	       void            b3ComputeBoxPolar(b3_ray *ray); 
		   b3_count        b3Count();
		   b3_bool         b3Prepare(b3_bool recursive = false);
		   char           *b3GetName();
		   b3_bool         b3BacktraceRecompute(b3BBox *search);
		   b3Base<b3Item> *b3FindBBoxHead(b3BBox  *bbox);
		   b3BBox         *b3FindParentBBox(b3Shape *shape);
	       b3_bool         b3Intersect(b3_ray *ray);
		   b3CSGShape     *b3IntersectCSG(b3_ray *ray);
		   void            b3CollectBBoxes(b3Array<b3BBoxReference> &array);
		   void            b3CollectBBoxes(b3_ray *ray,b3Array<b3BBox *> *array);
		   void            b3CollectBBoxes(b3_vector *lower,b3_vector *upper,b3Array<b3BBox *> *array);

		   void            b3CollectActiveBBoxes(b3Array<b3BBox *> *array,b3_bool activation);
 	static void            b3Reorg(b3Base<b3Item> *depot,b3Base<b3Item> *base,b3_count level,b3_count rec,b3Item *insert_after=null);
	static void            b3Recount(b3Base<b3Item> *base,b3_count level = 1);
	static b3_bool         b3FindBBox(b3Base<b3Item> *base,b3BBox *search);
	static b3BBox         *b3ReadCOB(const char *filename);
	static b3BBox         *b3ReadTGF(const char *filename);

	inline b3Base<b3Item> *b3GetShapeHead()
	{
		return &m_Heads[0];
	}

	inline b3Base<b3Item> *b3GetBBoxHead()
	{
		return &m_Heads[1];
	}

protected:
	inline void b3GetGridColor(b3Color &color)
	{
		color = m_GridColor;
	}

	inline b3_render_mode  b3GetRenderMode()
	{
		return m_GridVisible ? B3_RENDER_LINE : B3_RENDER_NOTHING;
	}

	void    b3AllocVertexMemory(b3RenderContext *context);
	void    b3ComputeVertices();
	void    b3ComputeNormals(b3_bool normalize = true);
	void    b3ComputeIndices();
};

#define BBB_HTML         0
#define BBB_TEXT         1
#define BBB_BOLD         2
#define BBB_ITALIC       3
#define BBB_SANSSERIF    4
#define BBB_TYPEWRITER   5
#define BBB_ACTIVE       6
#define BBB_EXPANDED     7

#define BBF_HTML        (1 << BBB_HTML)
#define BBF_TEXT        (1 << BBB_TEXT)
#define BBF_BOLD        (1 << BBB_BOLD)
#define BBF_ITALIC      (1 << BBB_ITALIC)
#define BBF_SERIF        0
#define BBF_SANSSERIF   (1 << BBB_SANSSERIF)
#define BBF_TYPEWRITER  (1 << BBB_TYPEWRITER)
#define BBF_ACTIVE      (1 << BBB_ACTIVE)
#define BBF_EXPANDED    (1 << BBB_EXPANDED)

#define BB_GETTEXTSIZE(BBox)      (((BBox)->Type & 0x0ff00) >>  8)
#define BB_SETTEXTSIZE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffff00ff) |  ((Size) << 8))
#define BB_GETTEXTTYPE(BBox)      (((BBox)->Type & 0x00030) >>  4)
#define BB_SETTEXTTYPE(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xffffffcf) |  ((Size) << 4))
#define BB_GETROTATION(BBox)     ((((long)((BBox)->Type) & 0x70000) << 13) >> 29)
#define BB_SETROTATION(BBox,Size)  ((BBox)->Type = ((BBox)->Type & 0xfff8ffff) | (((long)(Size) << 29) >> 13))

#endif
