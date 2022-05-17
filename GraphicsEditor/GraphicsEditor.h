#pragma once

#include <QtWidgets>
#include "ui_GraphicsEditor.h"
#include "ColorLabel.h"

class GraphicsEditor : public QMainWindow
{
	Q_OBJECT

public:
	GraphicsEditor(QWidget *parent = Q_NULLPTR);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:
	void fillColors();

	void onPencilClicked();
	void onBrushClicked();
	void onLineClicked();
	void onRectClicked();
	void onEllipseClicked();
	void onFillClicked();

	void onEraserClicked();
	void onColorPickerClicked();
	void onAirbrushClicked();

	void onFill(QPoint pos);
	void erase(QPoint pos);
	QPixmap getBrushShape();
	void drawBrush(QPoint pos, Qt::MouseButtons buttons);
	void drawAirbrush();

	void updateCanvas();
	void setCurrentColor(QPainter& painter, Qt::MouseButtons buttons);
	void onColorSelected(QColor color, Qt::MouseButton button);

	Ui::GraphicsEditorClass ui;

	QImage canvas;
	QImage canvasCopy;
	QPoint prevPos;
	Qt::MouseButton currentButton;
	QStack<QPoint> fillStack;

	QSize eraserSize = QSize(25, 25);
	QSize brushSize = QSize(11, 11);
	QSize airbrushSize = QSize(31, 31);

	QTimer airbrushTimer;
	QPoint airbrushPos;

	QColor baseColor = Qt::black;
	QColor backgroundColor = Qt::white;
};
