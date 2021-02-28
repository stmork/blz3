/*
**
**	$filename:	b3Scene.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing root scene
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"
#include "b3ReadTGF.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3Scene::b3Register()
{
	b3PrintF(B3LOG_DEBUG, "Registering scene classes...\n");
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEANGLE_MORK);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_MORK);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_MORK2);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEANGLE_PHONG);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_PHONG);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, TRACEPHOTO_ALBRECHT);
	b3Item::b3Register(&b3Scene::b3StaticInit, &b3Scene::b3StaticInit, GLOBAL_ILLUM);
}

b3Scene::b3Scene(b3_u32 class_type) : b3Item(sizeof(b3Scene), class_type)
{
	b3PrintF(B3LOG_DEBUG, "Blizzard III scene init.\n");

	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BBOX);
	m_Heads[1].b3InitBase(CLASS_LIGHT);
	m_Heads[2].b3InitBase(CLASS_SPECIAL);

	m_TopColor.b3Init(0.5f, 0.0f, 1.0f);
	m_BottomColor.b3Init(0.0f, 0.0f, 1.0f);

	m_xAngle           = 0;
	m_yAngle           = 0;
	m_BBoxOverSize     = 0.02f;
	m_BackgroundType   = TP_NOTHING;
	m_TraceDepth       = 5;
	m_Flags            = TP_SIZEVALID;
	m_ShadowBrightness = 0.5f;
	m_Epsilon          = 0.001f;
	m_xSize            = 200;
	m_ySize            = 150;
	m_BackTexture      = nullptr;
	m_ActualCamera     = nullptr;
	m_Shader           = nullptr;
	m_Filename.b3Empty();

	b3ReallocateShader();
}

b3Scene::b3Scene(b3_u32 * buffer) : b3Item(buffer)
{
	b3PrintF(B3LOG_DEBUG, "Blizzard III scene load.\n");

	// Background color
	b3InitColor(m_TopColor);
	b3InitColor(m_BottomColor);

	// Camera
	b3InitVector(&m_EyePoint);
	b3InitVector(&m_ViewPoint);
	b3InitVector(&m_Width);
	b3InitVector(&m_Height);

	// Some other values
	b3InitNull();
	m_xAngle           = b3InitFloat();
	m_yAngle           = b3InitFloat();
	m_BBoxOverSize     = b3InitFloat();
	m_BackgroundType   = (b3_bg_type)b3InitInt();
	m_TraceDepth       = b3InitInt();
	m_Flags            = b3InitInt();
	m_ShadowBrightness = b3InitFloat();
	m_Epsilon          = b3InitFloat();
	m_xSize            = b3InitInt();
	m_ySize            = b3InitInt();
	b3InitString(m_TextureName, B3_TEXSTRINGLEN);
	m_BackTexture      = nullptr;
	m_ActualCamera     = nullptr;
	m_Nebular          = nullptr;
	m_LensFlare        = nullptr;
	m_SuperSample      = nullptr;
	m_Shader           = nullptr;
	m_Filename.b3Empty();

	b3ReallocateShader();
}

void b3Scene::b3Write()
{
	b3PrintF(B3LOG_DEBUG, "Blizzard III scene storage.\n");

	// Background color
	b3StoreColor(m_TopColor);
	b3StoreColor(m_BottomColor);

	// Camera
	b3StoreVector(&m_EyePoint);
	b3StoreVector(&m_ViewPoint);
	b3StoreVector(&m_Width);
	b3StoreVector(&m_Height);

	// Some other values
	b3StoreNull();
	b3StoreFloat(m_xAngle);
	b3StoreFloat(m_yAngle);
	b3StoreFloat(m_BBoxOverSize);
	b3StoreInt(m_BackgroundType);
	b3StoreCount(m_TraceDepth);
	b3StoreInt(m_Flags);
	b3StoreFloat(m_ShadowBrightness);
	b3StoreFloat(m_Epsilon);
	b3StoreRes(m_xSize);
	b3StoreRes(m_ySize);
	b3StoreString(m_TextureName, B3_TEXSTRINGLEN);
}

b3Scene::~b3Scene()
{
	if (m_Shader != nullptr)
	{
		delete m_Shader;
	}
}

void b3Scene::b3SetShading(b3_u32 class_type)
{
	ClassType = class_type;
	b3ReallocateShader();
}

void b3Scene::b3ReallocateShader()
{
	if (m_Shader != nullptr)
	{
		delete m_Shader;
	}
	switch (b3GetClassType())
	{
	case TRACEPHOTO_ALBRECHT:
	case GLOBAL_ILLUM:
		b3PrintF(B3LOG_NORMAL, "Warning: Scene type %08lx not implemented yet...\n", b3GetClassType());
		B3_FALLTHROUGH;
	case TRACEPHOTO_MORK:
	case TRACEANGLE_MORK:
		m_Shader = new b3ShaderMork(this);
		break;

	case TRACEPHOTO_PHONG:
	case TRACEANGLE_PHONG:
		m_Shader = new b3ShaderPhong(this);
		break;

	case TRACEPHOTO_MORK2:
		m_Shader = new b3ShaderMork2(this);
		break;

	default:
		m_Shader = nullptr;
		break;
	}
	b3PrintF(B3LOG_DEBUG, "Using shader type %08x with instance %p\n", b3GetClassType(), m_Shader);
}

b3_bool b3Scene::b3GetDisplaySize(b3_res & xSize, b3_res & ySize)
{
	xSize = this->m_xSize;
	ySize = this->m_ySize;
	return (m_Flags & TP_SIZEVALID) != 0;
}

/*************************************************************************
**                                                                      **
**                        Filename handling                             **
**                                                                      **
*************************************************************************/

