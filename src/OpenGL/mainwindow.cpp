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
#include "b3BBoxItem.h"
#include "b3SceneItem.h"

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
	ui(new Ui::MainWindow)
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
				this, &MainWindow::on_selectedCamera);
	connect(
				light_model,  &QStandardItemModel::itemChanged,
				this, &MainWindow::on_selectLight);
	connect(
				bbox_model, &QStandardItemModel::itemChanged,
				this, &MainWindow::on_itemChanged);

	m_World.b3Read("FlippAmiga.bwd");
	m_Scene     = static_cast<b3Scene *>(m_World.b3GetFirst());
	m_Scene->b3SetFilename("FlippAmiga");
	m_Animation = m_Scene->b3GetAnimation();
	ui->glView->b3Prepare(m_Scene, &m_CameraVolume);
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
	selectCamera(m_Scene->b3GetActualCamera());

	B3_FOR_TYPED_BASE(b3Light, m_Scene->b3GetLightHead(), light)
	{
		QB3LightItem * item = new QB3LightItem(light);

		light_model->appendRow(item);
	}

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
	populateTreeView();

	enableView(B3_VIEW_3D);
	enableLight();
	enableAnimation();
}

b3BBox * MainWindow::getSelectedBBox()
{
	const QModelIndex & index = ui->treeView->currentIndex();
	const QB3BBoxItem * item  = static_cast<QB3BBoxItem *>(bbox_model->itemFromIndex(index));

	if (item != nullptr)
	{
		return *item;
	}
	else
	{
		return nullptr;
	}
}

b3CameraPart * MainWindow::getSelectedCamera()
{
	const QModelIndex &   index = ui->cameraListView->currentIndex();
	const QB3CameraItem * item  = static_cast<QB3CameraItem *>(camera_model->itemFromIndex(index));

	if (item != nullptr)
	{
		return *item;
	}
	else
	{
		return nullptr;
	}
}

void MainWindow::selectCamera(b3CameraPart * camera)
{
	const QModelIndexList & list = QB3CameraItem::match(camera_model, camera);

	if (!list.isEmpty())
	{
		ui->cameraListView->setCurrentIndex(list.first());
	}
}

void MainWindow::selectBBox(b3BBox * bbox)
{
	const QModelIndexList & list = QB3BBoxItem::match(bbox_model, bbox);

	if (!list.isEmpty())
	{
		ui->treeView->setCurrentIndex(list.first());
	}
}

QB3AbstractItem<b3BBox> * MainWindow::findBBoxItem(b3BBox * bbox)
{
	return QB3BBoxItem::find(bbox_model, bbox);
}

