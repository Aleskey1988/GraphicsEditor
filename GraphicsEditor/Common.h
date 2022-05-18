#pragma once

#include <QtWidgets>

static QSize eraserSize = QSize(25, 25);
static QSize brushSize = QSize(11, 11);
static QSize airbrushSize = QSize(31, 31);

enum class Tool
{
	Pencil,
	Brush,
	Line,
	Rectangle,
	Ellipse,
	Airbrush,
	Fill,
	ColorPicker,
	Eraser,
};

static QPixmap getBrushShape(QColor color)
{
	QPixmap shape(brushSize);
	shape.fill(Qt::transparent);
	QPainter painter(&shape);
	painter.setPen(QPen(Qt::transparent, 0));
	painter.setBrush(color);
	painter.drawEllipse(0, 0, brushSize.width(), brushSize.height());
	return shape;
}