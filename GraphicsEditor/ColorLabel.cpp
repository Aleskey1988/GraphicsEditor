#include "ColorLabel.h"

ColorLabel::ColorLabel(QWidget *parent)
	: QLabel(parent)
{
	ui.setupUi(this);
}
ColorLabel::~ColorLabel()
{
}

void ColorLabel::mousePressEvent(QMouseEvent* event)
{
	Qt::MouseButton button = event->button();
	QColor color = palette().color(QPalette::Window);
	emit clicked(color, button);
}
