#include "BrushPattern.h"

BrushPattern::BrushPattern(QPixmap pattern)
	: QLabel(nullptr)
{
	ui.setupUi(this);

	originalColor = palette().color(QPalette::Window);
	setAlignment(Qt::AlignCenter);
	setPixmap(pattern);
}
BrushPattern::~BrushPattern()
{
}

void BrushPattern::SetSelected(bool flag)
{
	isSelected = flag;
	QPalette pal = palette();
	if (flag)
	{
		pal.setColor(QPalette::Window, QColor(196, 196, 255));
	}
	else
	{
		pal.setColor(QPalette::Window, originalColor);
	}
	setPalette(pal);
}

void BrushPattern::mousePressEvent(QMouseEvent* event)
{
	emit selected(*pixmap());
	SetSelected(true);
}
