/*
**
**      $Filename:      b3ReadCOB.cc $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Converting Caligari objects into Blizzard III
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3ReadCOB.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2003/01/26 11:53:26  sm
**	- Added support for Caligari object loading.
**
*/

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
	b3Item      *node,
	b3_cob_id    id,
	b3_cob_id    parent,
	b3_cob_type  type)
{
	b3COBInfo *cobInfo;

	cobInfo = new b3COBInfo(node,id,parent,type);
	if (cobInfo)
	{
		cobInfos.b3Append(cobInfo);
	}
	return cobInfo != null;
}

/* This routine tries to find an cobInfo in the cobDone list identified by */
/* an ID. It returns the pointer to the found info. */
/* ----------------------------------------------------------------------- */
/* id: ID to search for. */

b3COBInfo *b3COBReader::b3COB_FindInfo(b3_cob_id id)
{
	b3COBInfo *cobInfo;

	B3_FOR_BASE(&cobDone,cobInfo)
	{
		if (cobInfo->refID == id)
		{
			return cobInfo;
		}
	}
	return null;
}

/* This routine reconstructs the hierarchy of Caligari objects to the */
/* Blizzard data structure. This will be done in two phases: */
/* 1. find root object, remove it from object list, append it to done list. */
/* 2. take first object from object list, find parent ID in done list. */
/*    create linkuage in Blizzard data structure, remove object from */
/*    object list, append it to done list. */
/* 3. repeat until object list is empty. */
/* The routine returns the Blizzard object of the root object. */

b3Item *b3COBReader::b3COB_Reconstruct()
{
	b3COBInfo       *cobInfo,*next;
	b3BBox          *root = null,*BBox;
	b3TriangleShape *Shape;


	/* First find root object in object list. */
	/* Follow error messages for exceptions! */
	for (cobInfo  = cobInfos.First;
	     cobInfo != null;
	     cobInfo  = next)
	{
		next = cobInfo->Succ;
		if (cobInfo->refParent == 0)
		{
			if (cobInfo->refType != COB_GROU)
			{
				b3PrintF (B3LOG_NORMAL," ERROR: root object not of required group type!\n");
				return null;
			}

			if (root != null)
			{
				b3PrintF(B3LOG_NORMAL," ERROR: more than one root object found!\n");
				return null;
			}

			root = (b3BBox *)cobInfo->refNode;

			cobInfos.b3Remove (cobInfo);
			cobDone.b3Append (cobInfo);
		}
	}

	if (root == null)
	{
		b3PrintF (B3LOG_NORMAL," ERROR: no root object found!\n");
		return null;
	}


	/* Second process object list for linkuage to objects in */
	/* done list. */
	/* ----------------------------------------------------- */
	/* NOTE: only following data links are possible: */
	/* groups    to groups */
	/* polygons  to groups */
	/* materials to polygons */

	while ((cobInfo = cobInfos.First) != null)
	{
		next = b3COB_FindInfo(cobInfo->refParent);
		if (next == null)
		{
			b3PrintF (B3LOG_NORMAL," ERROR: object with parent ID %ld not found!\n",
				cobInfo->refParent);
			return null;
		}

		switch (cobInfo->refType)
		{
			case COB_GROU :
				switch (next->refType)
				{
					case COB_GROU :
#ifndef ONE_BBOX
						BBox = (b3BBox *)next->refNode;
						BBox->b3GetBBoxHead()->b3Append (cobInfo->refNode);
#endif
						break;

					default :
						b3PrintF (B3LOG_NORMAL," ERROR: Cannot insert BBox into this kind of object!\n");
						return null;
				}
				break;

			case COB_POLH :
				switch (next->refType)
				{
					case COB_GROU :
#ifndef ONE_BBOX
						BBox = (b3BBox *)next->refNode;
#else
						BBox = root;
#endif
						BBox->b3GetShapeHead()->b3Append (cobInfo->refNode);
						break;

					default :
						b3PrintF (B3LOG_NORMAL," ERROR: Cannot insert polygons into this kind of object!\n");
						return null;
				}
				break;

			case COB_MAT1 :
				switch (next->refType)
				{
					case COB_POLH :
						Shape = (b3TriangleShape *)next->refNode;
						Shape->b3GetMaterialHead()->b3Append (cobInfo->refNode);
						break;

					default :
						b3PrintF (B3LOG_NORMAL," ERROR: Cannot insert material into this kind of object!\n");
						return null;
				}
				break;

			// Do nothing!
			default:
				break;
		}

		/* put object from object list to done list */
		cobInfos.b3Remove (cobInfo);
		cobDone.b3Append  (cobInfo);
	}

//	RecountBBox (root,1L);
	return root;
}

