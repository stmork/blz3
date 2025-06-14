/*
**
**	$Filename:	b3ReadCOB.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting Caligari objects into Blizzard III
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifdef _DEBUG
#define no_VERBOSE_INDEX
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3ReadCOB.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Aux.h"
#include "blz3/raytrace/b3Material.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3COBReader::b3COBReader()
{
	cobInfos.b3InitBase();
	cobDone.b3InitBase();
}

b3COBReader::~b3COBReader()
{
	cobInfos.b3Free();
	cobDone.b3Free();
}

/* This routine allocates an info structure to identify an caligari object. */
/* The info stores information about id, parent id and the created Blizzard */
/* structure. The info is appended in the cobInfo list. Later, when the */
/* Blizzard hierarchy is processed, the first object is removed to get */
/* the linkuage to the done objects. */
/* ------------------------------------------------------------------------ */
/* node:   created Blizzard object */
/* id:     ID of COB */
/* parent: parent ID of COB */
/* type:   COB type */

b3_bool b3COBReader::b3COB_AllocObject(
	b3Item   *   node,
	b3_cob_id    id,
	b3_cob_id    parent,
	b3_cob_type  type)
{
	b3COBInfo * cobInfo;

	cobInfo = new b3COBInfo(node, id, parent, type);
	if (cobInfo)
	{
		cobInfos.b3Append(cobInfo);
	}
	return cobInfo != nullptr;
}

/* This routine tries to find an cobInfo in the cobDone list identified by */
/* an ID. It returns the pointer to the found info. */
/* ----------------------------------------------------------------------- */
/* id: ID to search for. */

b3COBInfo * b3COBReader::b3COB_FindInfo(b3_cob_id id)
{
	b3COBInfo * cobInfo;

	B3_FOR_BASE(&cobDone, cobInfo)
	{
		if (cobInfo->refID == id)
		{
			return cobInfo;
		}
	}
	return nullptr;
}

/* This routine reconstructs the hierarchy of Caligari objects to the */
/* Blizzard data structure. This will be done in two phases: */
/* 1. find root object, remove it from object list, append it to done list. */
/* 2. take first object from object list, find parent ID in done list. */
/*    create linkuage in Blizzard data structure, remove object from */
/*    object list, append it to done list. */
/* 3. repeat until object list is empty. */
/* The routine returns the Blizzard object of the root object. */

