/*
#include "MailManager.h"
#include <iostream>
#include "ThirdParty/sqlite3/sqlite3.h"

using namespace std;

int main() {
    // TODO: 进入UI函数
    // UI负责操作MailManager


    // 测试拉取邮件
    MailManager mgr;

    mgr.SetCredential(CredentialInfo(MailAddress("2389206378@qq.com"), "{your_private_key}",
        ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));
    mgr.FetchMails();
    cout << "-------------------" << endl;
    auto mails = mgr.ListMails();
    for (auto& m : mails) {
        cout << m.GetSubject() << endl;
    }
    system("pause");
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
