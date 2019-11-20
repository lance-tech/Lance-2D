#include "Lance2D.h"
#include <QtWidgets/QApplication>

#include <QFile>
#include <QTextStream>
#include "Lance2D.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QFile cssFile("Resources/main.css");
	cssFile.open(QFile::ReadOnly | QFile::Text);
	QTextStream cssStream(&cssFile);
	QString cssText = cssStream.readAll();

	Lance2D engine;
	engine.show();
	engine.setStyleSheet(cssText);

	return a.exec();
}
