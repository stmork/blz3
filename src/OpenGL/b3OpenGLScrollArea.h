/*
**
**	$Filename:	b3OpenGLScrollArea.h $
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

#ifndef QB3OPENGLSCROLLAREA_H
#define QB3OPENGLSCROLLAREA_H

#include <QScrollArea>

#include "b3OpenGLWidget.h"

class QB3OpenGLScrollArea : public QScrollArea
{
	Q_OBJECT

public:
	explicit QB3OpenGLScrollArea(QB3OpenGLWidget * glWidget);

protected:
	void resizeEvent(QResizeEvent * event) override;
	void paintEvent(QPaintEvent * event) override;

private:
	void update();

	QB3OpenGLWidget * child;
};

#endif // QB3OPENGLSCROLLAREA_H