b3Item * b3COBReader::b3COB_Reconstruct()
{
	const b3BBox      *     BBox;
	const b3TriangleShape * Shape;
	b3BBox         *        root = nullptr;
	b3COBInfo       *       cobInfo;
	b3COBInfo       *       next;

	/* First find root object in object list. */
	/* Follow error messages for exceptions! */
	for (cobInfo  = cobInfos.First;
		cobInfo != nullptr;
		cobInfo  = next)
	{
		next = cobInfo->Succ;
		if (cobInfo->refParent == 0)
		{
			if (cobInfo->refType != COB_GROU)
			{
				b3PrintF(B3LOG_NORMAL, " ERROR: root object not of required group type!\n");
				return nullptr;
			}

			if (root != nullptr)
			{
				b3PrintF(B3LOG_NORMAL, " ERROR: more than one root object found!\n");
				return nullptr;
			}

			root = static_cast<b3BBox *>(cobInfo->refNode);

			cobInfos.b3Remove(cobInfo);
			cobDone.b3Append(cobInfo);
		}
	}

	if (root == nullptr)
	{
		b3PrintF(B3LOG_NORMAL, " ERROR: no root object found!\n");
		return nullptr;
	}


	/* Second process object list for linkuage to objects in */
	/* done list. */
	/* ----------------------------------------------------- */
	/* NOTE: only following data links are possible: */
	/* groups    to groups */
	/* polygons  to groups */
	/* materials to polygons */

	while ((cobInfo = cobInfos.First) != nullptr)
	{
		next = b3COB_FindInfo(cobInfo->refParent);
		if (next == nullptr)
		{
			b3PrintF(B3LOG_NORMAL, " ERROR: object with parent ID %d not found!\n",
				cobInfo->refParent);
			return nullptr;
		}

		switch (cobInfo->refType)
		{
		case COB_GROU :
			switch (next->refType)
			{
			case COB_GROU :
#ifndef ONE_BBOX
				BBox = static_cast<b3BBox *>(next->refNode);
				BBox->b3GetBBoxHead()->b3Append(cobInfo->refNode);
#endif
				break;

			default :
				b3PrintF(B3LOG_NORMAL, " ERROR: Cannot insert BBox into this kind of object!\n");
				return nullptr;
			}
			break;

		case COB_POLH :
			switch (next->refType)
			{
			case COB_GROU :
#ifndef ONE_BBOX
				BBox = static_cast<b3BBox *>(next->refNode);
#else
				BBox = root;
#endif
				BBox->b3GetShapeHead()->b3Append(cobInfo->refNode);
				break;

			default :
				b3PrintF(B3LOG_NORMAL, " ERROR: Cannot insert polygons into this kind of object!\n");
				return nullptr;
			}
			break;

		case COB_MAT1 :
			switch (next->refType)
			{
			case COB_POLH :
				Shape = static_cast<b3TriangleShape *>(next->refNode);
				Shape->b3GetMaterialHead()->b3Append(cobInfo->refNode);
				break;

			default :
				b3PrintF(B3LOG_NORMAL, " ERROR: Cannot insert material into this kind of object!\n");
				return nullptr;
			}
			break;

		// Do nothing!
		default:
			break;
		}

		/* put object from object list to done list */
		cobInfos.b3Remove(cobInfo);
		cobDone.b3Append(cobInfo);
	}

	//	RecountBBox (root,1L);
	return root;
}

