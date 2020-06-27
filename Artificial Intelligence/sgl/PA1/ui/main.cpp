#include "FiveChess.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FiveChess w;
	w.show();
	return a.exec();
}
