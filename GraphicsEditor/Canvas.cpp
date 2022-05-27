#include "Canvas.h"

Canvas::Canvas(QWidget *parent)
    : QLabel(parent)
{
    ui.setupUi(this);

	connect(&airbrushTimer, &QTimer::timeout, this, &Canvas::drawAirbrush);

	canvas = QImage(canvasSize, QImage::Format_ARGB32);
	canvas.fill(Qt::white);
	updateCanvas();
}
Canvas::~Canvas()
{
}

void Canvas::SetBaseColor(QColor color)
{
	baseColor = color;
	// colorize pixmap for drawing
	QPixmap drawingPixmap(brushShapeBase.size());
	drawingPixmap.fill(color);
	drawingPixmap.setMask(brushShapeBase.createMaskFromColor(Qt::transparent));
	brushShapeBase = drawingPixmap;
}
void Canvas::SetBackgroundColor(QColor color)
{
	backgroundColor = color;
	// colorize pixmap for drawing
	QPixmap drawingPixmap(brushShapeBackground.size());
	drawingPixmap.fill(color);
	drawingPixmap.setMask(brushShapeBackground.createMaskFromColor(Qt::transparent));
	brushShapeBackground = drawingPixmap;
}
void Canvas::SetPixmap(QPixmap pixmap)
{
	canvas = pixmap.toImage();
	updateCanvas();
}
void Canvas::SetBrushShape(QPixmap pixmap)
{
	brushShapeBase = pixmap;
	brushShapeBackground = pixmap;
	SetBaseColor(baseColor);
	SetBackgroundColor(backgroundColor);
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	prevPos = pos;
	if (tool == Tool::Pencil)
	{
		QPainter painter(&canvas);
		setCurrentColor(painter, event->button());
		painter.drawPoint(pos);
		updateCanvas();
	}
	else if (tool == Tool::Line || tool == Tool::Rectangle || tool == Tool::Ellipse)
	{
		canvasCopy = canvas;
	}
	else if (tool == Tool::Brush)
	{
		drawBrush(pos, event->button());
		updateCanvas();
	}
	else if (tool == Tool::Airbrush)
	{
		airbrushPos = pos;
		currentButton = event->button();
		airbrushTimer.start(10);
	}
	else if (tool == Tool::Fill)
	{
		QColor color;
		if (event->button() == Qt::LeftButton)
			color = baseColor;
		else if (event->button() == Qt::RightButton)
			color = backgroundColor;
		onFill(pos, color);
	}
	else if (tool == Tool::ColorPicker)
	{
		QColor color = canvas.pixelColor(pos);
		emit colorSelected(color, event->button());
	}
	else if (tool == Tool::Eraser)
	{
		erase(pos);
	}
}
void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	if (tool == Tool::Pencil)
	{
		QPainter painter(&canvas);
		setCurrentColor(painter, event->buttons());
		painter.drawLine(pos, prevPos);
		prevPos = pos;
		updateCanvas();
	}
	else if (tool == Tool::Line)
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		setCurrentColor(painter, event->buttons());
		painter.drawLine(pos, prevPos);
		updateCanvas();
	}
	else if (tool == Tool::Brush)
	{
		drawBrush(pos, event->buttons());
		updateCanvas();
	}
	else if (tool == Tool::Rectangle)
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.setPen(baseColor);
		painter.setBrush(backgroundColor);
		painter.drawRect(QRect(pos, prevPos));
		updateCanvas();
	}
	else if (tool == Tool::Ellipse)
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.setPen(baseColor);
		painter.setBrush(backgroundColor);
		painter.drawEllipse(QRect(pos, prevPos));
		updateCanvas();
	}
	else if (tool == Tool::Airbrush)
	{
		airbrushPos = pos;
	}
	else if (tool == Tool::Eraser)
	{
		erase(pos);
	}
}
void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (tool == Tool::Airbrush)
	{
		airbrushTimer.stop();
	}
	if (tool != Tool::ColorPicker)
	{
		emit canvasChanged(QPixmap::fromImage(canvas));
	}
}

void Canvas::onFill(QPoint pos, QColor colorFill)
{
	QColor colorReplace = canvas.pixelColor(pos);
	if (colorReplace != colorFill)
	{
		QStack<QPoint> stack;
		stack.push(pos);
		while (!stack.empty())
		{
			QPoint currectPos = stack.pop();
			canvas.setPixelColor(currectPos, colorFill);

			QPoint left(currectPos.x() - 1, currectPos.y());
			QPoint right(currectPos.x() + 1, currectPos.y());
			QPoint top(currectPos.x(), currectPos.y() - 1);
			QPoint bottom(currectPos.x(), currectPos.y() + 1);
			QColor leftPixel = canvas.pixelColor(left);
			QColor rightPixel = canvas.pixelColor(right);
			QColor topPixel = canvas.pixelColor(top);
			QColor bottomPixel = canvas.pixelColor(bottom);
			if (leftPixel == colorReplace)
				stack.push(left);
			if (rightPixel == colorReplace)
				stack.push(right);
			if (topPixel == colorReplace)
				stack.push(top);
			if (bottomPixel == colorReplace)
				stack.push(bottom);
		}
		updateCanvas();
	}
}
void Canvas::erase(QPoint pos)
{
	QPainter painter(&canvas);
	painter.setPen(QPen(Qt::transparent, 0));
	painter.setBrush(Qt::white);
	QRect r(pos.x() - (eraserSize.width() - 1) / 2,
		pos.y() - (eraserSize.height() - 1) / 2,
		eraserSize.width() - 1,
		eraserSize.height() - 1);
	painter.drawRect(r);
	updateCanvas();
}
void Canvas::drawBrush(QPoint pos, Qt::MouseButtons buttons)
{
	QPixmap brushShape;
	if (buttons == Qt::LeftButton)
		brushShape = brushShapeBase;
	else if (buttons == Qt::RightButton)
		brushShape = brushShapeBackground;
	QPainter painter(&canvas);
	QPoint centerPos(pos.x() - brushSize.width() / 2, pos.y() - brushSize.height() / 2);
	painter.drawPixmap(centerPos, brushShape);
}
void Canvas::drawAirbrush()
{
	QPainter painter(&canvas);
	setCurrentColor(painter, currentButton);
	for (int i = 0; i < 10; i++)
	{
		int x = 0;
		int y = 0;
		int radius = airbrushSize.width() / 2 + 1;
		do
		{
			x = qrand() % airbrushSize.width() - airbrushSize.width() / 2;
			y = qrand() % airbrushSize.height() - airbrushSize.height() / 2;
		}
		while (qSqrt(x * x + y * y) > radius);
		QPoint pos(x, y);
		painter.drawPoint(airbrushPos + pos);
	}
	updateCanvas();
}

void Canvas::updateCanvas()
{
	setPixmap(QPixmap::fromImage(canvas));
}
void Canvas::setCurrentColor(QPainter& painter, Qt::MouseButtons buttons)
{
	QPen pen = painter.pen();
	if (buttons == Qt::LeftButton)
	{
		pen.setColor(baseColor);
	}
	else if (buttons == Qt::RightButton)
	{
		pen.setColor(backgroundColor);
	}
	painter.setPen(pen);
}