char * b3Scene::b3GetName()
{
	m_Filename.b3SplitFileName(nullptr, m_SceneName);
	m_SceneName.b3RemoveExt();
	return m_SceneName;
}

char * b3Scene::b3GetFilename()
{
	return m_Filename;
}

void b3Scene::b3SetFilename(const char * filename)
{
	m_Filename.b3Format("%s", filename);
}

void b3Scene::b3SetTexture(const char * name)
{
	b3Item::b3SetString(m_TextureName, sizeof(m_TextureName), name);
}

/*************************************************************************
**                                                                      **
**                        Special handling                              **
**                                                                      **
*************************************************************************/

b3Animation * b3Scene::b3GetAnimation(b3_bool force)
{
	b3Animation * animation;
	b3Item   *   item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == ANIMATION)
		{
			return (b3Animation *)item;
		}
	}

	if (force)
	{
		animation = new b3Animation(ANIMATION);
		b3GetSpecialHead()->b3Append(animation);
	}
	else
	{
		animation = nullptr;
	}
	return animation;
}

b3_f64 b3Scene::b3GetTimePoint()
{
	b3Animation * anim = b3GetAnimation();

	return anim != nullptr ? anim->m_Time : 0;
}

b3ModellerInfo * b3Scene::b3GetModellerInfo()
{
	b3ModellerInfo * info;
	b3Item     *    item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == LINES_INFO)
		{
			return (b3ModellerInfo *)item;
		}
	}

	info = new b3ModellerInfo(LINES_INFO);
	b3GetSpecialHead()->b3Append(info);
	return info;
}

b3Distribute * b3Scene::b3GetDistributed(b3_bool force)
{
	b3Distribute * distributed;
	b3Item    *   item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == DISTRIBUTE)
		{
			return (b3Distribute *)item;
		}
	}

	if (force)
	{
		distributed = new b3Distribute(DISTRIBUTE);
		b3GetSpecialHead()->b3Append(distributed);
	}
	else
	{
		distributed = nullptr;
	}
	return distributed;
}

