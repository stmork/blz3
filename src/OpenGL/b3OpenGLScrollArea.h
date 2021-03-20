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

struct QB3BarInfo
{
	static const int BAR_RANGE_MIN =     0;
	static const int BAR_RANGE_MAX = 16384;
	static const int BAR_RANGE     = BAR_RANGE_MAX - BAR_RANGE_MIN;

	b3_f64 min;
	b3_f64 max;
	b3_f64 page_size;
	b3_f64 page_pos;
	b3_f64 scale;

	int    bar_page_size;
	int    bar_pos;

	void set(
		const b3_f32 scene_lower,
		const b3_f32 scene_upper,
		const b3_f32 view_lower,
		const b3_f32 view_upper);

	b3_f64 relFromBar(const int value) const;
	int    relToBar(const b3_f64 value) const;
	int    posToBar(const b3_f64 pos);
};

class QB3OpenGLScrollArea : public QScrollArea
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
	void update(QScrollBar * bar, QB3BarInfo & info, b3_bool negate = false);

	QB3OpenGLWidget * child;
	QB3BarInfo        h, v;
};

#endif // QB3OPENGLSCROLLAREA_H
