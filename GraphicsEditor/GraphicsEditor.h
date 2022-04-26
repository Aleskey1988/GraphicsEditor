#pragma once

#include <QtWidgets>
#include "ui_GraphicsEditor.h"

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
	Ui::GraphicsEditorClass ui;

	QImage canvas;
	QImage canvasCopy;
	QPoint prevPos;
};