void b3COBReader::b3COB_ComputeAvrgColor(
	const b3Tx * texture,
	b3Color   &  color)
{
	b3_coord     x, y, xMax, yMax;

	color.b3Init();
	xMax = texture->xSize;
	yMax = texture->ySize;
	for (y = 0; y < yMax; y++)
	{
		for (x = 0; x < xMax; x++)
		{
			color = texture->b3GetHdrValue(x, y);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        routines for parsing ASCII file               **
**                                                                      **
*************************************************************************/

/* This routine copies one line of the COB file from the text buffer */
/* into a line buffer. The end of line is marked by a CR (0x0a) even */
/* on MS DOS systems. The routine returns the length of the line */
/* without the ending 0-byte. */
/* ----------------------------------------------------------------- */
/* line:    line buffer */
/* buffer:  text buffer where to copy line from */
/* maxLine: size of line buffer. If the line buffer is not long enough */
/*          the routine returns the negative length of the line buffer. */

b3_size b3COBReader::b3COB_GetLine(
	char * line,
	const char * buffer,
	b3_size     maxLine)

{
	b3_size i;

	for (i = 0; i < maxLine; i++)
	{
		switch (*buffer)
		{
		case 10 :
		case 13 :
			*line = 0;
			if (buffer[1] == 13)
			{
				i++;
			}
			return i;
		}
		*line++ = *buffer++;
	}
	--line;
	*line = 0;
	return 1 - i;
}

/* This routine compresses a command line to a simple token. */
/* --------------------------------------------------------- */
/* buffer: memory buffer of command line */

b3_cob_type b3COBReader::b3COB_GetToken(const char * buffer)
{
	if (strncmp(buffer, "Grou", 4) == 0)
	{
		return COB_GROU;
	}
	if (strncmp(buffer, "PolH", 4) == 0)
	{
		return COB_POLH;
	}
	if (strncmp(buffer, "Mat1", 4) == 0)
	{
		return COB_MAT1;
	}
	if (strncmp(buffer, "END ", 4) == 0)
	{
		return COB_END;
	}
	if (strncmp(buffer, "Unit", 4) == 0)
	{
		return COB_UNIT;
	}
	if (strncmp(buffer, "DefP", 4) == 0)
	{
		return COB_DEFP;
	}
	if (strncmp(buffer, "Chan", 4) == 0)
	{
		return COB_CHAN;
	}
	if (strncmp(buffer, "DDiv", 4) == 0)
	{
		return COB_DDIV;
	}

	return COB_UNKNOWN;
}

/* This routine processes a group chunk. It reads the ID, the parent ID and */
/* the group name to store it for the BBox name. It then creates a Blizzard */
/* bounding box and cobInfo structure. */
/* ------------------------------------------------------------------------ */
/* buffer: buffer where the group chunk starts. */

b3_size b3COBReader::b3COB_ParseGrou(
	const char * buffer,
	const char * name B3_UNUSED)
{
	b3BBox  *  BBox;
	char       line[MAX_LINE];
	char       box_name[B3_BOXSTRINGLEN];
	b3_size    len;
	b3_cob_id  id, parent;
	b3_count   ver, rev;
	b3_size    size;

	len = b3COB_GetLine(line, buffer, sizeof(line));
	sscanf(line, "Grou V%d.%d Id %d Parent %d Size %08zd",
		&ver, &rev, &id, &parent, &size);

	b3COB_GetLine(line, &buffer[len + 1], sizeof(line));
	if (sscanf(line, "Name %32s", box_name) != 1)
	{
		b3Mem::b3StrCpy(box_name, "bbox", sizeof(box_name));
	}

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "G: V%d.%02d ID: %8d P: %8d - size: %8zd,%8zd\n",
		ver, rev, id, parent, len, size);
#endif

	BBox = new b3BBox(BBOX);
	if (BBox != nullptr)
	{
		b3Mem::b3StrCpy(BBox->m_BoxName, box_name, sizeof(BBox->m_BoxName));
		if (!b3COB_AllocObject(BBox, id, parent, COB_GROU))
		{
			delete BBox;
		}
	}
	return len + size;
}

/* This routine parses a polygon chunk. This will be done in two steps. */
/* 1. Compute number of vertices, polygons and triangles and overall */
/*    transformation. */
/* 2. After creation of Blizzard triangle shape read vertices and */
/*    triangle ID's. */
/* --------------------------------------------------------------------  */
/* buffer: memory buffer containing polygon chunk */
/* name:   file name of COB */

b3_size b3COBReader::b3COB_ParsePolH(
	const char * buffer,
	const char * name)
{
	b3Triangles   *  TriaShape;
	b3_vertex    *   vert;
	b3_vector        pos;
	b3_matrix        transform;
	char             line[MAX_LINE];
	b3_size          vPos = 0, fPos = 0;
	b3_count         ver, rev;
	b3_cob_id        id, parent;
	b3_count         count, vertices = 0, faces = 0, polygons = 0, k;
#ifdef _DEBUG
	b3_count         dbgCount = 0;
#endif
	b3_size          size, len, i;
	b3_index         index;

	len = b3COB_GetLine(line, buffer, sizeof(line));
	sscanf(line, "PolH V%d.%d Id %d Parent %d Size %08zd",
		&ver, &rev, &id, &parent, &size);

	if (parent == 0)
	{
		b3BBox * BBox = new b3BBox(BBOX);

		if (BBox != nullptr)
		{
			b3Mem::b3StrCpy(BBox->m_BoxName, name, sizeof(BBox->m_BoxName));
			if (!b3COB_AllocObject(BBox, 1, parent, COB_GROU))
			{
				delete BBox;
			}
#ifdef _DEBUG
			else
			{
				b3PrintF(B3LOG_FULL, "R: V%d.%02d ID: %8d P: %8d - size: %8d,%8d\n",
					0, 0, 1, 0, 0, 0);
			}
#endif
		}
		parent = 1;
	}

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "P: V%d.%02d ID: %8d P: %8d - size: %8zd,%8zd\n",
		ver, rev, id, parent, len, size);
#endif

	size += len;
	b3Matrix::b3Unit(&transform);
	for (i = len + 1; i < size; i += (len + 1))
	{
		len = b3COB_GetLine(line, &buffer[i], sizeof(line));

		/* read number of polygons, number of triangles */
		if (sscanf(line, "Faces %d", &polygons) == 1)
		{
			fPos  = i + len + 1;
			faces = 0;
			for (ver = 0; ver < polygons; ver++)
			{
				i += (len + 1);
				len = b3COB_GetLine(line, &buffer[i], sizeof(line));
				rev = 2;
				if (sscanf(line, "Face verts %d", &rev) == 1)
				{
					faces += (rev - 2);
				}

				i += (len + 1);
				len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			}
		}

		/* read number of vertices */
		if (sscanf(line, "World Vertices %d", &vertices) == 1)
		{
			vPos  = i + len + 1;
		}

		/* read transformation matrix */
		if (strcmp(line, "Transform") == 0)
		{
			i += (len + 1);
			len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			sscanf(line, "%f %f %f %f",
				&transform.m11, &transform.m12, &transform.m13, &transform.m14);

			i += (len + 1);
			len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			sscanf(line, "%f %f %f %f",
				&transform.m21, &transform.m22, &transform.m23, &transform.m24);

			i += (len + 1);
			len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			sscanf(line, "%f %f %f %f",
				&transform.m31, &transform.m32, &transform.m33, &transform.m34);

			i += (len + 1);
			len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			sscanf(line, "%f %f %f %f",
				&transform.m41, &transform.m42, &transform.m43, &transform.m44);
		}
	}
#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "%d vertices, %d polygons, %d triangles\n",
		vertices, polygons, faces);
#endif

	TriaShape = new b3Triangles(TRIANGLES);
	if (TriaShape != nullptr)
	{
		TriaShape->b3Init(vertices, faces, 1, 1);
		if (!b3COB_AllocObject(TriaShape, id, parent, COB_POLH))
		{
			delete TriaShape;
		}
		else
		{
			b3_triangle   *  tria;

			vert = TriaShape->m_Vertices;
			tria = TriaShape->m_Triangles;

			/* read vertices */
			i = vPos;
			for (count = 0; count < vertices; count++)
			{
				len = b3COB_GetLine(line, &buffer[i], sizeof(line));
				sscanf(line, "%f %f %f", &pos.x, &pos.y, &pos.z);
				b3Matrix::b3VMul(&transform, &pos, &vert[count].Point, true);
				i += (len + 1);
			}

			/* loop for reading triangle indices */
			i = fPos; /* outer polygon loop */
			for (count = 0; count < polygons; count++)
			{
				len = b3COB_GetLine(line, &buffer[i], sizeof(line));
				rev = 0;
				if (sscanf(line, "Face verts %d", &rev) == 1)
				{
					rev -= 2;
				}
				i += (len + 1);

				len = b3COB_GetLine(line, &buffer[i], sizeof(line));
				i += (len + 1);
#ifndef SINGLE_CYCLE
				b3Array<b3_index> IDs;
				b3_index          u, l;
				b3_size           read;

				/* first, scan indices */
				rev += 2;
				for (k = 0, len = 0; k < rev; k++)
				{
					sscanf(&line[len], "<%zd,%*d> %d", &read, &index);
					IDs.b3Add(read);
					len += index;
				}

				l = 1;
				u = rev;
				for (k = 2; k < rev; k++)
				{
					b3_index P1, P2, P3;

					P1 = u % rev;
					P2 = l;
					if (k & 1)
					{
						u--;
						P3 = u % rev;
					}
					else
					{
						l++;
						P3 = l;
					}
					tria->P1 = IDs[P1];
					tria->P2 = IDs[P2];
					tria->P3 = IDs[P3];
#ifdef VERBOSE_INDEX
					b3PrintF(B3LOG_FULL, "%3ld - %3ld - %3ld\n", tria->P1, tria->P2, tria->P3);
#endif
					tria++;
#ifdef _DEBUG
					dbgCount++;
#endif
				}
#else
				b3_index id1, id2;
				sscanf(line, "<%ld,%*d> <%ld,%*d> %n", &id1, &id2, &index);

				len = index;
				for (k = 0; k < rev; k++) /* inner triangulation loop */
				{
					tria->P1 = id1;
					tria->P2 = id2;

					sscanf(&line[len], "<%ld,%*d> %n", &tria->P3, &index);
					id2  = tria->P3;
					len += index;

#ifdef VERBOSE_INDEX
					b3PrintF(B3LOG_FULL, "%6ld %6ld %6ld\n", tria->P1, tria->P2, tria->P3);
#endif
					tria++;
#ifdef _DEBUG
					dbgCount++;
#endif
				}
#endif
#ifdef VERBOSE_INDEX
				b3PrintF(B3LOG_FULL, "---\n");
#endif
			}
#ifdef _DEBUG
#ifdef VERBOSE_INDEX
			b3PrintF(B3LOG_FULL, "%ld triangles expected, %ld triangles counted.\n", faces, dbgCount);
#endif
#endif
			TriaShape->m_Flags = B3_PHONG;
		}
	}
	return size;
}

