#include "MailManager.h"
#include <iostream>
#include "ThirdParty/sqlite3/sqlite3.h"

using namespace std;

int main() {
    // TODO: ����UI����
    // UI�������MailManager


    // ������ȡ�ʼ�
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