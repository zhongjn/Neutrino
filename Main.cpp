#include "CryptoProvider.h"
#include <iostream>
#include "UI/UIMain.h"

//int RunQt(int argc, char* argv[]) {
//
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//
//    return a.exec();
//}

int main(int argc, char *argv[])
{
    string origin = "test string here";
    CryptoProvider* crypto = new Win32CryptoProvider();
    string im = crypto->Encrypt(origin);
    im = "asdasidj";
    cout << im << endl;
    string res = crypto->Decrypt(im);
    cout << res << endl;



    // test code
    //{
    //    MailManager mgr;
    //    bool b = mgr.IsLoginNeeded();
    //    if (b) mgr.Login(CredentialInfo(MailAddress("610223674@qq.com"), "ycagpzuevtubbbee",
    //        ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)), true);

    //    mgr.FetchMails();
    //}

    //{
    //    MailManager mgr;
    //    bool b = mgr.IsLoginNeeded();
    //   mgr.FetchMails();
    //}


    return RunQt(argc, argv);

}