/* This routine processes a material chunk. It reads the ID, the parent ID */
/* and the material values for color, specular and ambient index and the */
/* transparency values. It then creates a Blizzard material and cobInfo */
/* structure. This routine does not process textures! */
/* ------------------------------------------------------------------------ */
/* buffer: buffer where the material chunk starts. */

b3_size b3COBReader::b3COB_ParseMat(const char * buffer)
{
	b3MatNormal * Mat;
	char         line[MAX_LINE];
	b3_count     ver, rev;
	b3_cob_id    id, parent;
	b3_f32       ambient = 0, specular = 0, alpha, r, g, b;
	b3_size      size, len, i;

	len = b3COB_GetLine(line, buffer, sizeof(line));
	sscanf(line, "Mat1 V%d.%d Id %d Parent %d Size %08zd",
		&ver, &rev, &id, &parent, &size);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "M: V%d.%02d ID: %8d P: %8d - size: %8zd,%8zd\n",
		ver, rev, id, parent, len, size);
#endif

	size += len;

	Mat = new b3MatNormal(NORMAL_MATERIAL);
	if (Mat != nullptr)
	{
		char         name[B3_FILESTRINGLEN];

		name[0] = 0;
		if (!b3COB_AllocObject(Mat, id, parent, COB_MAT1))
		{
			delete Mat;
			return size;
		}

		for (i = len + 1; i < size; i += (len + 1))
		{
			b3_f32 exp, ior;

			len = b3COB_GetLine(line, &buffer[i], sizeof(line));
			sscanf(line, "rgb %f , %f , %f", &r, &g, &b);
			sscanf(line, "alpha %f ka %f ks %f exp %f ior %f",
				&alpha,
				&ambient,
				&specular,
				&exp,
				&ior);
			Mat->m_SpecularExp = exp;
			Mat->m_Ior         = ior;
			sscanf(line, "texture: %64s", name);
			Mat->m_Diffuse.b3Init(r, g, b);
		}
		if ((Mat->m_Ior == 0) || (Mat->m_Ior == 1))
		{
			Mat->m_Refraction = 0;
		}
		else
		{
			Mat->m_Refraction = 0.5;
		}
		Mat->m_SpecularExp  *= 100000;
		if (Mat->m_SpecularExp <     20)
		{
			Mat->m_SpecularExp =     20;
		}
		if (Mat->m_SpecularExp > 100000)
		{
			Mat->m_SpecularExp = 100000;
		}
		if (strlen(name) > 0)
		{
			const b3Tx * texture = b3Scene::m_TexturePool.b3LoadTexture(name);

			if (texture != nullptr)
			{
				b3COB_ComputeAvrgColor(texture, Mat->m_Diffuse);
			}
		}
		Mat->m_Ambient  = Mat->m_Diffuse * ambient;
		Mat->m_Specular = Mat->m_Diffuse * specular;
	}
	return size;
}

