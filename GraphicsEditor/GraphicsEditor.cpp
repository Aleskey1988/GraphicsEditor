#include "GraphicsEditor.h"

GraphicsEditor::GraphicsEditor(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.buttonPencil, &QRadioButton::clicked, this, &GraphicsEditor::onPencilClicked);
	connect(ui.buttonBrush, &QRadioButton::clicked, this, &GraphicsEditor::onBrushClicked);
	connect(ui.buttonLine, &QRadioButton::clicked, this, &GraphicsEditor::onLineClicked);
	connect(ui.buttonRectangle, &QRadioButton::clicked, this, &GraphicsEditor::onRectangleClicked);
	connect(ui.buttonEllipse, &QRadioButton::clicked, this, &GraphicsEditor::onEllipseClicked);
	connect(ui.buttonFill, &QRadioButton::clicked, this, &GraphicsEditor::onFillClicked);
	connect(ui.buttonEraser, &QRadioButton::clicked, this, &GraphicsEditor::onEraserClicked);
	connect(ui.buttonColorPicker, &QRadioButton::clicked, this, &GraphicsEditor::onColorPickerClicked);
	connect(ui.buttonAirbrush, &QRadioButton::clicked, this, &GraphicsEditor::onAirbrushClicked);

	connect(ui.canvas, &Canvas::colorSelected, this, &GraphicsEditor::onColorSelected);

	fillColors();

	onPencilClicked();
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
	tool = Tool::Pencil;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::UpArrowCursor);
}
void GraphicsEditor::onBrushClicked()
{
	tool = Tool::Brush;
	ui.canvas->SetTool(tool);
	QPixmap brushShape = getBrushShape(Qt::black);
	ui.canvas->setCursor(QCursor(brushShape,
		(brushSize.width() - 1) / 2, (brushSize.height() - 1) / 2));
}
void GraphicsEditor::onLineClicked()
{
	tool = Tool::Line;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onRectangleClicked()
{
	tool = Tool::Rectangle;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onEllipseClicked()
{
	tool = Tool::Ellipse;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onFillClicked()
{
	tool = Tool::Fill;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::PointingHandCursor);
}
void GraphicsEditor::onEraserClicked()
{
	tool = Tool::Eraser;
	ui.canvas->SetTool(tool);
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
	tool = Tool::ColorPicker;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::PointingHandCursor);
}
void GraphicsEditor::onAirbrushClicked()
{
	tool = Tool::Airbrush;
	ui.canvas->SetTool(tool);
	ui.canvas->setCursor(Qt::SizeAllCursor);
}
void GraphicsEditor::onColorSelected(QColor color, Qt::MouseButton button)
{
	if (button == Qt::LeftButton)
	{
		baseColor = color;
		QPalette palette = ui.labelBaseColor->palette();
		palette.setColor(QPalette::Window, color);
		ui.labelBaseColor->setPalette(palette);
		ui.canvas->SetBaseColor(baseColor);
	}
	else if (button == Qt::RightButton)
	{
		backgroundColor = color;
		QPalette palette = ui.labelBackgroundColor->palette();
		palette.setColor(QPalette::Window, color);
		ui.labelBackgroundColor->setPalette(palette);
		ui.canvas->SetBackgroundColor(backgroundColor);
	}
}