b3Nebular * b3Scene::b3GetNebular(b3_bool force)
{
	b3Nebular * nebular;
	b3Item  *  item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == NEBULAR)
		{
			return (b3Nebular *)item;
		}
	}

	if (force)
	{
		nebular = new b3Nebular(NEBULAR);
		b3GetSpecialHead()->b3Append(nebular);
	}
	else
	{
		nebular = nullptr;
	}
	return nebular;
}

b3SuperSample * b3Scene::b3GetSuperSample(b3_bool force)
{
	b3SuperSample * supersample;
	b3Item    *    item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == SUPERSAMPLE4)
		{
			return (b3SuperSample *)item;
		}
	}

	if (force)
	{
		supersample = new b3SuperSample(SUPERSAMPLE4);
		b3GetSpecialHead()->b3Append(supersample);
	}
	else
	{
		supersample = nullptr;
	}
	return supersample;
}

b3LensFlare * b3Scene::b3GetLensFlare(b3_bool force)
{
	b3LensFlare * lensflare;
	b3Item   *   item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == LENSFLARE)
		{
			return (b3LensFlare *)item;
		}
	}

	if (force)
	{
		lensflare = new b3LensFlare(LENSFLARE);
		b3GetSpecialHead()->b3Append(lensflare);
	}
	else
	{
		lensflare = nullptr;
	}

	return lensflare;
}

b3CloudBackground * b3Scene::b3GetCloudBackground(b3_bool force)
{
	b3CloudBackground * clouds;
	b3Item      *      item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == CLOUDS)
		{
			return (b3CloudBackground *)item;
		}
	}

	if (force)
	{
		clouds = new b3CloudBackground(CLOUDS);
		b3GetSpecialHead()->b3Append(clouds);
	}
	else
	{
		clouds = nullptr;
	}

	return clouds;
}

/*************************************************************************
**                                                                      **
**                        Camera handling                               **
**                                                                      **
*************************************************************************/

b3CameraPart * b3Scene::b3GetFirstCamera(b3_bool must_active)
{
	b3CameraPart * camera, *first = nullptr;
	b3Item    *    item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (first == nullptr)
			{
				first = camera;
			}
			if ((!must_active) || (camera->m_Flags & CAMERA_ACTIVE))
			{
				return camera;
			}
		}
	}

	if (first == nullptr)
	{
		camera = new b3CameraPart(CAMERA);
		camera->m_EyePoint  = m_EyePoint;
		camera->m_ViewPoint = m_ViewPoint;
		camera->m_Width     = m_Width;
		camera->m_Height    = m_Height;
		camera->m_Flags     = CAMERA_ACTIVE;
		b3GetTitle(camera->m_CameraName, sizeof(camera->m_CameraName));
		b3GetSpecialHead()->b3Append(camera);
	}
	else
	{
		camera = first;
	}

	return camera;
}


b3CameraPart * b3Scene::b3GetCameraByName(const char * camera_name)
{
	b3CameraPart * camera;
	b3Item    *   item;

	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (stricmp(camera->b3GetName(), camera_name) == 0)
			{
				return camera;
			}
		}
	}
	return nullptr;
}

b3CameraPart * b3Scene::b3GetActualCamera()
{
	if (m_ActualCamera == nullptr)
	{
		b3SetCamera(b3GetFirstCamera(false));
	}

	return m_ActualCamera;
}

b3CameraPart * b3Scene::b3GetNextCamera(b3CameraPart * camera)
{
	while ((camera = (b3CameraPart *)camera->Succ) != nullptr)
	{
		if (camera->b3GetClassType() == CAMERA)
		{
			return camera;
		}
	}
	return nullptr;
}

b3CameraPart * b3Scene::b3UpdateCamera()
{
	if (m_ActualCamera != nullptr)
	{
		b3PrintF(B3LOG_DEBUG, "Using camera %s\n", m_ActualCamera->b3GetName());
		m_EyePoint 	= m_ActualCamera->m_EyePoint;
		m_ViewPoint = m_ActualCamera->m_ViewPoint;
		m_Width     = m_ActualCamera->m_Width;
		m_Height    = m_ActualCamera->m_Height;
	}
	return m_ActualCamera;
}

