/*
**
**	$Filename:	mainwindow.cc $
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

#include <QFileDialog>

#include "b3CameraItem.h"
#include "b3LightItem.h"

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
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	world_icon(":/treeview/World"),
	bbox_taxonomy_map
	{
		QIcon(":/treeview/BBox_empty"),        // BBOX_EMPTY
		QIcon(":/treeview/BBox_empty"),        // BBOX_ACTIVATED
		QIcon(":/treeview/BBox_sub"),          // BBOX_BBOX_SUB
		QIcon(":/treeview/BBox_sub"),          // BBOX_BBOX_SUB | BBOX_ACTIVATED
		QIcon(":/treeview/BBox_Shape"),        // BBOX_SHAPE_SUB
		QIcon(":/treeview/BBox_Shape_sel"),    // BBOX_SHAPE_SUB | BBOX_ACTIVATED
		QIcon(":/treeview/BBox_Shape_sub"),    // BBOX_SHAPE_SUB | BBOX_BBOX_SUB
		QIcon(":/treeview/BBox_Shape_sub_sel") // BBOX_SHAPE_SUB | BBOX_BBOX_SUB | BBOX_ACTIVATED
	}
{
	QSurfaceFormat format;
	format.setSamples(4);
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

	camera_model = new QStandardItemModel(ui->cameraListView);
	ui->cameraListView->setModel(camera_model);

	light_model = new QStandardItemModel(ui->lightListView);
	ui->lightListView->setModel(light_model);

	bbox_model = new QStandardItemModel(ui->treeView);
	ui->treeView->setModel(bbox_model);

	animation.setTargetObject(ui->animationSlider);
	animation.setPropertyName("value");

	connect(
				ui->animationSlider, &QSlider::valueChanged,
				this, &MainWindow::animate);
	connect(
				camera_model,  &QStandardItemModel::itemChanged,
				this, &MainWindow::selectCamera);
	connect(
				light_model,  &QStandardItemModel::itemChanged,
				this, &MainWindow::selectLight);

	m_World.b3Read("FlippAmiga.bwd");
	m_Scene     = static_cast<b3Scene *>(m_World.b3GetFirst());
	m_Animation = m_Scene->b3GetAnimation();
	ui->glView->b3Prepare(m_Scene);
	prepareUI();
}

MainWindow::~MainWindow()
{
	free();
	delete ui;
}

void MainWindow::free()
{
	camera_model->clear();
	light_model->clear();
	bbox_model->clear();
	m_World.b3Free();

	m_Scene     = nullptr;
	m_Animation = nullptr;
}

void MainWindow::prepareUI()
{
	for(b3CameraPart * camera = m_Scene->b3GetFirstCamera();
		camera != nullptr;
		camera  = m_Scene->b3GetNextCamera(camera))
	{
		QB3CameraItem * item = new QB3CameraItem(camera);

		camera_model->appendRow(item);
	}
	B3_FOR_TYPED_BASE(b3Light, m_Scene->b3GetLightHead(), light)
	{
		QB3LightItem * item = new QB3LightItem(light);

		light_model->appendRow(item);
	}
	populateTreeView();

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

	enableView(B3_VIEW_3D);
	enableLight();
	enableAnimation();
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

void MainWindow::selectCamera(QStandardItem * item)
{
	QB3CameraItem * camera_item = static_cast<QB3CameraItem *>(item);

	camera_item->check();
}

void MainWindow::selectLight(QStandardItem * item)
{
	QB3LightItem * light_item = static_cast<QB3LightItem *>(item);

	light_item->check();
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
	ui->actionAnimStop->setChecked(animation.state() == QPropertyAnimation::Stopped);
	ui->actionAnimPause->setChecked(animation.state() == QPropertyAnimation::Paused);
}

void MainWindow::enableLight()
{
	const bool all = ui->glView->b3IsAllLights();

	ui->actionLightSimple->setChecked(!all);
	ui->actionLightAll->setChecked(all);
	ui->actionLightSpot->setEnabled(all);
	ui->actionLightSpot->setChecked(ui->glView->b3IsSpotLight() && all);
}

void MainWindow::populateTreeView()
{
	QStandardItem * world = new QStandardItem(world_icon, m_Scene->b3GetFilename());

	bbox_model->appendRow(world);
	populateTreeView(world, m_Scene->b3GetBBoxHead());
}

void MainWindow::populateTreeView(QStandardItem * parent, b3Base<b3Item> * base)
{
	B3_FOR_TYPED_BASE(b3BBox, base, bbox)
	{
		b3Base<b3Item> * sub_bboxes = bbox->b3GetBBoxHead();
		QStandardItem *  item;
		unsigned         taxonomy = BBOX_EMPTY;

		if (bbox->b3IsActive())
		{
			taxonomy |= bbox_taxonomy::BBOX_ACTIVATED;
		}
		if (!sub_bboxes->b3IsEmpty())
		{
			taxonomy |= BBOX_BBOX_SUB;
		}
		if (!bbox->b3GetShapeHead()->b3IsEmpty())
		{
			taxonomy |= BBOX_SHAPE_SUB;
		}

		const QIcon & icon = bbox_taxonomy_map[taxonomy];
		item = new QStandardItem(icon, QString::fromLatin1(bbox->b3GetName()));
		if (bbox->b3IsExpanded())
		{
			// TODO: Expand item here!
		}
		parent->appendRow(item);
		if (taxonomy & BBOX_BBOX_SUB)
		{
			populateTreeView(item, sub_bboxes);
		}
	}
}


void MainWindow::on_actionOpenScene_triggered()
{
	QFileDialog dialog(this, tr("Blizzard III Szene öffnen"), QString(data), "*.bwd");

	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	if (dialog.exec())
	{
		QStringList files = dialog.selectedFiles();

		free();
		m_World.b3Read(files.first().toStdString().c_str());
		m_Scene     = static_cast<b3Scene *>(m_World.b3GetFirst());
		m_Animation = m_Scene->b3GetAnimation();
		ui->glView->b3Prepare(m_Scene);
		prepareUI();
	}
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

void MainWindow::on_actionLightSimple_triggered()
{
	ui->glView->b3SetAllLights(false);
	enableLight();
}

void MainWindow::on_actionLightAll_triggered()
{
	ui->glView->b3SetAllLights(true);
	enableLight();
}

void MainWindow::on_cameraListView_clicked(const QModelIndex &index)
{
	QB3CameraItem * camera_item = static_cast<QB3CameraItem *>(camera_model->itemFromIndex(index));

	ui->glView->b3SetCamera(*camera_item);
}

void MainWindow::on_actionLightSpot_triggered()
{
	ui->glView->b3SetSpotLight(!ui->glView->b3IsSpotLight());
	enableLight();
}
