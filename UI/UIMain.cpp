#include "UIMain.h"

int RunQt(int argc, char* argv[]) {

    mgr = new MailManager();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
