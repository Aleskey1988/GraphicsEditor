#pragma once

#include <QtWidgets>

#include "ui_Canvas.h"
#include "Common.h"

class Canvas : public QLabel
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = Q_NULLPTR);
    ~Canvas();

	void SetBaseColor(QColor color);
	void SetBackgroundColor(QColor color);

	void SetPixmap(QPixmap pixmap);
	void SetTool(Tool tool) { this->tool = tool; }
	void SetBrushShape(QPixmap pixmap);

signals:
	void colorSelected(QColor color, Qt::MouseButton button);
	void canvasChanged(QPixmap pixmap);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:
	void onFill(QPoint pos, QColor colorFill);
	void erase(QPoint pos);
	void drawBrush(QPoint pos, Qt::MouseButtons buttons);
	void drawAirbrush();

	void updateCanvas();
	void setCurrentColor(QPainter& painter, Qt::MouseButtons buttons);

    Ui::Canvas ui;

	QImage canvas;
	QImage canvasCopy;
	QPoint prevPos;
	Qt::MouseButton currentButton;
	QTimer airbrushTimer;
	QPoint airbrushPos;
	QPixmap brushShapeBase;
	QPixmap brushShapeBackground;

	QColor baseColor = Qt::black;
	QColor backgroundColor = Qt::white;
	Tool tool;
};
