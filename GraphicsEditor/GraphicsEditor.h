#pragma once

#include <QtWidgets>
#include "ui_GraphicsEditor.h"

class GraphicsEditor : public QMainWindow
{
	Q_OBJECT

public:
	GraphicsEditor(QWidget *parent = Q_NULLPTR);

private:
	Ui::GraphicsEditorClass ui;

	void onDraw();
};