void b3Scene::b3SetCamera(b3CameraPart * camera, b3_bool reorder)
{
	m_ActualCamera = camera;
	b3UpdateCamera();
	if (camera != nullptr)
	{
		if (reorder)
		{
			b3GetSpecialHead()->b3Remove(camera);
			b3GetSpecialHead()->b3First(camera);
		}

		if (strlen(camera->m_CameraName) == 0)
		{
			b3Dir::b3SplitFileName(m_Filename, nullptr, camera->m_CameraName);
		}
	}
}

b3_bool b3Scene::b3GetTitle(char * title, size_t size)
{
	title[0] = 0;
	if (m_ActualCamera != nullptr)
	{
		if (strlen(m_ActualCamera->m_CameraName) > 0)
		{
			strlcpy(title, m_ActualCamera->m_CameraName, size);
		}
	}
	else
	{
		b3Dir::b3SplitFileName(m_Filename, nullptr, title);
	}
	return strlen(title) > 0;
}

/*************************************************************************
**                                                                      **
**                        Light handling                                **
**                                                                      **
*************************************************************************/

b3Light * b3Scene::b3GetLightByName(const char * light_name)
{
	b3Light * light;
	b3Item * item;

	B3_FOR_BASE(b3GetLightHead(), item)
	{
		light = (b3Light *)item;
		if (stricmp(light->b3GetName(), light_name) == 0)
		{
			return light;
		}
	}
	return nullptr;
}

b3_count b3Scene::b3GetLightCount()
{
	return m_LightCount;
}

b3Light * b3Scene::b3GetLight(b3_bool must_active)
{
	b3Light * light;
	b3Item * item;

	B3_FOR_BASE(b3GetLightHead(), item)
	{
		light = (b3Light *)item;
		if ((!must_active) || ((light->m_Flags & LIGHT_OFF) == 0))
		{
			return light;
		}
	}

	if ((light = (b3Light *)b3GetLightHead()->First) == nullptr)
	{
		light = new b3Light(SPOT_LIGHT);
		strlcpy(light->m_Name, "Light", sizeof(light->m_Name));
		b3GetLightHead()->b3Append(light);
	}

	return light;
}

/*************************************************************************
**                                                                      **
**                        Object handling                               **
**                                                                      **
*************************************************************************/

b3Scene * b3Scene::b3ReadTGF(const char * filename)
{
	return b3TGFReader::b3ReadTGFScene(filename);
}

/*************************************************************************
**                                                                      **
**                        shading                                       **
**                                                                      **
*************************************************************************/

const b3_f64 b3Scene::m_Distances[LENSFLARE_LOOP] =
{
	0.55, 0.0, 0.0, 0.25, 0.45, 0.55
};

const b3_f64 b3Scene::m_ResultWeights[LENSFLARE_LOOP] =
{
	0.9, 0.95, 0.95, 0.6, 0.6, 0.6
};

const b3_f64 b3Scene::m_Exponents[LENSFLARE_LOOP] =
{
	2.4, 1.5, 1.5, 2.0, 7.0, 4.0
};

