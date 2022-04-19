#include "GraphicsEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GraphicsEditor w;
	w.show();
	return a.exec();
}