/* This routine processes a chunk as dummy. */
/* ------------------------------------------- */
/* buffer: buffer where the unit chunk starts. */

b3_size b3COBReader::b3COB_ParseDummy(const char * buffer)
{
	char       line[MAX_LINE];
	char       command[8];
	b3_size    size, len;
	b3_count   ver, rev;
	b3_cob_id  id, parent;

	len = b3COB_GetLine(line, buffer, sizeof(line));
	sscanf(line, "%4s V%d.%d Id %d Parent %d Size %08zd",
		command, &ver, &rev, &id, &parent, &size);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "D: V%d.%02d ID: %8d P: %8d - size: %8zd,%8zd (%c%c%c%c)\n",
		ver, rev, id, parent, len, size,
		command[0],
		command[1],
		command[2],
		command[3]);
#endif

	size += len;
	return size;
}

/* This routine processes the COB file. It checkes wether the file is  */
/* a COB file, than processes each chunk until end of file is reached. */
/* ------------------------------------------------------------------- */
/* buffer: memory buffer containing COB file */
/* name:   filename of COB */
/* size:   size of buffer */

b3Item * b3COBReader::b3COB_Parse(
	const char * buffer,
	const char * name,
	b3_size     size)
{
	char        line[MAX_LINE];
	b3_size     len, pos;
	b3_cob_type token;

	len = b3COB_GetLine(line, buffer, sizeof(line));
	if (strcmp(line, "Caligari V00.01ALH             ") != 0)
	{
		b3PrintF(B3LOG_NORMAL, " This file is not an Caligari ASCII file!!\n");
		return nullptr;
	}

	buffer += (++len);
	pos = len;
	while (pos < size)
	{
		token = b3COB_GetToken(buffer);
		switch (token)
		{
		case COB_GROU :
			len =  b3COB_ParseGrou(buffer, name);
			break;

		case COB_POLH :
			len =  b3COB_ParsePolH(buffer, name);
			break;

		case COB_MAT1 :
			len = b3COB_ParseMat(buffer);
			break;

		case COB_UNIT :
		case COB_DEFP :
		case COB_CHAN :
		case COB_DDIV :
			len = b3COB_ParseDummy(buffer);
			break;

		default :
#ifdef _DEBUG
			b3PrintF(B3LOG_FULL, "%6ld - %6ld: %s\n", pos, size, token == COB_POLH ? "PolH" : "-");
#endif
			b3PrintF(B3LOG_NORMAL, " ERROR: unknown token (%c%c%c%c) found in COB file!\n",
				buffer[0],
				buffer[1],
				buffer[2],
				buffer[3]);
			B3_FALLTHROUGH;
		case COB_END  :
			pos = size;
			break;
		}
		pos    += len;
		buffer += len;
	}
	return b3COB_Reconstruct();
}

b3BBox * b3COBReader::b3ReadCOB(const char * cobfile)
{
	b3COBReader  reader;
	b3BBox   *   bbox = nullptr;
	b3File       file;
	const char * buffer;
	b3_size      size;

	b3PrintF(B3LOG_NORMAL, "Reading COB %s\n", cobfile);
	buffer = reinterpret_cast<const char *>(file.b3ReadBuffer(cobfile, size));
	if (buffer != nullptr)
	{
		bbox = static_cast<b3BBox *>(reader.b3COB_Parse(buffer, cobfile, size));
	}
	return bbox;
}
