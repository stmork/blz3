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

	m_World.b3AddPath(data);
	m_World.b3Read("FlippAmiga.bwd");
	m_Scene     = static_cast<b3Scene *>(m_World.b3GetFirst());
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

		animation.setStartValue(0);
		animation.setEndValue(ui->animationSlider->maximum());
		animation.setDuration((m_Animation->m_End - m_Animation->m_Start) * 1000);
		animate(0);
	}
	else
	{
		ui->animationSlider->setDisabled(true);
	}

	animation.setTargetObject(ui->animationSlider);
	animation.setPropertyName("value");

	connect(
				ui->animationSlider, &QSlider::valueChanged,
				this, &MainWindow::animate);
}

MainWindow::~MainWindow()
{
//	m_Scene->b3FreeVertices();
	m_World.b3Free();

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

void MainWindow::enableView(const b3_view_mode mode)
{
	ui->actionView3D->setChecked(mode == B3_VIEW_3D);
	ui->actionViewFront->setChecked(mode == B3_VIEW_FRONT);
	ui->actionViewTop->setChecked(mode == B3_VIEW_TOP);
	ui->actionViewRight->setChecked(mode == B3_VIEW_RIGHT);
	ui->actionViewLeft->setChecked(mode == B3_VIEW_LEFT);

	ui->glView->b3SetViewmode(mode);
}

void MainWindow::enableAnimation()
{
	ui->actionAnimPlay->setChecked(animation.state() == QPropertyAnimation::Running);
	ui->actionAnimStop->setChecked(animation.state() == QPropertyAnimation::Paused);
	ui->actionAnimPause->setChecked(animation.state() == QPropertyAnimation::Paused);
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionView3D_triggered()
{
	enableView(B3_VIEW_3D);
}

void MainWindow::on_actionViewFront_triggered()
{
	enableView(B3_VIEW_FRONT);
}

void MainWindow::on_actionViewTop_triggered()
{
	enableView(B3_VIEW_TOP);
}

void MainWindow::on_actionViewLeft_triggered()
{
	enableView(B3_VIEW_LEFT);
}

void MainWindow::on_actionViewRight_triggered()
{
	enableView(B3_VIEW_RIGHT);
}

void MainWindow::on_actionActivateAll_triggered()
{
	m_Scene->b3Activate(true);
	ui->glView->update();
}

void MainWindow::on_actionDeaktivateAll_triggered()
{
	m_Scene->b3Activate(false);
	ui->glView->update();
}

void MainWindow::on_actionAnimPlay_triggered()
{
	animation.start();
	enableAnimation();
}

void MainWindow::on_actionAnimStop_triggered()
{
	animation.stop();
	enableAnimation();
}

void MainWindow::on_actionAnimPause_triggered()
{
	switch(animation.state())
	{
	case QPropertyAnimation::Running:
		animation.pause();
		break;

	case QPropertyAnimation::Stopped:
		animation.start();
		break;

	case QPropertyAnimation::Paused:
		animation.resume();
		break;
	}
	enableAnimation();
}

void MainWindow::on_actionAnimRepeat_triggered()
{
	bool repeat = animation.loopCount() < 0;

	animation.setLoopCount(repeat ? 1 : -1);
	ui->actionAnimRepeat->setChecked(animation.loopCount() < 0);
}
