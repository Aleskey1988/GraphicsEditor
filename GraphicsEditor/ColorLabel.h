#pragma once

#include <QtWidgets>
#include "ui_ColorLabel.h"

class ColorLabel : public QLabel
{
	Q_OBJECT

public:
	ColorLabel(QWidget *parent = Q_NULLPTR);
	~ColorLabel();

signals:
	void clicked(QColor color, Qt::MouseButton button);

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private:
	Ui::ColorLabel ui;
};