void MainWindow::animate(int frame)
{
	if (m_Animation != nullptr)
	{
		const b3_count fps = m_Animation->m_FramesPerSecond;
		const double   t   = m_Animation->m_Start + (double)frame / fps;

		b3PrintF(B3LOG_DEBUG, "t=%1.3fs\n", t);
		m_Scene->b3SetAnimation(t);
		m_CameraVolume.b3Update(getSelectedCamera());
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

/*************************************************************************
**                                                                      **
**                        UI control and updates                        **
**                                                                      **
*************************************************************************/

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
	QStandardItem * world = new QB3SceneItem(m_Scene);

	bbox_model->appendRow(world);
	ui->treeView->setExpanded(world->index(), true);
	populateTreeView(world, m_Scene->b3GetBBoxHead());
}

void MainWindow::populateTreeView(QStandardItem * parent, b3Base<b3Item> * base)
{
	B3_FOR_TYPED_BASE(b3BBox, base, bbox)
	{
		b3Base<b3Item> * sub_bboxes = bbox->b3GetBBoxHead();
		QStandardItem *  item       = new QB3BBoxItem(bbox);

		parent->appendRow(item);
		if (!sub_bboxes->b3IsEmpty())
		{
			populateTreeView(item, sub_bboxes);
		}

		// Update expansion
		const QModelIndex & index = item->index();
		ui->treeView->setExpanded(index, bbox->b3IsExpanded());
	}
}

void MainWindow::updateTreeView()
{
	updateTreeView(bbox_model->item(0));
}

void MainWindow::updateTreeView(QStandardItem * parent)
{
	for (int i = 0; i < parent->rowCount(); i++)
	{
		QB3BBoxItem * item = static_cast<QB3BBoxItem *>(parent->child(i));

		item->update();
		if (item->hasChildren())
		{
			updateTreeView(item);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Actions                                       **
**                                                                      **
*************************************************************************/

void MainWindow::on_selectedCamera(QStandardItem * item)
{
	QB3CameraItem * camera_item = static_cast<QB3CameraItem *>(item);

	camera_item->check();
}

void MainWindow::on_selectLight(QStandardItem * item)
{
	QB3LightItem * light_item = static_cast<QB3LightItem *>(item);

	light_item->check();
}

void MainWindow::on_itemChanged(QStandardItem * item)
{
	b3BBox * bbox = item->data().value<b3BBox *>();

	bbox->b3Expand(ui->treeView->isExpanded(item->index()));
	bbox->b3SetName(item->text().toLatin1().constData());
}

void MainWindow::on_actionOpenScene_triggered()
{
	QFileDialog dialog(this, tr("Blizzard III Szene öffnen"), QString(data), "*.bwd");

	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	if (dialog.exec())
	{
		const QStringList files = dialog.selectedFiles();
		const std::string filename = files.first().toStdString().c_str();

		free();
		m_World.b3Read(filename.c_str());
		m_Scene     = static_cast<b3Scene *>(m_World.b3GetFirst());
		m_Scene->b3SetFilename(filename.c_str());
		m_Animation = m_Scene->b3GetAnimation();
		prepareUI();
		ui->glView->b3Prepare(m_Scene, &m_CameraVolume);
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
	updateTreeView();
	ui->glView->update();
}

void MainWindow::on_actionDeaktivateAll_triggered()
{
	m_Scene->b3Activate(false);
	updateTreeView();
	ui->glView->update();
}

void MainWindow::on_actionActivate_triggered()
{
	b3BBox * bbox = getSelectedBBox();

	if (bbox != nullptr)
	{
		bbox->b3Activate(true);
		updateTreeView();
		ui->glView->update();

		// Proceed with next bbox.
		selectBBox((b3BBox *)bbox->Succ);
	}
}

void MainWindow::on_actionDeactivate_triggered()
{
	b3BBox * bbox = getSelectedBBox();

	if (bbox != nullptr)
	{
		bbox->b3Activate(false);
		updateTreeView();
		ui->glView->update();

		// Proceed with next bbox.
		selectBBox((b3BBox *)bbox->Succ);
	}
}

void MainWindow::on_actionDeactivateOther_triggered()
{
	b3BBox * bbox = getSelectedBBox();

	if (bbox != nullptr)
	{
		b3BBox * aux;

		bbox->b3Activate(true);
		for (aux = (b3BBox *)bbox->Prev;aux != nullptr;aux = (b3BBox *)aux->Prev)
		{
			aux->b3Activate(false);
		}
		for (aux = (b3BBox *)bbox->Succ;aux != nullptr;aux = (b3BBox *)aux->Succ)
		{
			aux->b3Activate(false);
		}
		updateTreeView();
		ui->glView->update();
	}
}

void MainWindow::on_actionDeactivateAllOther_triggered()
{
	b3BBox * bbox = getSelectedBBox();

	if (bbox != nullptr)
	{
		m_Scene->b3Activate(false);
		bbox->b3Activate(true);
		updateTreeView();
		ui->glView->update();
	}
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

void MainWindow::on_actionLightSpot_triggered()
{
	ui->glView->b3SetSpotLight(!ui->glView->b3IsSpotLight());
	enableLight();
}

void MainWindow::on_cameraListView_clicked(const QModelIndex &index)
{
	QB3CameraItem * camera_item = static_cast<QB3CameraItem *>(camera_model->itemFromIndex(index));

	ui->glView->b3SetCamera(*camera_item);
}
