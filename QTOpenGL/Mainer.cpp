#include "MetapodGLWindow.h"
#include <QtWidgets\qapplication.h>

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	MetapodGLWindow myApp;

	myApp.show();

	return app.exec();
}