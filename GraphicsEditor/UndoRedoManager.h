#pragma once

#include <QtWidgets>
#include "Common.h"

class UndoRedoManager : public QObject
{
	Q_OBJECT

public:
	UndoRedoManager();
	~UndoRedoManager();

	void AddPixmap(QPixmap pixmap);
	void Undo();
	void Redo();

signals:
	void stateChanged(QPixmap pixmap, bool undo, bool redo);

private:
	QStack<QPixmap> left;
	QStack<QPixmap> right;
};
