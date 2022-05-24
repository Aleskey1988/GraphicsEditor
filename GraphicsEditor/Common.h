#pragma once

#include <QtWidgets>

static QSize eraserSize = QSize(25, 25);
static QSize brushSize = QSize(11, 11);
static QSize airbrushSize = QSize(31, 31);
static QSize brushPatternSize = QSize(19, 19);

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