void b3COBReader::b3COB_ComputeAvrgColor (
	b3Tx     *texture,
	b3_color *Color)
{
	b3_pkd_color value;
	b3_coord     x,y,xMax,yMax;

	Color->r =
	Color->g =
	Color->b = 0;
	xMax = texture->xSize;
	yMax = texture->ySize;
	for (y = 0;y < yMax;y++)
	{
		for (x = 0;x < xMax;x++)
		{
			value = texture->b3GetValue(x,y);
			b3Color::b3GetColor(Color,value);
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

b3_size b3COBReader::b3COB_GetLine (
	      char *line,
	const char *buffer,
	b3_size     maxLine)

{
	b3_size i;

	for (i = 0;i < maxLine;i++)
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

b3_cob_type b3COBReader::b3COB_GetToken (const char *buffer)
{
	if (strncmp(buffer,"Grou",4) == 0) return COB_GROU;
	if (strncmp(buffer,"PolH",4) == 0) return COB_POLH;
	if (strncmp(buffer,"Mat1",4) == 0) return COB_MAT1;
	if (strncmp(buffer,"END ",4) == 0) return COB_END;
	if (strncmp(buffer,"Unit",4) == 0) return COB_UNIT;
	if (strncmp(buffer,"DefP",4) == 0) return COB_DEFP;
	if (strncmp(buffer,"Chan",4) == 0) return COB_CHAN;
	if (strncmp(buffer,"DDiv",4) == 0) return COB_DDIV;

	return COB_UNKNOWN;
}

/* This routine processes a group chunk. It reads the ID, the parent ID and */
/* the group name to store it for the BBox name. It then creates a Blizzard */
/* bounding box and cobInfo structure. */
/* ------------------------------------------------------------------------ */
/* buffer: buffer where the group chunk starts. */

b3_size b3COBReader::b3COB_ParseGrou(
	const char *buffer,
	const char *name)
{
	b3BBox    *BBox;
	char       line[MAX_LINE];
	char       boxName[B3_BOXSTRINGLEN];
	b3_size    len,size;
	b3_cob_id  id,parent;
	b3_count   ver,rev;

	len = b3COB_GetLine (line,buffer,sizeof(line));
	sscanf (line,"Grou V%ld.%ld Id %d Parent %d Size %08d",
		&ver,&rev,&id,&parent,&size);

	b3COB_GetLine (line,&buffer[len+1],sizeof(line));
	if (sscanf (line,"Name %32s",boxName) != 1) strcpy (boxName,"bbox");

#ifdef DEBUG
	b3PrintF (B3LOG_FULL,"G: V%ld.%02ld ID: %8ld P: %8ld - size: %8ld,%8ld\n",
		ver,rev,id,parent,len,size);
#endif

	BBox = new b3BBox (BBOX);
	if (BBox != null)
	{
		strcpy(BBox->m_BoxName,boxName);
		if (!b3COB_AllocObject(BBox,id,parent,COB_GROU))
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
	const char *buffer,
	const char *name)
{
	b3TriangleShape *TriaShape;
	b3BBox          *BBox;
	b3_vertex       *vert;
	b3_triangle     *tria;
	b3_vector        pos;
	b3_matrix        transform;
	char             line[MAX_LINE];
	b3_size          len,size,i,index;
	b3_index         id1,id2,vPos = 0,fPos = 0;
	b3_count         ver,rev;
	b3_cob_id        id,parent;
	b3_count         count,vertices=0,faces=0,polygons=0,k;
#ifdef DEBUG
	b3_count         dbgCount = 0;
#endif

	len = b3COB_GetLine (line,buffer,sizeof(line));
	sscanf (line,"PolH V%ld.%ld Id %d Parent %d Size %08d",
		&ver,&rev,&id,&parent,&size);

	if (parent == 0)
	{
		BBox = new b3BBox(BBOX);
		if (BBox != null)
		{
			strcpy(BBox->m_BoxName,name);
			if (!b3COB_AllocObject(BBox,1,parent,COB_GROU))
			{
				delete BBox;
			}
#ifdef DEBUG
			else
			{
				b3PrintF (B3LOG_FULL,"R: V%ld.%02ld ID: %8ld P: %8ld - size: %8ld,%8ld\n",
					0,0,1,0,0,0);
			}
#endif
		}
		parent = 1;
	}

#ifdef DEBUG
	b3PrintF (B3LOG_FULL,"P: V%ld.%02ld ID: %8ld P: %8ld - size: %8ld,%8ld\n",
		ver,rev,id,parent,len,size);
#endif

	size += len;
	b3MatrixUnit (&transform);
	for (i = len + 1;i < size;i += (len+1))
	{
		len = b3COB_GetLine (line,&buffer[i],sizeof(line));

		/* read number of polygons, number of triangles */
		if (sscanf(line,"Faces %ld",&polygons) == 1)
		{
			fPos  = i + len + 1;
			faces = 0;
			for (ver = 0;ver < polygons;ver++)
			{
				i += (len+1);
				len = b3COB_GetLine (line,&buffer[i],sizeof(line));
				rev = 2;
				if (sscanf (line,"Face verts %ld",&rev) == 1)
				{
					faces += (rev - 2);
				}

				i += (len+1);
				len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			}
		}

		/* read number of vertices */
		if (sscanf(line,"World Vertices %ld",&vertices) == 1)
		{
			vPos  = i + len + 1;
		}

		/* read transformation matrix */
		if (strcmp(line,"Transform") == 0)
		{
			i += (len+1);
			len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			sscanf (line,"%f %f %f %f",
				&transform.m11,&transform.m12,&transform.m13,&transform.m14);

			i += (len+1);
			len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			sscanf (line,"%f %f %f %f",
				&transform.m21,&transform.m22,&transform.m23,&transform.m24);

			i += (len+1);
			len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			sscanf (line,"%f %f %f %f",
				&transform.m31,&transform.m32,&transform.m33,&transform.m34);

			i += (len+1);
			len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			sscanf (line,"%f %f %f %f",
				&transform.m41,&transform.m42,&transform.m43,&transform.m44);
		}
	}
#ifdef DEBUG
	b3PrintF (B3LOG_FULL,"%ld vertices, %ld polygons, %ld triangles\n",vertices,polygons,faces);
#endif

	TriaShape = new b3TriangleShape(TRIANGLES);
	if (TriaShape != null)
	{
		TriaShape->b3Init(vertices,faces,0,0);
		if (!b3COB_AllocObject(TriaShape,id,parent,COB_POLH))
		{
			delete TriaShape;
		}
		else
		{
			vert = (b3_vertex *)TriaShape->m_Vertices;
			tria = (b3_triangle *)TriaShape->m_Triangles;

			/* read vertices */
			i = vPos;
			for (count = 0;count < vertices;count++)
			{
				len = b3COB_GetLine (line,&buffer[i],sizeof(line));
				sscanf (line,"%f %f %f",&pos.x,&pos.y,&pos.z);
				b3MatrixVMul (&transform,&pos,(b3_vector *)&vert[count],true);
/*
#ifdef DEBUG
				b3PrintF (B3LOG_FULL,"% 3.3f % 3.3f % 3.3f\n",
					vert[count].x,vert[count].y,vert[count].z);
#endif
*/
				i += (len+1);
			}

			/* loop for reading triangle indices */
			i = fPos; /* outer polygon loop */
			for (count = 0;count < polygons;count++)
			{
				long *IDs;

				len = b3COB_GetLine (line,&buffer[i],sizeof(line));
				rev = 0;
				if (sscanf (line,"Face verts %ld",&rev) == 1)
				{
					rev -= 2;
				}
				i += (len+1);

#ifndef SINGLE_CYCLE
				IDs = (long *)malloc(sizeof(long *) * (size_t)(rev + 2));
#else
				IDs = null;
#endif
				len = b3COB_GetLine (line,&buffer[i],sizeof(line));
				i += (len+1);
				if (IDs)
				{
					long u,l;

					/* first, scan indices */
					rev += 2;
					for (k = 0,len = 0;k < rev;k++)
					{
						sscanf (&line[len],"<%ld,%*d> %n",&IDs[k],&index);
						len += index;
					}

					l = 1;
					u = rev;
					for (k = 2;k < rev;k++)
					{
						if (k & 1)
						{
#ifdef VERBOSE_INDEX
							b3PrintF (B3LOG_FULL,"%3ld - %3ld - %3ld #",u % rev,l,(u - 1) % rev);
#endif
							tria->P1 = IDs[u % rev];
							tria->P2 = IDs[l];
							u--;
							tria->P3 = IDs[u % rev];
						}
						else
						{
#ifdef VERBOSE_INDEX
							b3PrintF (B3LOG_FULL,"%3ld - %3ld - %3ld #",u % rev,l,l+1);
#endif
							tria->P1 = IDs[u % rev];
							tria->P2 = IDs[l];
							l++;
							tria->P3 = IDs[l];
						}
#ifdef VERBOSE_INDEX
						b3PrintF (B3LOG_FULL,"%3ld - %3ld - %3ld\n",tria->P1,tria->P2,tria->P3);
#endif
						tria++;
#ifdef DEBUG
						dbgCount++;
#endif
					}
					free (IDs);
				}
				else
				{
					sscanf (line,"<%ld,%*d> <%ld,%*d> %n",&id1,&id2,&index);

					len = index;
					for (k = 0;k < rev;k++) /* inner triangulation loop */
					{
						tria->P1 = id1;
						tria->P2 = id2;

						sscanf (&line[len],"<%ld,%*d> %n",&tria->P3,&index);
						id2  = tria->P3;
						len += index;

#ifdef VERBOSE_INDEX
						b3PrintF (B3LOG_FULL,"%6ld %6ld %6ld\n",tria->P1,tria->P2,tria->P3);
#endif
						tria++;
#ifdef DEBUG
						dbgCount++;
#endif
					}
				}
#ifdef VERBOSE_INDEX
				b3PrintF (B3LOG_FULL,"---\n");
#endif
			}
#ifdef DEBUG
#ifdef VERBOSE_INDEX
			b3PrintF (B3LOG_FULL,"%ld triangles expected, %ld triangles counted.\n",faces,dbgCount);
#endif
#endif
			TriaShape->m_Flags     = PHONG;
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

b3_size b3COBReader::b3COB_ParseMat(const char *buffer)
{
	b3MatNormal *Mat;
	char         line[MAX_LINE];
	char         name[B3_FILESTRINGLEN];
	b3_size      len,size,i;
	b3_count     ver,rev;
	b3_cob_id    id,parent;
	b3_f32       ambient = 0,specular = 0;

	len = b3COB_GetLine (line,buffer,sizeof(line));
	sscanf (line,"Mat1 V%ld.%ld Id %d Parent %d Size %08d",
		&ver,&rev,&id,&parent,&size);

#ifdef DEBUG
	b3PrintF (B3LOG_FULL,"M: V%ld.%02ld ID: %8ld P: %8ld - size: %8ld,%8ld\n",
		ver,rev,id,parent,len,size);
#endif

	size += len;

	Mat = new b3MatNormal(MATERIAL);
	if (Mat != null)
	{
		name[0] = 0;
		if (!b3COB_AllocObject(Mat,id,parent,COB_MAT1))
		{
			delete Mat;
			return size;
		}

		for (i = len+1;i < size;i += (len+1))
		{
			len = b3COB_GetLine (line,&buffer[i],sizeof(line));
			sscanf(line,"rgb %f , %f , %f",
				&Mat->m_DiffColor.r,
				&Mat->m_DiffColor.g,
				&Mat->m_DiffColor.b);
			sscanf(line,"alpha %f ka %f ks %f exp %f ior %f",
				&Mat->m_DiffColor.a,
				&ambient,
				&specular,
				&Mat->m_HighLight,
				&Mat->m_RefrValue);
			sscanf(line,"texture: %s",name);
		}
		if ((Mat->m_RefrValue == 0) || (Mat->m_RefrValue == 1))
		{
			Mat->m_Refraction = 0;
		}
		else
		{
			Mat->m_Refraction = 0.5;
		}
		Mat->m_HighLight  *= 100000;
		if (Mat->m_HighLight <     20) Mat->m_HighLight =     20;
		if (Mat->m_HighLight < 100000) Mat->m_HighLight = 100000;
		if (strlen(name) > 0)
		{
			b3Tx *texture;

			texture = texture_pool.b3LoadTexture(name);
			if (texture != null)
			{
				b3COB_ComputeAvrgColor (texture,&Mat->m_DiffColor);
			}
		}
		Mat->m_AmbColor.r  = ambient  * Mat->m_DiffColor.r;
		Mat->m_AmbColor.g  = ambient  * Mat->m_DiffColor.g;
		Mat->m_AmbColor.b  = ambient  * Mat->m_DiffColor.b;
		Mat->m_SpecColor.r = specular * Mat->m_DiffColor.r;
		Mat->m_SpecColor.g = specular * Mat->m_DiffColor.g;
		Mat->m_SpecColor.b = specular * Mat->m_DiffColor.b;
	}
	return size;
}

/* This routine processes a chunk as dummy. */
/* ------------------------------------------- */
/* buffer: buffer where the unit chunk starts. */

b3_size b3COBReader::b3COB_ParseDummy(const char *buffer)
{
	char       line[MAX_LINE];
	char       command[8];
	b3_size    len,size;
	b3_count   ver,rev;
	b3_cob_id  id,parent;

	len = b3COB_GetLine (line,buffer,sizeof(line));
	sscanf (line,"%4s V%ld.%ld Id %d Parent %d Size %08d",
		command,&ver,&rev,&id,&parent,&size);

#ifdef DEBUG
	b3PrintF (B3LOG_FULL,"D: V%ld.%02ld ID: %8ld P: %8ld - size: %8ld,%8ld (%c%c%c%c)\n",
		ver,rev,id,parent,len,size,
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

b3Item *b3COBReader::b3COB_Parse(
	const char *buffer,
	const char *name,
	b3_size     size)
{
	char        line[MAX_LINE];
	b3_size     len,pos;
	b3_cob_type token;

	len = b3COB_GetLine (line,buffer,sizeof(line));
	if (strcmp (line,"Caligari V00.01ALH             ") != 0)
	{
		b3PrintF (B3LOG_NORMAL," This file is not an Caligari ASCII file!!\n");
		return null;
	}

	buffer += (++len);
	pos = len;
	while (pos < size)
	{
		token = b3COB_GetToken (buffer);
		switch (token)
		{
		case COB_GROU :
			len =  b3COB_ParseGrou(buffer,name);
			break;

		case COB_POLH :
			len =  b3COB_ParsePolH(buffer,name);
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
#ifdef DEBUG
			B3_BEEP;
			b3PrintF (B3LOG_FULL,"%6ld - %6ld: %s\n",pos,size,token == COB_POLH ? "PolH" : "-");
#endif
			b3PrintF (B3LOG_NORMAL," ERROR: unknown token (%c%c%c%c) found in COB file!\n",
				buffer[0],
				buffer[1],
				buffer[2],
				buffer[3]);
		case COB_END  :
			pos = size;
			break;
		}
		pos    += len;
		buffer += len;
	}
	return b3COB_Reconstruct();
}

b3BBox *b3COBReader::b3ReadCOB(const char *cobfile)
{
	b3COBReader  reader;
	b3BBox      *bbox = null;
	b3File       file;
	const char  *buffer;
	b3_size      size;

	b3PrintF(B3LOG_NORMAL,"Reading COB %s\n",cobfile);
	buffer = (const char *)file.b3ReadBuffer(cobfile,size);
	if(buffer != null)
	{
		bbox = (b3BBox *)reader.b3COB_Parse(buffer,cobfile,size);
	}
	return bbox;
}
