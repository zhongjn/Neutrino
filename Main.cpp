#include "UI/mainwindow.h"
#include "UI/send.h"
#include "UI/inbox.h"
#include <QApplication>
#include <QTextCodec>
#include "CryptoProvider.h"
#include <iostream>

int RunQt(int argc, char* argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

int main(int argc, char *argv[])
{
    //string origin = "test string here";
    //CryptoProvider* crypto = new Win32CryptoProvider();
    //string im = crypto->Encrypt(origin);
    //cout << im << endl;
    //string res = crypto->Decrypt(im);
    //cout << res << endl;



    // test code
    //{
    //    MailManager mgr;
    //    bool b = mgr.IsLoginNeeded();
    //    mgr.Login(CredentialInfo(MailAddress("610223674@qq.com"), "ycagpzuevtubbbee",
    //        ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)), true);
    //}

    //{
    //    MailManager mgr;
    //    bool b = mgr.IsLoginNeeded();
    //   mgr.FetchMails();
    //}


	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    return RunQt(argc, argv);

}
