/*
**
**	$Filename:	b3ReadTGF.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting ArCon scenes into Blizzard III
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifdef _DEBUG
#define not_VERBOSE
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"
#include "b3ReadTGF.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Endian.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3TGFReader::b3TGFReader()
{
	m_CameraBase.b3InitBase(CLASS_SPECIAL);
	m_LightBase.b3InitBase(CLASS_LIGHT);
}

b3TGFReader::~b3TGFReader()
{
	m_CameraBase.b3Free();
	m_LightBase.b3Free();
}

b3_size b3TGFReader::b3StrCpy(
	char *dst,
	const char *src,
	b3_size    	dst_size,
	b3_size     src_size)
{
	b3_size len;

	if (dst_size < src_size)
	{
		len = dst_size - 1;
	}
	else
	{
		len = src_size;
	}
	strlcpy(dst,src,len);
	return strlen(dst);
}

b3_bool b3TGFReader::b3ParseCamera(char *ptr)
{
	b3CameraPart *camera = new b3CameraPart(CAMERA);
	b3_vector     view;

	b3StrCpy(camera->m_CameraName,ptr,sizeof(camera->m_CameraName),81);
	ptr += 81;

	// Ignore camera type
	ptr += 2;

	// Get eye point
	camera->m_EyePoint.x = b3Endian::b3GetIntelDouble(&ptr[ 0]) / 1000000.0;
	camera->m_EyePoint.y = b3Endian::b3GetIntelDouble(&ptr[ 8]) / 1000000.0;
	camera->m_EyePoint.z = b3Endian::b3GetIntelDouble(&ptr[16]) / 1000000.0;
	ptr += 24;

	camera->m_ViewPoint.x = camera->m_EyePoint.x + b3Endian::b3GetIntelDouble(&ptr[ 0]) / 1000000.0;
	camera->m_ViewPoint.y = camera->m_EyePoint.y + b3Endian::b3GetIntelDouble(&ptr[ 8]) / 1000000.0;
	camera->m_ViewPoint.z = camera->m_EyePoint.z + b3Endian::b3GetIntelDouble(&ptr[16]) / 1000000.0;
	ptr += 24;

	camera->m_Height.x = b3Endian::b3GetIntelDouble(&ptr[ 0]) / 1000000.0;
	camera->m_Height.y = b3Endian::b3GetIntelDouble(&ptr[ 8]) / 1000000.0;
	camera->m_Height.z = b3Endian::b3GetIntelDouble(&ptr[16]) / 1000000.0;

	b3Vector::b3Sub(&camera->m_ViewPoint,&camera->m_EyePoint,&view);
	b3Vector::b3CrossProduct(&view,&camera->m_Height,&camera->m_Width);

	m_CameraBase.b3Append(camera);
	return true;
}

b3_bool b3TGFReader::b3ParseLight(char *ptr)
{
	b3Light     *light = new b3Light(SPOT_LIGHT);
	b3_vector64  pos,dir;
	b3_u16       type;
	b3_f64       attenuation,cone,spot;

	b3StrCpy(light->m_Name,ptr,sizeof(light->m_Name),81);
	ptr += 81;

	type = b3Endian::b3GetIntel16(&ptr[0]);
	light->m_LightActive = ptr[2];
	ptr +=3;

	pos.x = b3Endian::b3GetIntelDouble(&ptr[ 0]) / 1000000.0;
	pos.y = b3Endian::b3GetIntelDouble(&ptr[ 8]) / 1000000.0;
	pos.z = b3Endian::b3GetIntelDouble(&ptr[16]) / 1000000.0;
	dir.x = b3Endian::b3GetIntelDouble(&ptr[24]) / 1000000.0;
	dir.y = b3Endian::b3GetIntelDouble(&ptr[32]) / 1000000.0;
	dir.z = b3Endian::b3GetIntelDouble(&ptr[40]) / 1000000.0;
	b3Vector::b3Init(&light->m_Position, pos.x,pos.y,pos.z);
	b3Vector::b3Init(&light->m_Direction,dir.x,dir.y,dir.z);
	ptr += 48;

	light->m_Color.b3Init(
		b3Endian::b3GetIntelFloat(&ptr[0]),
		b3Endian::b3GetIntelFloat(&ptr[4]),
		b3Endian::b3GetIntelFloat(&ptr[8]));
	ptr += 12;

	attenuation = b3Endian::b3GetIntelDouble(&ptr[ 0]);
	cone        = b3Endian::b3GetIntelDouble(&ptr[ 8]);
	spot        = b3Endian::b3GetIntelDouble(&ptr[16]);

	switch (type)
	{
	case 0:
	case 1:
	case 2:
		break;

	case 3:
		light->m_SpotActive = true;
		break;

	default:
		break;
	}
	m_LightBase.b3Append(light);
	return true;
}

b3_bool b3TGFReader::b3ProcessMaterial(b3Shape *shape,b3_index index)
{
	b3_count max = m_Materials.b3GetCount();
	b3_index i;

	for (i = 0;i < max;i++)
	{
		if (m_Materials[i].m_Index == index)
		{
			b3MatNormal *material = new b3MatNormal(MATERIAL);

			material->m_Diffuse = m_Materials[i].m_Color;
			shape->b3GetMaterialHead()->b3Append(material);
			return true;
		}
	}
	return false;
}

b3Triangles *b3TGFReader::b3ProcessOneShape(
	b3Array<b3_vertex>      &vertices,
	b3Array<b3_tgf_facette> &facettes,
	b3_index                 facStart,
	b3_index                 facEnd)
{
	b3Triangles *shape = new b3Triangles(TRIANGLES);
	b3_count     k,min,max,start,end,face = 0;
	b3_count     faces = 0,diff;

	// Count faces, compute vertex range
	min = vertices.b3GetCount();
	max = 0;
	for (k = facStart;k < facEnd;k++)
	{
		start = facettes[k].m_Start;
		end   = facettes[k].m_End;
		if (start < min) min = start;
		if (end   > max) max = end;
		faces += end - start - 2;
	}

	// Init shape
#ifdef VERBOSE
	b3PrintF(B3LOG_FULL,"F: %5d - %5d # %5d faces # V: %5d - %5d\n",
			 facStart,facEnd,faces,min,max);
#endif
	shape->b3Init(max - min,faces,0,0);

	// Copy needed amount of vertices
	memcpy(shape->m_Vertices,&vertices[min],(max - min) * sizeof(b3_vertex));

	// Copy triangles
	for (k = facStart;k < facEnd;k++)
	{
		start  = facettes[k].m_Start - min;
		end    = facettes[k].m_End   - min;
		diff   = end - start;

		for (b3_index f = 2;f < diff;f++)
		{
			shape->m_Triangles[face].P1 = start;
			shape->m_Triangles[face].P2 = start + f - 1;
			shape->m_Triangles[face].P3 = start + f;
			face++;
		}
	}
	B3_ASSERT(faces == face);
	return shape;
}

b3_bool b3TGFReader::b3ParseShapes(
	b3BBox                  *bbox,
	b3Array<b3_vertex>      &vertices,
	b3Array<b3_tgf_facette> &facettes)
{
	b3Triangles *shape;
	b3_index     i,facStart,facEnd;
	b3_count     numFac,count;
	b3_index     matIndex;

	facStart = 0;
	numFac  = facettes.b3GetCount();
	for (i = 0;i < numFac;i += count)
	{
		matIndex = facettes[i].m_MatIndex;
		facStart = i;
		for (
			facEnd = i + 1;
			(facEnd < numFac) && (facettes[facEnd].m_MatIndex == matIndex);
			facEnd++)
		{
		}
		count = facEnd - facStart;
		shape = b3ProcessOneShape(vertices,facettes,facStart,facEnd);
		b3ProcessMaterial(shape,matIndex);
		bbox->b3GetShapeHead()->b3Append(shape);
	}
	return true;
}

b3_bool b3TGFReader::b3ParseGeometry(b3BBox *bbox, char *ptr)
{
	b3Array<b3_vertex>       vertices;
	b3Array<b3_tgf_facette>  facettes;
	b3_vertex                vertex;
	b3_tgf_facette           facette;
	b3_u32                   numVert;
	b3_u32                   numAttr;
	b3_u32                   numFac;
	b3_u32                   numDef;
	b3_tgf_vertex            type;
	b3_size                  size,skip,i,pos;
	b3_f64                  *vPtr,u,v;
	b3_u32                  *lPtr;
	b3_u16                  *sPtr;

	numVert = b3Endian::b3GetIntel32(&ptr[ 0]);
	numAttr = b3Endian::b3GetIntel32(&ptr[ 4]);
	numFac  = b3Endian::b3GetIntel32(&ptr[ 8]);
	numDef  = b3Endian::b3GetIntel32(&ptr[12]);
	type    = (b3_tgf_vertex)b3Endian::b3GetIntel16(&ptr[16]);
	switch (type)
	{
	case TGF_VERTEX_POINT:
		size = 3;
		break;
	case TGF_VERTEX_POINT_NORMAL:
		size = 6;
		break;
	case TGF_VERTEX_POINT_NORMAL_TEX:
		size = 8;
		break;
	default:
		return false;
	}
	ptr += 18;
	pos  = 18;

	vPtr  = (b3_f64 *)ptr;
	skip  = numVert * sizeof(b3_f64) * size;
	for (i = 0;i < numVert;i++)
	{
		vertex.Point.x = b3Endian::b3GetIntelDouble(&vPtr[0]) / 1000000.0;
		vertex.Point.y = b3Endian::b3GetIntelDouble(&vPtr[1]) / 1000000.0;
		vertex.Point.z = b3Endian::b3GetIntelDouble(&vPtr[2]) / 1000000.0;
		if (size > 3)
		{
			vertex.Normal.x = b3Endian::b3GetIntelDouble(&vPtr[3]) / 1000000.0;
			vertex.Normal.y = b3Endian::b3GetIntelDouble(&vPtr[4]) / 1000000.0;
			vertex.Normal.z = b3Endian::b3GetIntelDouble(&vPtr[5]) / 1000000.0;
			if (size > 6)
			{
				u = b3Endian::b3GetIntelDouble(&vPtr[6]);
				v = b3Endian::b3GetIntelDouble(&vPtr[7]);
			}
		}
		vertices.b3Add(vertex);
		vPtr += size;
	}
	ptr  += skip;
	pos  += skip;

	skip  = numVert * sizeof(b3_u08);
	ptr  += skip;
	pos  += skip;

	sPtr  = (b3_u16 *)ptr;
	skip  = numAttr * sizeof(b3_u16) * 3;
	for (i = 0;i < numAttr;i++)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"%5u %5u - %5u\n",
				 b3Endian::b3GetIntel16(&sPtr[0]),
				 b3Endian::b3GetIntel16(&sPtr[1]),
				 b3Endian::b3GetIntel16(&sPtr[2]));
#endif
		sPtr += 3;
	}
	ptr  += skip;
	pos  += skip;

	lPtr  = (b3_u32 *)ptr;
	skip  = numFac  * sizeof(b3_u32) * 3;
	for (i = 0;i < numFac;i++)
	{
		facette.m_Start    = b3Endian::b3GetIntel32(&lPtr[0]);
		facette.m_End      = b3Endian::b3GetIntel32(&lPtr[1]);
		facette.m_MatIndex = b3Endian::b3GetIntel32(&lPtr[2]);
		facettes.b3Add(facette);
		lPtr += 3;
	}
	ptr  += skip;
	pos  += skip;

	lPtr  = (b3_u32 *)ptr;
	skip  = numDef  * sizeof(b3_u32) * 6;
	for (i = 0;i < numDef;i++)
	{
#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"%5u %5u %5u # %5u %5u %5u\n",
				 b3Endian::b3GetIntel32(&lPtr[0]),
				 b3Endian::b3GetIntel32(&lPtr[1]),
				 b3Endian::b3GetIntel32(&lPtr[2]),
				 b3Endian::b3GetIntel32(&lPtr[3]),
				 b3Endian::b3GetIntel32(&lPtr[4]),
				 b3Endian::b3GetIntel32(&lPtr[5]));
#endif
		lPtr += 6;
	}
	ptr  += skip;
	pos  += skip;

	return b3ParseShapes(bbox,vertices,facettes);
}

b3_bool b3TGFReader::b3ParseMaterial(char *ptr)
{
	b3_tgf_material mat;

	// Ignoring material name
	ptr += strlen(ptr) + 1;

	mat.m_Index = b3Endian::b3GetIntel32(ptr);
	b3ColorBase::b3Init(&mat.m_Color,
						b3Endian::b3GetIntelFloat(&ptr[ 4]),
						b3Endian::b3GetIntelFloat(&ptr[ 8]),
						b3Endian::b3GetIntelFloat(&ptr[12]));

	m_Materials.b3Add(mat);
	return true;
}

b3BBox *b3TGFReader::b3Parse(char *ptr,b3_size size,const char *filename) throw(b3WorldException)
{
	b3BBox     *bbox  = new b3BBox(BBOX);
	b3_count    cameras = 0;
	b3_size     pos = 0;
	b3_tgf_tag  tag;
	b3_u32      tagSize;
	b3_bool     error = false;

	while ((pos < size) && (!error))
	{
		tag     = (b3_tgf_tag)b3Endian::b3GetIntel16(&ptr[0]);
		tagSize = b3Endian::b3GetIntel32(&ptr[2]);
		ptr += 6;
		pos += 6;

#ifdef VERBOSE
		b3PrintF(B3LOG_FULL,"Tag: %4d size: %9d\n",tag,tagSize);
#endif
		switch (tag)
		{
		case TGF_HEADER_TAG:
			error = strcmp(ptr,"GAMMA TGF") != 0;
			if (!error)
			{
				b3PrintF(B3LOG_NORMAL,"GAMMA TGF Version %1.2f\n",
						 (double)b3Endian::b3GetIntel16(&ptr[10]) / 100);
			}
			break;

		case TGF_ORIGIN_TAG:
			b3PrintF(B3LOG_NORMAL,"TGF-Origin: %-81.81s\n",ptr);
			break;

		case TGF_GEOMETRY_TAG:
			error = !b3ParseGeometry(bbox,ptr);
			break;

		case TGF_TRANSFORM_TAG:
			break;

		case TGF_COPY_TRANSFORM_TAG:
			break;

		case TGF_LIGHT_TAG:
			error = !b3ParseLight(ptr);
			break;

		case TGF_CAMERA_TAG:
			error = !b3ParseCamera(ptr);
			cameras++;
			break;

		case TGF_MATERIAL_TAG:
			error = !b3ParseMaterial(ptr);
			break;

		case TGF_SHELL_INDEX_TAG:
			break;

		case TGF_SHELL_POLY_TAG:
			break;

		case TGF_SHELL_CLOSURE_TAG:
			break;

		case TGF_SHELL_VERTEX_TAG:
			break;

		case TGF_SHELL_VERTEX_NORMAL_TAG:
			break;

		case TGF_SHELL_POLY_NORMAL_TAG:
			break;

		case TGF_SHELL_VERTEX_TEXTURE_TAG:
			break;

		case TGF_SHELL_MATERIAL_TAG:
			break;

		case TGF_SHELL_TAG:
			break;

		default:
			b3PrintF(B3LOG_NORMAL,"Unknown TGF tag: %04x\n",tag);
			error = true;
			break;
		}
		ptr += tagSize;
		pos += tagSize;
	}

	if (!error)
	{
		b3Path name;

		b3Path::b3SplitFileName(filename,null,name);
		name.b3RemoveExt();
		strlcpy (bbox->m_BoxName,name,sizeof(bbox->m_BoxName));
	}
	else
	{
		delete bbox;
		bbox = null;
		B3_THROW(b3WorldException,B3_WORLD_IMPORT);
	}
	return bbox;
}

b3Scene *b3TGFReader::b3ReadTGFScene(const char *tgffile)
{
	b3Scene     *scene = new b3Scene(TRACEPHOTO_MORK);
	b3BBox      *bbox;
	b3TGFReader  reader;
	b3File       file;
	char        *buffer;
	b3_size      size;

	b3PrintF(B3LOG_NORMAL,"Reading TGF %s\n",tgffile);
	buffer = (char *)file.b3ReadBuffer(tgffile,size);
	if (buffer != null)
	{
		bbox = reader.b3Parse(buffer,size,tgffile);
		if (bbox != null)
		{
			b3ModellerInfo *info;

			// Setup at least one camera
			if (reader.m_CameraBase.b3IsEmpty())
			{
				b3CameraPart *camera = new b3CameraPart(CAMERA);

				reader.m_CameraBase.b3Append(camera);
			}

			// Setup some Lines info
			info = scene->b3GetModellerInfo();
			info->m_Unit             = B3_UNIT_MM;
			info->m_AngleGridCamera  = false;
			info->m_AngleGridObjects = true;

			// Setup class hierarchy
			scene->b3GetBBoxHead()->b3Append(bbox);
			scene->b3GetSpecialHead()->b3Move(&reader.m_CameraBase);
			scene->b3GetLightHead()->b3Move(&reader.m_LightBase);
			scene->b3SetFilename(tgffile);
		}
	}
	return scene;
}

b3BBox *b3TGFReader::b3ReadTGFBBox(const char *tgffile)
{
	b3TGFReader  reader;
	b3BBox      *bbox = null;
	b3File       file;
	char        *buffer;
	b3_size      size;

	b3PrintF(B3LOG_NORMAL,"Reading TGF %s\n",tgffile);
	buffer = (char *)file.b3ReadBuffer(tgffile,size);
	if (buffer != null)
	{
		bbox = reader.b3Parse(buffer,size,tgffile);
	}
	return bbox;
}
