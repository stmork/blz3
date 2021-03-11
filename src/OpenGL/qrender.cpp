/*
**
**	$Filename:	qrender.cpp $
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

#include <QApplication>

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

int main(int argc, char *argv[])
{
	b3Log::b3SetLevel(B3LOG_DEBUG);

	b3PrintF(B3LOG_NORMAL, "Blizzard III Qt OpenGL scene viewer\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2021\n");
	b3PrintF(B3LOG_NORMAL, "\n");

	QApplication app(argc, argv);
	MainWindow   w;

	w.show();
	return app.exec();
}
