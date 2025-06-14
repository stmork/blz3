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

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

#include <blz3/system/b3Dir.h>
#include <blz3/system/b3Plugin.h>
#include <blz3/base/b3World.h>
#include <blz3/raytrace/b3Scene.h>
#include <blz3/raytrace/b3RenderView.h>

#include "b3CameraVolume.h"
#include "b3AbstractItem.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class QStandardItemModel;
class QStandardItem;
class QB3OpenGLScrollArea;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget * parent = nullptr);
	~MainWindow();

private slots:
	void animate(int frame);
	void on_selectedCamera(QStandardItem * item);
	void on_selectLight(QStandardItem * item);
	void on_itemChanged(QStandardItem * item);

	void on_actionOpenScene_triggered();
	void on_actionQuit_triggered();

	void on_actionView3D_triggered();
	void on_actionViewFront_triggered();
	void on_actionViewTop_triggered();
	void on_actionViewLeft_triggered();
	void on_actionViewRight_triggered();

	void on_actionViewZoomIn_triggered();
	void on_actionViewZoomOut_triggered();
	void on_actionViewPop_triggered();
	void on_actionViewSelect_triggered();
	void on_actionViewFull_triggered();
	void on_actionViewMoveRight_triggered();
	void on_actionViewMoveLeft_triggered();
	void on_actionViewMoveUp_triggered();
	void on_actionViewMoveDown_triggered();

	void on_actionActivateAll_triggered();
	void on_actionDeaktivateAll_triggered();
	void on_actionActivate_triggered();
	void on_actionDeactivate_triggered();
	void on_actionDeactivateOther_triggered();
	void on_actionDeactivateAllOther_triggered();

	void on_actionAnimPlay_triggered();
	void on_actionAnimStop_triggered();
	void on_actionAnimPause_triggered();
	void on_actionAnimRepeat_triggered();

	void on_actionLightSimple_triggered();
	void on_actionLightAll_triggered();
	void on_actionLightSpot_triggered();

	void on_cameraListView_clicked(const QModelIndex & index);

	void on_selection_end();

private:
	QString timecode(const int frame) const;
	void    enableView(const b3_view_mode mode);
	void    enableAnimation();
	void    enableLight();
	void    populateTreeView(void);
	void    populateTreeView(QStandardItem * item, b3Base<b3Item> * base);
	void    updateTreeView();
	void    updateTreeView(QStandardItem * item);

	void    free();
	void    prepareUI();

	b3BBox    *    getSelectedBBox();
	b3CameraPart * getSelectedCamera();

	void selectCamera(b3CameraPart * camera);
	void selectBBox(b3BBox * bbox);
	QB3AbstractItem<b3BBox> * findBBoxItem(b3BBox * bbox);

	Ui::MainWindow    *   ui           = nullptr;
	QStandardItemModel  * camera_model = nullptr;
	QStandardItemModel  * light_model  = nullptr;
	QStandardItemModel  * bbox_model   = nullptr;
	QPropertyAnimation    animation;

	b3Path                textures;
	b3Path                pictures;
	b3Path                data;
	b3Loader              loader;
	b3World               m_World;
	b3CameraVolume        m_CameraVolume;
	b3Scene       *       m_Scene     = nullptr;
	b3Animation     *     m_Animation = nullptr;
};

#endif // MAINWINDOW_H
