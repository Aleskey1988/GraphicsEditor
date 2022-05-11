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
        painter.drawPoint(pos);
        updateCanvas();
    }
    else if (ui.buttonLine->isChecked() || ui.buttonRect->isChecked() || ui.buttonEllipse->isChecked())
    {
        canvasCopy = canvas;
    }
    else if (ui.buttonBrush->isChecked())
    {
        drawBrush(pos);
        updateCanvas();
    }
    else if (ui.buttonAirbrush->isChecked())
    {
        airbrushPos = pos;
        airbrushTimer.start(1);
    }
    else if (ui.buttonFill->isChecked())
    {
        onFill(pos);
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
        painter.drawLine(pos, prevPos);
        prevPos = pos;
        updateCanvas();
    }
    else if (ui.buttonLine->isChecked())
    {
        canvas = canvasCopy;
        QPainter painter(&canvas);
        painter.drawLine(pos, prevPos);
        updateCanvas();
    }
    else if (ui.buttonBrush->isChecked())
    {
        drawBrush(pos);
        updateCanvas();
    }
    else if (ui.buttonRect->isChecked())
    {
        canvas = canvasCopy;
        QPainter painter(&canvas);
        painter.drawRect(QRect(pos, prevPos));
        updateCanvas();
    }
    else if (ui.buttonEllipse->isChecked())
    {
        canvas = canvasCopy;
        QPainter painter(&canvas);
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
void GraphicsEditor::onAirbrushClicked()
{

}

void GraphicsEditor::onFill(QPoint pos)
{
    QColor color = canvas.pixelColor(pos);
    fillStack.push(pos);
    while (!fillStack.empty())
    {
        QPoint currectPos = fillStack.pop();
        canvas.setPixelColor(currectPos, Qt::black);

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
    QPixmap brushShape(brushSize);
    brushShape.fill(Qt::transparent);
    QPainter painterShape(&brushShape);
    painterShape.setPen(QPen(Qt::transparent, 0));
    painterShape.setBrush(Qt::black);
    painterShape.drawEllipse(0, 0, brushSize.width(), brushSize.height());
    return brushShape;
}
void GraphicsEditor::drawBrush(QPoint pos)
{
    QPixmap brushShape = getBrushShape();
    QPainter painterBrush(&canvas);
    QPoint centerPos(pos.x() - brushSize.width() / 2, pos.y() - brushSize.height() / 2);
    painterBrush.drawPixmap(centerPos, brushShape);
}
void GraphicsEditor::drawAirbrush()
{
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < 2; i++)
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