void b3Scene::b3MixLensFlare(b3_ray * ray)
{
	b3Item  *  item;
	b3Light  * light;
	b3_vector  central, toLight, nLight;
	b3Color    result;
	b3_f64     distance, weight = 0.6;
	b3_count   i;

	B3_FOR_BASE(b3GetLightHead(), item)
	{
		light = (b3Light *)item;

		toLight.x = light->m_Position.x - m_ViewPoint.x;
		toLight.y = light->m_Position.y - m_ViewPoint.y;
		toLight.z = light->m_Position.z - m_ViewPoint.z;
		distance  = b3Vector::b3Length(&toLight) / m_ViewAxisLen;
		central.x = distance * m_ViewAxis.x;
		central.y = distance * m_ViewAxis.y;
		central.z = distance * m_ViewAxis.z;

		b3Vector::b3CrossProduct(&m_ViewAxis, &toLight, &nLight);

		for (i = 0; i < LENSFLARE_LOOP; i++)
		{
			b3_f64    cWeight, lWeight, angle, reverse, beta;
#ifdef TX_DISTURB
			b3_vector diff;
#endif

			cWeight  = m_Distances[i];
			lWeight  = 1.0 - cWeight;
			nLight.x = cWeight * central.x + lWeight * toLight.x;
			nLight.y = cWeight * central.y + lWeight * toLight.y;
			nLight.z = cWeight * central.z + lWeight * toLight.z;

			beta     = (
					nLight.x * ray->dir.x +
					nLight.y * ray->dir.y +
					nLight.z * ray->dir.z) / b3Vector::b3Length(&nLight);
			angle    = pow(beta, m_LensFlare->m_Expon * m_Exponents[i]);
			if (i < LENSFLARE_RING)
			{
				if ((angle > 0.5) && (angle < 0.55))
				{
					angle = 0.3;
				}
				else
				{
					angle = 0;
				}
			}
			else
			{
				if (angle > 0.9)
				{
					angle  = 0.9;
				}
				else
				{
					if (angle > 0.89)
					{
						angle = 0.95;
					}
					else
					{
						angle *= m_ResultWeights[i];
					}
				}
				angle *= weight;
			}

			reverse  = 1.0 - angle;
			if (i < LENSFLARE_RING)
			{
				result.b3Init(
					ray->color[b3Color::R] * reverse + angle * light->m_Color[b3Color::R] * m_LensFlare->m_Color[b3Color::R],
					ray->color[b3Color::G],
					ray->color[b3Color::B]);
			}
			else
			{
				result = light->m_Color * m_LensFlare->m_Color * angle + ray->color * reverse;
			}
			ray->color = result;
		}
	}
}

void b3Scene::b3GetBackgroundColor(
	b3_ray * ray,
	b3_f64  lx,
	b3_f64  ly)
{
	b3_coord  x, y;
	b3_f64    r, sight;

	switch (m_BackgroundType)
	{
	case TP_TEXTURE :
		x = (b3_coord)(((lx + 1) * 0.5 * (m_BackTexture->xSize - 1)));
		y = (b3_coord)(((1 - ly) * 0.5 * (m_BackTexture->ySize - 1)));
		if (x < 0)
		{
			x = 0;
		}
		if (x >= m_BackTexture->xSize)
		{
			x = m_BackTexture->xSize - 1;
		}
		if (y < 0)
		{
			y = 0;
		}
		if (y >= m_BackTexture->ySize)
		{
			y = m_BackTexture->ySize - 1;
		}

		ray->color = m_BackTexture->b3GetHdrValue(x, y);
		break;

	case TP_SKY_N_HELL :
		sight      = m_Clouds->b3ComputeClouds(ray, r, b3GetTimePoint());
		ray->color = b3Color::b3Mix(m_BottomColor, b3Color(
					B3_MAX(r, m_TopColor[b3Color::R]),
					B3_MAX(r, m_TopColor[b3Color::G]),
					B3_MAX(r, m_TopColor[b3Color::B])), sight);
		break;

	case TP_SLIDE :
		ray->color = m_AvrgColor + m_DiffColor * ly;
		break;

	default:
		ray->color.b3Init(m_ShadowBrightness, m_ShadowBrightness, m_ShadowBrightness);
		break;
	}
}

void b3Scene::b3GetInfiniteColor(b3_ray * ray)
{
	b3_f64 ly, lx;

	ly =
		ray->dir.x * m_NormHeight.x +
		ray->dir.y * m_NormHeight.y +
		ray->dir.z * m_NormHeight.z;
	lx =
		ray->dir.x * m_NormWidth.x +
		ray->dir.y * m_NormWidth.y +
		ray->dir.z * m_NormWidth.z;

	b3GetBackgroundColor(ray, lx, ly);
}
