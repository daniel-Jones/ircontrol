#include "ircontrolwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ircontrolwindow w;
	w.show();

	return a.exec();
}
