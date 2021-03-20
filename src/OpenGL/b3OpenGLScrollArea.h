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

class QB3OpenGLScrollArea : public QAbstractScrollArea
{
	Q_OBJECT

public:
	explicit QB3OpenGLScrollArea(QB3OpenGLWidget * glWidget);

	void b3SetViewmode(const b3_view_mode mode);
	void b3MoveView(const b3_f64 dx, const b3_f64 dy);
	void b3ScaleView(const b3_f64 factor);
	void b3FullView();
	void b3PreviousView();

protected:
	void resizeEvent(QResizeEvent * event) override;
	void paintEvent(QPaintEvent * event) override;

private:
	void update();
	void update(QScrollBar * bar, const QB3BarInfo & info);

	QB3OpenGLWidget * child;
	QB3BarInfo        h, v;
};

#endif // QB3OPENGLSCROLLAREA_H
