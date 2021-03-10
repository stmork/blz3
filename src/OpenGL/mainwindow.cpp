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

#include "mainwindow.h"
#include "ui_mainwindow.h"


/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Plugin.h"
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/raytrace/b3RenderLight.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	const char * BLZ3_PLUGINS = getenv("BLZ3_PLUGINS");
	const char * BLZ3_BIN     = getenv("BLZ3_BIN");
	const char * HOME         = getenv("HOME");

	b3Dir::b3LinkFileName(data,     HOME, "Blizzard/Data");
	b3Dir::b3LinkFileName(textures, HOME, "Blizzard/Textures");
	b3Dir::b3LinkFileName(pictures, HOME, "Blizzard/Pictures");

	b3Scene::m_TexturePool.b3AddPath(textures);
	b3Scene::m_TexturePool.b3AddPath(pictures);

	b3RaytracingItems::b3Register();
	if (BLZ3_BIN != nullptr)
	{
		loader.b3AddPath(BLZ3_BIN);
	}
	if (BLZ3_PLUGINS != nullptr)
	{
		loader.b3AddPath(BLZ3_PLUGINS);
	}
	loader.b3Load();

	world.b3AddPath(data);
	world.b3Read("FlippAmiga.bwd");
}

MainWindow::~MainWindow()
{
	delete ui;
}

