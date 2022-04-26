#include "GraphicsEditor.h"

GraphicsEditor::GraphicsEditor(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	canvas = QImage(640, 480, QImage::Format_ARGB32);
	canvas.fill(Qt::white);
	ui.canvas->setPixmap(QPixmap::fromImage(canvas));
}

void GraphicsEditor::mousePressEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	prevPos = pos;
	if (ui.buttonPencil->isChecked())
	{
		QPainter painter(&canvas);
		painter.drawPoint(pos);
		ui.canvas->setPixmap(QPixmap::fromImage(canvas));
	}
	else if (ui.buttonLine->isChecked() || ui.buttonRect->isChecked())
	{
		canvasCopy = canvas;
	}
}
void GraphicsEditor::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	if (ui.buttonPencil->isChecked())
	{
		QPainter painter(&canvas);
		painter.drawLine(pos, prevPos);
		prevPos = pos;
		ui.canvas->setPixmap(QPixmap::fromImage(canvas));
	}
	else if (ui.buttonLine->isChecked())
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.drawLine(pos, prevPos);
		ui.canvas->setPixmap(QPixmap::fromImage(canvas));
	}
	else if (ui.buttonRect->isChecked())
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.drawRect(QRect(pos, prevPos));
		ui.canvas->setPixmap(QPixmap::fromImage(canvas));
	}
}
void GraphicsEditor::mouseReleaseEvent(QMouseEvent* event)
{

}
