#pragma once

#include <QtWidgets>
#include "ui_GraphicsEditor.h"
#include "Common.h"
#include "ColorLabel.h"
#include "BrushPattern.h"

class GraphicsEditor : public QMainWindow
{
	Q_OBJECT

public:
	GraphicsEditor(QWidget *parent = Q_NULLPTR);

private:
	void fillColors();
	void fillBrushPatterns();

	void onPencilClicked();
	void onBrushClicked();
	void onLineClicked();
	void onRectangleClicked();
	void onEllipseClicked();
	void onFillClicked();

	void onEraserClicked();
	void onColorPickerClicked();
	void onAirbrushClicked();

	void onColorSelected(QColor color, Qt::MouseButton button);
	void onBrushPatternSelected(QPixmap pixmap);

	Ui::GraphicsEditorClass ui;

	QColor baseColor = Qt::black;
	QColor backgroundColor = Qt::white;
	Tool tool;

	QList<BrushPattern*> brushPatterns;
};
