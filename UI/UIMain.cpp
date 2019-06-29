#include "UIMain.h"

int RunQt(int argc, char* argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    mgr = new MailManager();
	
    QApplication a(argc, argv);
	
    MainWindow w;
    w.show();

    return a.exec();
}
