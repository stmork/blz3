/*
**
**      $Filename:      b3ReadTGF.h $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Converting ArCon scenes into Blizzard III
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_READTGF_H
#define B3_READTGF_H

#include "blz3/raytrace/b3Raytrace.h"

enum b3_tgf_tag
{
	// Version 1.0
	TGF_HEADER_TAG          =   0,
	TGF_ORIGIN_TAG          =   1,
	TGF_GEOMETRY_TAG        = 300,
	TGF_TRANSFORM_TAG       = 301,
	TGF_COPY_TRANSFORM_TAG  = 302,
	TGF_LIGHT_TAG           = 310,
	TGF_CAMERA_TAG          = 311,
	TGF_MATERIAL_TAG        = 312,

	// Version 1.1
	TGF_SHELL_INDEX_TAG          = 313,
	TGF_SHELL_POLY_TAG           = 314,
	TGF_SHELL_CLOSURE_TAG        = 315,
	TGF_SHELL_VERTEX_TAG         = 316,
	TGF_SHELL_VERTEX_NORMAL_TAG  = 317,
	TGF_SHELL_POLY_NORMAL_TAG    = 318,
	TGF_SHELL_VERTEX_TEXTURE_TAG = 319,
	TGF_SHELL_MATERIAL_TAG       = 320,
	TGF_SHELL_TAG                = 321
};

enum b3_tgf_vertex
{
	TGF_VERTEX_POINT,
	TGF_VERTEX_POINT_NORMAL,
	TGF_VERTEX_POINT_NORMAL_TEX
};

struct b3_tgf_material
{
	b3_index  m_Index;
	b3_color  m_Color;
};

struct b3_tgf_facette
{
	b3_u32   m_Start;
	b3_u32   m_End;
	b3_index m_MatIndex;
};

class b3TGFReader
{
	b3Array<b3_tgf_material> m_Materials;
	b3Base<b3Item>           m_CameraBase;
	b3Base<b3Item>           m_LightBase;

	             b3TGFReader();
				~b3TGFReader();
	b3BBox      *b3Parse(char *buffer,b3_size size,const char *filename) throw(b3WorldException);
	b3_bool      b3ParseCamera(char *ptr);
	b3_bool      b3ParseLight(char *ptr);
	b3_bool      b3ParseMaterial(char *tr);
	b3_bool      b3ParseGeometry(b3BBox *bbox,char *ptr);
	b3_bool      b3ParseShapes(b3BBox *bbox,b3Array<b3_vertex> &vertices,b3Array<b3_tgf_facette> &facettes);
	b3Triangles *b3ProcessOneShape(b3Array<b3_vertex> &vertices,b3Array<b3_tgf_facette> &facettes,b3_index facStart,b3_index facEnd);
	b3_bool      b3ProcessMaterial(b3Shape *shape,b3_index matIndex);

	static b3_size b3StrCpy(char *dst,const char *src,b3_size dstSize,b3_size srcSize);

public:
	static b3Scene *b3ReadTGFScene(const char *filename);
	static b3BBox  *b3ReadTGFBBox(const char *filename);
};

#endif
