#include "UndoRedoManager.h"

UndoRedoManager::UndoRedoManager()
{}

UndoRedoManager::~UndoRedoManager()
{}

void UndoRedoManager::AddPixmap(QPixmap pixmap)
{
    left.push(pixmap);
    right.clear();
    bool undo = !left.isEmpty();
    bool redo = !right.isEmpty();
    emit stateChanged(QPixmap(), undo, redo);
}
void UndoRedoManager::Undo()
{
    if (!left.isEmpty())
    {
        QPixmap leftPixmap = left.pop();
        right.push(leftPixmap);
        bool undo = !left.isEmpty();
        bool redo = !right.isEmpty();
        QPixmap lastPixmap;
        if (!left.isEmpty())
            lastPixmap = left.last();
        else
        {
            lastPixmap = QPixmap(canvasSize);
            lastPixmap.fill(Qt::white);
        }
        emit stateChanged(lastPixmap, undo, redo);
    }
}
void UndoRedoManager::Redo()
{
    QPixmap pixmap;
    if (!right.isEmpty())
    {
        pixmap = right.pop();
        left.push(pixmap);
        bool undo = !left.isEmpty();
        bool redo = !right.isEmpty();
        emit stateChanged(pixmap, undo, redo);
    }
}
