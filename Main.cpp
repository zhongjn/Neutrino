#include "MailManager.h"
#include <iostream>
#include "ThirdParty/sqlite3/sqlite3.h"

using namespace std;

int main() {
    // TODO: ����UI����
    // UI�������MailManager


    // ����
    MailManager mgr;

    mgr.SetCredential(CredentialInfo(MailAddress("2389206378@qq.com"), "{private_key_here}",
        ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));
    // mgr.SendMail(Mail("Test", "Test Content", MailAddress("2389206378@qq.com"), MailAddress("2389206378@qq.com")));
    mgr.FetchMails();
    return 0;
}