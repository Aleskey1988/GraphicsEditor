#include "GraphicsEditor.h"

GraphicsEditor::GraphicsEditor(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.buttonPencil, &QRadioButton::clicked, this, &GraphicsEditor::onPencilClicked);
	connect(ui.buttonBrush, &QRadioButton::clicked, this, &GraphicsEditor::onBrushClicked);
	connect(ui.buttonLine, &QRadioButton::clicked, this, &GraphicsEditor::onLineClicked);
	connect(ui.buttonRect, &QRadioButton::clicked, this, &GraphicsEditor::onRectClicked);
	connect(ui.buttonEllipse, &QRadioButton::clicked, this, &GraphicsEditor::onEllipseClicked);
	connect(ui.buttonFill, &QRadioButton::clicked, this, &GraphicsEditor::onFillClicked);
	connect(ui.buttonEraser, &QRadioButton::clicked, this, &GraphicsEditor::onEraserClicked);
	connect(ui.buttonAirbrush, &QRadioButton::clicked, this, &GraphicsEditor::onAirbrushClicked);

	connect(&airbrushTimer, &QTimer::timeout, this, &GraphicsEditor::drawAirbrush);

	fillColors();

	canvas = QImage(640, 480, QImage::Format_ARGB32);
	canvas.fill(Qt::white);
	updateCanvas();
	onPencilClicked();
}

void GraphicsEditor::mousePressEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	prevPos = pos;
	if (ui.buttonPencil->isChecked())
	{
		QPainter painter(&canvas);
		setCurrentColor(painter, event->button());
		painter.drawPoint(pos);
		updateCanvas();
	}
	else if (ui.buttonLine->isChecked() || ui.buttonRect->isChecked() || ui.buttonEllipse->isChecked())
	{
		canvasCopy = canvas;
	}
	else if (ui.buttonBrush->isChecked())
	{
		drawBrush(pos, event->button());
		updateCanvas();
	}
	else if (ui.buttonAirbrush->isChecked())
	{
		airbrushPos = pos;
		currentButton = event->button();
		airbrushTimer.start(10);
	}
	else if (ui.buttonFill->isChecked())
	{
		onFill(pos);
	}
	else if (ui.buttonColorPicker->isChecked())
	{
		QColor color = canvas.pixelColor(pos);
		onColorSelected(color, event->button());
	}
	else if (ui.buttonEraser->isChecked())
	{
		erase(pos);
	}
}
void GraphicsEditor::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	if (ui.buttonPencil->isChecked())
	{
		QPainter painter(&canvas);
		setCurrentColor(painter, event->buttons());
		painter.drawLine(pos, prevPos);
		prevPos = pos;
		updateCanvas();
	}
	else if (ui.buttonLine->isChecked())
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		setCurrentColor(painter, event->buttons());
		painter.drawLine(pos, prevPos);
		updateCanvas();
	}
	else if (ui.buttonBrush->isChecked())
	{
		drawBrush(pos, event->button());
		updateCanvas();
	}
	else if (ui.buttonRect->isChecked())
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.setPen(baseColor);
		painter.setBrush(backgroundColor);
		painter.drawRect(QRect(pos, prevPos));
		updateCanvas();
	}
	else if (ui.buttonEllipse->isChecked())
	{
		canvas = canvasCopy;
		QPainter painter(&canvas);
		painter.setPen(baseColor);
		painter.setBrush(backgroundColor);
		painter.drawEllipse(QRect(pos, prevPos));
		updateCanvas();
	}
	else if (ui.buttonAirbrush->isChecked())
	{
		airbrushPos = pos;
	}
	else if (ui.buttonEraser->isChecked())
	{
		erase(pos);
	}
}
void GraphicsEditor::mouseReleaseEvent(QMouseEvent* event)
{
	if (ui.buttonAirbrush->isChecked())
	{
		airbrushTimer.stop();
	}
}

