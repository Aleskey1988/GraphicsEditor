#pragma once

#include <QtWidgets>
#include "ui_BrushPattern.h"

class BrushPattern : public QLabel
{
	Q_OBJECT

public:
	BrushPattern(QPixmap pattern);
	~BrushPattern();

	void SetSelected(bool flag);
	bool IsSelected() { return isSelected; }

signals:
	void selected(QPixmap pixmap);

protected:
	virtual void mousePressEvent(QMouseEvent* event);

private:
	Ui::BrushPattern ui;
	QColor originalColor;
	bool isSelected = false;
};
