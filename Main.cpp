#include "UI/mainwindow.h"
#include "UI/send.h"
#include "UI/welcome.h"
#include "UI/inbox.h"
#include <QApplication>

int RunQt(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

int main(int argc, char *argv[])
{
  
    // test code
    MailManager mgr;
    ListCondition cond;
    cond.match_full = "jn";
    auto mails = mgr.ListMails(ListSource(), cond);

    return RunQt(argc, argv);

}