void GraphicsEditor::fillColors()
{
	QList<QColor> colors;
	colors << QColor(0, 0, 0) << QColor(128, 128, 128) <<
		QColor(136, 0, 21) << QColor(138, 153, 119) <<
		QColor(0, 128, 0) << QColor(255, 255, 255) <<
		QColor(0, 0, 255) << QColor(255, 0, 0) <<
		QColor(255, 255, 0) << QColor(255, 0, 255);

	for (int i = 0; i < colors.size(); ++i)
	{
		int x = i % (colors.size() / 2);
		int y = i / (colors.size() / 2);
		ColorLabel* label = new ColorLabel();
		QPalette palette = label->palette();
		palette.setColor(QPalette::Window, colors[i]);
		label->setPalette(palette);
		label->setAutoFillBackground(true);
		label->setFixedSize(21, 21);
		ui.colors->addWidget(label, y, x);
		connect(label, &ColorLabel::clicked, this, &GraphicsEditor::onColorSelected);
	}
}
void GraphicsEditor::onPencilClicked()
{
	ui.canvas->setCursor(Qt::UpArrowCursor);
}
void GraphicsEditor::onBrushClicked()
{
	QPixmap brushShape = getBrushShape();
	ui.canvas->setCursor(QCursor(brushShape,
		(brushSize.width() - 1) / 2, (brushSize.height() - 1) / 2));
}
void GraphicsEditor::onLineClicked()
{
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onRectClicked()
{
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onEllipseClicked()
{
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onFillClicked()
{
	ui.canvas->setCursor(Qt::PointingHandCursor);
}
void GraphicsEditor::onEraserClicked()
{
	QPixmap eraserCursor(eraserSize);
	eraserCursor.fill(Qt::white);
	QPainter painter(&eraserCursor);
	painter.setPen(QPen(Qt::black, 1));
	painter.setBrush(Qt::white);
	painter.drawRect(0, 0, eraserSize.width() - 1, eraserSize.height() - 1);
	ui.canvas->setCursor(QCursor(eraserCursor,
		(eraserSize.width() - 1) / 2, (eraserSize.height() - 1) / 2));
}
void GraphicsEditor::onColorPickerClicked()
{

}
void GraphicsEditor::onAirbrushClicked()
{

}

void GraphicsEditor::onFill(QPoint pos)
{
	QColor color = canvas.pixelColor(pos);
	if (color != baseColor)
	{
		fillStack.push(pos);
		while (!fillStack.empty())
		{
			QPoint currectPos = fillStack.pop();
			canvas.setPixelColor(currectPos, baseColor);

			QPoint left(currectPos.x() - 1, currectPos.y());
			QPoint right(currectPos.x() + 1, currectPos.y());
			QPoint top(currectPos.x(), currectPos.y() - 1);
			QPoint bottom(currectPos.x(), currectPos.y() + 1);
			QColor leftPixel = canvas.pixelColor(left);
			QColor rightPixel = canvas.pixelColor(right);
			QColor topPixel = canvas.pixelColor(top);
			QColor bottomPixel = canvas.pixelColor(bottom);
			if (leftPixel == color)
				fillStack.push(left);
			if (rightPixel == color)
				fillStack.push(right);
			if (topPixel == color)
				fillStack.push(top);
			if (bottomPixel == color)
				fillStack.push(bottom);
		}
		updateCanvas();
	}
}
void GraphicsEditor::erase(QPoint pos)
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
QPixmap GraphicsEditor::getBrushShape()
{
	QPixmap shape(brushSize);
	shape.fill(Qt::transparent);
	QPainter painter(&shape);
	painter.setPen(QPen(Qt::transparent, 0));
	painter.setBrush(baseColor);
	painter.drawEllipse(0, 0, brushSize.width(), brushSize.height());
	return shape;
}
void GraphicsEditor::drawBrush(QPoint pos, Qt::MouseButtons buttons)
{
	QPixmap shape = getBrushShape();
	QPainter painter(&canvas);
	setCurrentColor(painter, buttons);
	QPoint centerPos(pos.x() - brushSize.width() / 2, pos.y() - brushSize.height() / 2);
	painter.drawPixmap(centerPos, shape);
}
void GraphicsEditor::drawAirbrush()
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

void GraphicsEditor::updateCanvas()
{
	ui.canvas->setPixmap(QPixmap::fromImage(canvas));
}
void GraphicsEditor::setCurrentColor(QPainter& painter, Qt::MouseButtons buttons)
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
void GraphicsEditor::onColorSelected(QColor color, Qt::MouseButton button)
{
	if (button == Qt::LeftButton)
	{
		baseColor = color;
		QPalette palette = ui.labelBaseColor->palette();
		palette.setColor(QPalette::Window, color);
		ui.labelBaseColor->setPalette(palette);
	}
	else if (button == Qt::RightButton)
	{
		backgroundColor = color;
		QPalette palette = ui.labelBackgroundColor->palette();
		palette.setColor(QPalette::Window, color);
		ui.labelBackgroundColor->setPalette(palette);
	}
}
