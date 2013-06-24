#include "stdafx.h"
#include "DumbObfuscator.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DumbObfuscator w;
	w.show();
	return a.exec();
}
