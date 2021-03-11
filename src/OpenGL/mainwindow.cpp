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
	QSurfaceFormat format;
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

	// must be called before the widget or its parent window gets shown
	QSurfaceFormat::setDefaultFormat(format);

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
	m_Scene     = static_cast<b3Scene *>(world.b3GetFirst());
	m_Animation = m_Scene->b3GetAnimation();
	ui->glView->b3Prepare(m_Scene);

	if (m_Animation != nullptr)
	{
		const b3_count fps = m_Animation->m_FramesPerSecond;

		ui->animationSlider->setEnabled(true);
		ui->animationSlider->setTickInterval(fps);
		ui->animationSlider->setPageStep(fps * 5);
		ui->animationSlider->setMinimum(0);
		ui->animationSlider->setMaximum(fps * (m_Animation->m_End - m_Animation->m_Start));
		ui->animationSlider->setValue(0);
	}
	else
	{
		ui->animationSlider->setDisabled(true);
	}
	connect(
				ui->animationSlider, &QSlider::valueChanged,
				this, &MainWindow::animate);
}

MainWindow::~MainWindow()
{
	world.b3Free();

	delete ui;
}

void MainWindow::animate(int frame)
{
	if (m_Animation != nullptr)
	{
		const b3_count fps = m_Animation->m_FramesPerSecond;
		const double   t   = m_Animation->m_Start + (double)frame / fps;

		b3PrintF(B3LOG_DEBUG, "t=%1.3fs\n", t);
		m_Scene->b3SetAnimation(t);
		update();
	}

	ui->animationLabel->setText(timecode(frame));
	ui->glView->update();
}

QString MainWindow::timecode(const int frame) const
{
	if (m_Animation != nullptr)
	{
		const b3_count fps    = m_Animation->m_FramesPerSecond;
		const double   t      = m_Animation->m_Start + (double)frame / fps;
		const int      hour   = t / 3600;
		const unsigned minute = abs(int(t) / 60) % 60;
		const unsigned second = abs(int(t)) % 60;
		const unsigned sub    = abs(int(t * fps)) % fps;

		return QString::asprintf("%02d:%02u:%02u.%02u  %3d",
								 hour, minute, second, sub, frame);
	}
	else
	{
		return "";
	}
}
