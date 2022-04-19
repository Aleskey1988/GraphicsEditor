#include "GraphicsEditor.h"

GraphicsEditor::GraphicsEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.buttonDraw, &QPushButton::clicked, this, &GraphicsEditor::onDraw);
}

void GraphicsEditor::onDraw()
{
	QImage image(320, 240, QImage::Format_ARGB32);
	image.fill(Qt::white);
	QPainter p(&image);
	QPen pen(Qt::blue, 5, Qt::PenStyle::DashLine, Qt::PenCapStyle::RoundCap);
	p.setPen(pen);
	p.drawLine(160, 10, 240, 100);
	pen = QPen(Qt::red, 10, Qt::PenStyle::DotLine, Qt::PenCapStyle::SquareCap);
	p.setPen(pen);
	p.drawLine(240, 100, 80, 100);
	pen = QPen(Qt::green, 7.5, Qt::PenStyle::DashDotLine, Qt::PenCapStyle::FlatCap);
	p.setPen(pen);
	p.drawLine(80, 100, 160, 10);
	ui.label->setPixmap(QPixmap::fromImage(image));
}
