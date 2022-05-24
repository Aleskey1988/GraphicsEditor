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
	fillBrushPatterns();

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
void GraphicsEditor::fillBrushPatterns()
{
	QVector<QPixmap> patterns;
	// p1
	QPixmap p1(11, 11);
	p1.fill(Qt::transparent);
	QPainter p(&p1);
	p.setBrush(Qt::black);
	p.drawEllipse(0, 0, p1.width() - 1, p1.height() - 1);
	p.end();
	patterns << p1;
	// p2
	QPixmap p2(7, 7);
	p2.fill(Qt::transparent);
	p.begin(&p2);
	p.setBrush(Qt::black);
	p.drawEllipse(0, 0, p2.width() - 1, p2.height() - 1);
	p.end();
	patterns << p2;
	// p3
	QPixmap p3(3, 3);
	p3.fill(Qt::transparent);
	p.begin(&p3);
	p.setBrush(Qt::black);
	p.drawEllipse(0, 0, p3.width() - 1, p3.height() - 1);
	p.end();
	patterns << p3;
	// p4
	QPixmap p4(11, 11);
	p4.fill(Qt::transparent);
	p.begin(&p4);
	p.setBrush(Qt::black);
	p.drawRect(0, 0, p4.width() - 1, p4.height() - 1);
	p.end();
	patterns << p4;
	// p5
	QPixmap p5(7, 7);
	p5.fill(Qt::transparent);
	p.begin(&p5);
	p.setBrush(Qt::black);
	p.drawRect(0, 0, p5.width() - 1, p5.height() - 1);
	p.end();
	patterns << p5;
	// p6
	QPixmap p6(3, 3);
	p6.fill(Qt::transparent);
	p.begin(&p6);
	p.setBrush(Qt::black);
	p.drawRect(0, 0, p6.width() - 1, p6.height() - 1);
	p.end();
	patterns << p6;
	// p7
	QPixmap p7(11, 11);
	p7.fill(Qt::transparent);
	p.begin(&p7);
	p.drawLine(0, 0, p7.width() - 1, p7.height() - 1);
	p.end();
	patterns << p7;
	// p8
	QPixmap p8(7, 7);
	p8.fill(Qt::transparent);
	p.begin(&p8);
	p.drawLine(0, 0, p8.width() - 1, p8.height() - 1);
	p.end();
	patterns << p8;
	// p9
	QPixmap p9(3, 3);
	p9.fill(Qt::transparent);
	p.begin(&p9);
	p.drawLine(0, 0, p9.width() - 1, p9.height() - 1);
	p.end();
	patterns << p9;
	// p10
	QPixmap p10(11, 11);
	p10.fill(Qt::transparent);
	p.begin(&p10);
	p.drawLine(p10.width() - 1, 0, 0, p10.height() - 1);
	p.end();
	patterns << p10;
	// p11
	QPixmap p11(7, 7);
	p11.fill(Qt::transparent);
	p.begin(&p11);
	p.drawLine(p11.width() - 1, 0, 0, p11.height() - 1);
	p.end();
	patterns << p11;
	// p12
	QPixmap p12(3, 3);
	p12.fill(Qt::transparent);
	p.begin(&p12);
	p.drawLine(p12.width() - 1, 0, 0, p12.height() - 1);
	p.end();
	patterns << p12;
	// add to layout
	for (int i = 0; i < patterns.size(); ++i)
	{
		int x = i % 3;
		int y = i / (patterns.size() / 3 - 1);
		BrushPattern* pattern = new BrushPattern(patterns[i]);
		pattern->setFixedSize(brushPatternSize);
		ui.layoutBrushPatterns->addWidget(pattern, y, x, Qt::AlignCenter);
		connect(pattern, &BrushPattern::selected, this, &GraphicsEditor::onBrushPatternSelected);
		brushPatterns << pattern;
	}
	// set default pattern
	brushPatterns.first()->SetSelected(true);

}
void GraphicsEditor::onPencilClicked()
{
	tool = Tool::Pencil;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::UpArrowCursor);
}
void GraphicsEditor::onBrushClicked()
{
	tool = Tool::Brush;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(true);
	for (auto pattern : brushPatterns)
	{
		if (pattern->IsSelected())
		{
			QPixmap pixmap = *pattern->pixmap();
			ui.canvas->setCursor(QCursor(pixmap, (brushSize.width() - 1) / 2, (brushSize.height() - 1) / 2));
			ui.canvas->SetBrushShape(pixmap);
			break;
		}
	}
}
void GraphicsEditor::onLineClicked()
{
	tool = Tool::Line;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onRectangleClicked()
{
	tool = Tool::Rectangle;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onEllipseClicked()
{
	tool = Tool::Ellipse;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::CrossCursor);
}
void GraphicsEditor::onFillClicked()
{
	tool = Tool::Fill;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::PointingHandCursor);
}
void GraphicsEditor::onEraserClicked()
{
	tool = Tool::Eraser;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
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
	ui.groupBrushPatterns->setVisible(false);
	ui.canvas->setCursor(Qt::PointingHandCursor);
}
void GraphicsEditor::onAirbrushClicked()
{
	tool = Tool::Airbrush;
	ui.canvas->SetTool(tool);
	ui.groupBrushPatterns->setVisible(false);
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
void GraphicsEditor::onBrushPatternSelected(QPixmap pixmap)
{
	for (auto pattern : brushPatterns)
		pattern->SetSelected(false);
	ui.canvas->SetBrushShape(pixmap);
	ui.canvas->setCursor(QCursor(pixmap, (brushSize.width() - 1) / 2, (brushSize.height() - 1) / 2));

}
