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

#include <blz3/system/b3Dir.h>
#include <blz3/system/b3Plugin.h>
#include <blz3/base/b3World.h>
#include <blz3/raytrace/b3Scene.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

private slots:
	void animate(int frame);

private:
	QString timecode(const int frame) const;

	Ui::MainWindow * ui;

	b3Path          textures;
	b3Path          pictures;
	b3Path          data;
	b3Loader        loader;
	b3World         m_World;
	b3Scene *       m_Scene     = nullptr;
	b3Animation *   m_Animation = nullptr;
};

#endif // MAINWINDOW_H
