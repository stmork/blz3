/*
**
**	$Filename:	brt3.cc $
**	$Release:	Dortmund 2001 - 2021 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#define no_DEBUG_VIEW

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3DisplayView.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Plugin.h"
#include "blz3/image/b3TxPool.h"
#include "blz3/image/b3Encoder.h"

#include "blz3/raytrace/b3RenderLight.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static char BLZ3_EXTENSION[16] = ".jpg";

static b3_bool b3SaveRaytracedImage(
	b3Display  * display,
	const char * picture_home,
	const char * camera_name)
{
	b3Path imagename;
	b3Path filename;

	filename.b3RemoveExt(camera_name);
	if (picture_home != nullptr)
	{
		imagename.b3LinkFileName(picture_home, filename);
		imagename.b3Append(BLZ3_EXTENSION);
	}
	else
	{
		imagename.b3Format("%s%s", (const char *)filename, BLZ3_EXTENSION);
	}
	return display->b3SaveImage(imagename);
}

static b3Display * b3AllocDisplay(
	b3Scene * scene,
	b3_bool   force_no_display,
	b3_res    size)
{
	b3Display * display;
	b3_res      xSize, ySize;

#ifdef DEBUG_VIEW
	// Show a small display in every case
	display = new b3DisplayView(200, 150);
#else
	if (size > 0)
	{
		if (scene->b3GetDisplaySize(xSize, ySize))
		{
			if (xSize > ySize)
			{
				ySize = size * ySize / xSize;
				xSize = size;
			}
			else
			{
				xSize = size * xSize / ySize;
				ySize = size;
			}
		}
		else
		{
			xSize = size;
			ySize = size * 3 / 4;
		}

		// Ensure size is dividsible by two!
		xSize &= 0xfffe;
		ySize &= 0xfffe;

		// And now prepare display.
		if ((scene->m_Flags & TP_NO_GFX) || force_no_display)
		{
			display = new b3Display(xSize, ySize);
		}
		else
		{
			display = new b3DisplayView(xSize, ySize);
		}
	}
	else if (scene->b3GetDisplaySize(xSize, ySize))
	{
		if ((scene->m_Flags & TP_NO_GFX) || force_no_display)
		{
			display = new b3Display(xSize, ySize);
		}
		else
		{
			display = new b3DisplayView(xSize, ySize);
		}
	}
	else
	{
		if ((scene->m_Flags & TP_NO_GFX) || force_no_display)
		{
			display = new b3Display();
		}
		else
		{
			display = new b3DisplayView();
		}
	}
#endif
	return display;
}

static void b3Banner(const char * command)
{
	b3PrintF(B3LOG_NORMAL, "Blizzard III Raytracer\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2021\n");
	b3PrintF(B3LOG_NORMAL, "\n");
	if (command != nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s [-d][-f][-a][-n][-w][-s size][-g][-i][-j][-r][-p]%s {Blizzard World Data files}\n",
			command,
#ifdef BLZ3_USE_OPENEXR
			"[-x]"
#else
			""
#endif
		);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -d        debug level output\n");
		b3PrintF(B3LOG_NORMAL, "  -f        verbose level output\n");
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -a        disable animation\n");
		b3PrintF(B3LOG_NORMAL, "  -n        disable display\n");
		b3PrintF(B3LOG_NORMAL, "  -w        nowait after display output\n");
		b3PrintF(B3LOG_NORMAL, "  -S        force one raytracing thread\n");
		b3PrintF(B3LOG_NORMAL, "  -M        use multithreading for raytracing using all CPU cores\n");
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -s size   image size definition\n");
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -g        TGA image saving\n");
#ifdef HAVE_LIBTIFF
		b3PrintF(B3LOG_NORMAL, "  -i        TIFF image saving\n");
#endif
#ifdef HAVE_LIBJPEG
		b3PrintF(B3LOG_NORMAL, "  -j        JPEG image saving (default)\n");
#endif
		b3PrintF(B3LOG_NORMAL, "  -r        RGB8 image saving\n");
		b3PrintF(B3LOG_NORMAL, "  -p        PostScript image saving\n");
#ifdef BLZ3_USE_OPENEXR
		b3PrintF(B3LOG_NORMAL, "  -x        OpenEXR image saving\n");
#endif
		b3PrintF(B3LOG_NORMAL, "\n");
	}
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
}

int main(int argc, char * argv[])
{
	b3ShapeRenderContext  context;
	b3World       *       world;
	b3Display      *      display;
	b3Scene       *       scene;
	b3Animation     *     animation;
	b3CameraPart     *    camera;
	b3Item        *       item;
	b3_vector             lower, upper;
	const char      *     BLZ3_DATA        = getenv("BLZ3_DATA");
	const char      *     BLZ3_TEXTURES    = getenv("BLZ3_TEXTURES");
	const char      *     BLZ3_PICTURES    = getenv("BLZ3_PICTURES");
	const char      *     BLZ3_PLUGINS     = getenv("BLZ3_PLUGINS");
	const char      *     BLZ3_BIN         = getenv("BLZ3_BIN");
	const char      *     BLZ3_RENDER_PRIO = getenv("BLZ3_RENDER_PRIO");
	const char      *     HOME             = getenv("HOME");
	b3Path                textures;
	b3Path                pictures;
	b3Path                data;
	b3Loader              loader;
	b3_bool               force_no_anim    = false;
	b3_bool               force_no_display = false;
	b3_bool               force_no_wait    = false;
	b3_bool               multi_threaded   = true;
	b3_index              i;
	b3_count              CPUs = b3Runtime::b3GetNumCPUs();
	b3_res                size = 0;

	context.b3UseGL(false);

	if (argc > 1)
	{
		world = new b3World();
		if (HOME != nullptr)
		{
			b3Dir::b3LinkFileName(textures, HOME, "Blizzard/Textures");
			b3Dir::b3LinkFileName(pictures, HOME, "Blizzard/Pictures");
			b3Dir::b3LinkFileName(data,     HOME, "Blizzard/Data");
			b3Scene::m_TexturePool.b3AddPath(textures);
			b3Scene::m_TexturePool.b3AddPath(BLZ3_TEXTURES);
			b3Scene::m_TexturePool.b3AddPath(pictures);
			b3Scene::m_TexturePool.b3AddPath(BLZ3_PICTURES);

			world->b3AddPath(data);
			world->b3AddPath(BLZ3_DATA);
		}

		b3RaytracingItems::b3Register();
#ifdef HAVE_VIDEO_ENCODER
		b3CodecRegister::b3Instance().b3PrepareCodecs();
#endif
		loader.b3AddPath(BLZ3_BIN);
		loader.b3AddPath(BLZ3_PLUGINS);
		loader.b3Load();

		if (BLZ3_RENDER_PRIO != nullptr)
		{
			b3Scene::m_RenderPriority = b3Math::b3Clamp(atoi(BLZ3_RENDER_PRIO), -2, 2);
		}

		b3PrintF(B3LOG_NORMAL, "\nFound %d CPU core%s.\n",
			CPUs,
			CPUs > 1 ? "s" : "");

		b3PrintF(B3LOG_DEBUG, "Used environment:\n");
		b3PrintF(B3LOG_DEBUG, "HOME=%s\n",             HOME);
		b3PrintF(B3LOG_DEBUG, "BLZ3_DATA=%s\n",        BLZ3_DATA);
		b3PrintF(B3LOG_DEBUG, "BLZ3_TEXTURES=%s\n",    BLZ3_TEXTURES);
		b3PrintF(B3LOG_DEBUG, "BLZ3_PICTURES=%s\n",    BLZ3_PICTURES);
		b3PrintF(B3LOG_DEBUG, "BLZ3_PLUGINS=%s\n",     BLZ3_PLUGINS);
		b3PrintF(B3LOG_DEBUG, "BLZ3_BIN=%s\n",         BLZ3_BIN);
		b3PrintF(B3LOG_DEBUG, "BLZ3_RENDER_PRIO=%s\n", BLZ3_RENDER_PRIO);

		for (i = 1; i < argc; i++)
		{
			char    number[1024];

			if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
				case 'd' :
					b3Log::b3SetLevel(B3LOG_DEBUG);
					break;
				case 'f' :
					b3Log::b3SetLevel(B3LOG_FULL);
					break;
				case 'a' :
					force_no_anim = true;
					b3PrintF(B3LOG_NORMAL, "Forcing no animation.\n");
					break;
				case 'n' :
					force_no_display = true;
					b3PrintF(B3LOG_NORMAL, "Forcing no display output.\n");
					break;
				case 'w' :
					force_no_wait = true;
					b3PrintF(B3LOG_NORMAL, "Forcing no wait after display output.\n");
					break;
				case 's':
					i = b3Runtime::b3ParseOption(argc, argv, i, number, sizeof(number));
					size = atoi(number);
					break;
#ifdef HAVE_LIBTIFF
				case 'i':
					strlcpy(BLZ3_EXTENSION, ".tif", sizeof(BLZ3_EXTENSION));
					break;
#endif
#ifdef HAVE_LIBJPEG
				case 'j':
					strlcpy(BLZ3_EXTENSION, ".jpg", sizeof(BLZ3_EXTENSION));
					break;
#endif
				case 'g':
					strlcpy(BLZ3_EXTENSION, ".tga", sizeof(BLZ3_EXTENSION));
					break;
				case 'r':
					strlcpy(BLZ3_EXTENSION, ".rgb8", sizeof(BLZ3_EXTENSION));
					break;
				case 'p':
					strlcpy(BLZ3_EXTENSION, ".ps", sizeof(BLZ3_EXTENSION));
					break;
				case 'S':
					multi_threaded = false;
					b3PrintF(B3LOG_NORMAL, "Forcing one raytracing thread.\n");
					break;
				case 'M':
					multi_threaded = true;
					b3PrintF(B3LOG_NORMAL, "Using multithreading.\n");
					break;
#ifdef BLZ3_USE_OPENEXR
				case 'x':
					strlcpy(BLZ3_EXTENSION, ".exr", sizeof(BLZ3_EXTENSION));
					break;
#endif

				case 'v' :
					b3Banner(nullptr);
					break;
				}
			}
			else
			{
				try
				{
					world->b3Read(argv[i]);
					for (item  = world->b3GetFirst();
						item != nullptr;
						item  = scene->Succ)
					{
						scene = (b3Scene *)item;
						scene->b3Reorg();
						scene->b3SetupVertexMemory(&context);
						scene->b3SetFilename(argv[i]);
						animation = scene->b3GetAnimation();
						if (animation != nullptr)
						{
							if ((!animation->b3IsActive()) || (force_no_anim))
							{
								b3PrintF(B3LOG_NORMAL, "Animation deactivated...\n");
								animation = nullptr;
							}
							else
							{
								b3PrintF(B3LOG_NORMAL, "Using animation...\n");
							}
						}

						display = b3AllocDisplay(scene, force_no_display, size);
						if ((camera = scene->b3GetFirstCamera(false)) != nullptr)
						{
							do
							{
								if (camera->m_Flags & CAMERA_ACTIVE)
								{
									scene->b3SetCamera(camera);
									b3PrintF(B3LOG_NORMAL, "Rendering \"%s\"...\n",
										camera->m_CameraName);

									if (animation != nullptr)
									{
										b3_f64   t;
										b3_index iEnd, frame;
										b3Path   img_name;
#ifdef HAVE_VIDEO_ENCODER
										img_name.b3LinkFileName(BLZ3_PICTURES, camera->b3GetName());
										img_name.b3RemoveExt();
										img_name.b3Append(".webm");

										b3MovieEncoder encoder(img_name, *display, animation->m_FramesPerSecond);
#else
										b3_count count = 0;
#endif

										b3PrintF(B3LOG_NORMAL, "Animating!!!\n\n");
										scene->b3ResetAnimation();
										iEnd = animation->b3AnimFrameIndex(animation->m_End);
										for (frame = 0; frame <= iEnd; frame++)
										{
											t = animation->b3AnimTimeCode(frame);
											b3PrintF(B3LOG_NORMAL, "Rendering frame t=%1.2f\n", t);
											scene->b3SetAnimation(t);
											scene->b3Raytrace(display, multi_threaded);
#ifdef HAVE_VIDEO_ENCODER
											encoder.b3AddFrame(*display);
#else
											img_name.b3Format("%s_%04d", camera->b3GetName(), count++);

											b3SaveRaytracedImage(
												display,
												BLZ3_PICTURES, img_name);
#endif
										}
									}
									else
									{
										scene->b3ComputeBounds(&lower, &upper);
										scene->b3Raytrace(display, multi_threaded);
										b3SaveRaytracedImage(
											display,
											BLZ3_PICTURES, camera->b3GetName());
									}
								}
								else
								{
									b3PrintF(B3LOG_NORMAL, "Skipping \"%s\"...\n",
										camera->m_CameraName);
								}
								camera = scene->b3GetNextCamera(camera);
							}
							while (camera != nullptr);
						}
						else
						{
							// Default camera without any camera
							// in special list
							b3PrintF(B3LOG_NORMAL, "Rendering default camera...\n");
							scene->b3ComputeBounds(&lower, &upper);
							scene->b3Raytrace(display, multi_threaded);
							if (!b3SaveRaytracedImage(
									display,
									BLZ3_PICTURES, scene->b3GetName()))
							{
								b3PrintF(B3LOG_NORMAL, "Cannot save image %s!\n", scene->b3GetName());
							}
						}

						if (!force_no_wait)
						{
							display->b3Wait();
						}
						delete display;
					}
				}
				catch (b3WorldException & w)
				{
					b3PrintF(B3LOG_NORMAL, "Error parsing %s\n", argv[i]);
					b3PrintF(B3LOG_NORMAL, "Error code: %d\n", w.b3GetError());
					b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", w.b3GetErrorMsg());
				}
				catch (b3FileException & f)
				{
					b3PrintF(B3LOG_NORMAL, "File IO error using %s\n", argv[i]);
					b3PrintF(B3LOG_NORMAL, "Error code: %d\n", f.b3GetError());
					b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", f.b3GetErrorMsg());
				}
				catch (b3TxException & t)
				{
					b3PrintF(B3LOG_NORMAL, "Image error using %s\n", argv[i]);
					b3PrintF(B3LOG_NORMAL, "Error code: %d\n", t.b3GetError());
					b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", t.b3GetErrorMsg());
				}
				catch (...)
				{
					b3PrintF(B3LOG_NORMAL, "Unknown error occured loading %s\n", argv[i]);
				}
			}
		}
		delete world;
	}
	else
	{
		b3Banner(argv[0]);
	}
	return EXIT_SUCCESS;
}
