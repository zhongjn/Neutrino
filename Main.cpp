/*
#include "MailManager.h"
#include <iostream>
#include "ThirdParty/sqlite3/sqlite3.h"

using namespace std;

int main() {
    // TODO: 进入UI函数
    // UI负责操作MailManager
    MailManager mgr;

    return 0;
}
*/
#include "UI/mainwindow.h"
#include "UI/send.h"
#include "UI/welcome.h"
#include "UI/inbox.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
