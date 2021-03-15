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

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class QStandardItemModel;
class QStandardItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

private slots:
	void animate(int frame);
	void selectCamera(QStandardItem * item);
	void selectLight(QStandardItem * item);

	void on_actionOpenScene_triggered();
	void on_actionQuit_triggered();

	void on_actionView3D_triggered();
	void on_actionViewFront_triggered();
	void on_actionViewTop_triggered();
	void on_actionViewLeft_triggered();
	void on_actionViewRight_triggered();

	void on_actionActivateAll_triggered();
	void on_actionDeaktivateAll_triggered();

	void on_actionAnimPlay_triggered();
	void on_actionAnimStop_triggered();
	void on_actionAnimPause_triggered();
	void on_actionAnimRepeat_triggered();

	void on_actionLightSimple_triggered();
	void on_actionLightAll_triggered();

	void on_cameraListView_clicked(const QModelIndex & index);

	void on_actionLightSpot_triggered();

private:
	QString timecode(const int frame) const;
	void    enableView(const b3_view_mode mode);
	void    enableAnimation();
	void    enableLight();
	void    populateTreeView(void);
	void    populateTreeView(QStandardItem * item, b3Base<b3Item> * base);

	void    free();
	void    prepareUI();

	enum bbox_taxonomy
	{
		BBOX_EMPTY     = 0,
		BBOX_ACTIVATED = 1,
		BBOX_BBOX_SUB  = 2,
		BBOX_SHAPE_SUB = 4,
		BBOX_MASK      = BBOX_ACTIVATED | BBOX_BBOX_SUB | BBOX_SHAPE_SUB
	};

	Ui::MainWindow   *   ui;
	QPropertyAnimation   animation;
	QStandardItemModel * camera_model;
	QStandardItemModel * light_model;
	QStandardItemModel * bbox_model;
	QIcon                world_icon;
	QIcon                bbox_taxonomy_map[8];

	b3Path               textures;
	b3Path               pictures;
	b3Path               data;
	b3Loader             loader;
	b3World              m_World;
	b3Scene       *      m_Scene     = nullptr;
	b3Animation     *    m_Animation = nullptr;
};

#endif // MAINWINDOW_H
