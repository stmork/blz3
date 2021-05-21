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
#include "src-gen/MouseSelect.h"

struct QB3BarInfo
{
	b3_f64  min;
	b3_f64  max;
	b3_f64  page_size;
	b3_f64  range;

	int  bar_page_size;
	int  bar_pos;
	int  bar_min;
	int  bar_max;
	bool bar_inv;

	void set(
		const b3_bool inverse,
		const b3_f32  scene_lower,
		const b3_f32  scene_upper,
		const b3_f32  view_lower,
		const b3_f32  view_upper);

	b3_f64 relFromBar(const int value) const;
	int    relToBar(const b3_f64 value) const;
	int    posToBar(const b3_f64 pos) const;
};

class QB3OpenGLScrollArea :
	public QScrollArea,
	protected MouseSelect,
	protected MouseSelect::View,
	protected MouseSelect::Gui,
	protected MouseSelect::Gui::OperationCallback,
	protected sc::rx::SingleSubscriptionObserver<void>
{
	Q_OBJECT

public:
	explicit QB3OpenGLScrollArea(QWidget * parent = nullptr);

	void setGlWidget(QB3OpenGLWidget * glWidget);
	void onSelect(bool checked);
	void b3SetViewMode(const b3_view_mode mode);
	void b3MoveView(const b3_f64 dx, const b3_f64 dy);
	void b3ScaleView(const b3_f64 factor);
	void b3FullView();
	void b3PreviousView();

signals:
	void selectionEnd();

private slots:
	void xValueChanged(int value);
	void yValueChanged(int value);
	void updateScrolling() override;

protected:
	void resizeEvent(QResizeEvent * event) override;
	void paintEvent(QPaintEvent * event) override;
	void mousePressEvent(QMouseEvent * event) override;
	void mouseMoveEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
	void next() override;

private:
	void update();
	void update(QScrollBar * bar, QB3BarInfo & info);
	static void b3GetBarInfo(
		const b3_view_info & info,
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);

	QB3OpenGLWidget * child = nullptr;
	QB3BarInfo        h, v;
};

#endif // QB3OPENGLSCROLLAREA_H
