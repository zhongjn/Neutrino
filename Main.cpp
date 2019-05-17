#include "MailManager.h"
#include <iostream>
#include "ThirdParty/sqlite3/sqlite3.h"

using namespace std;

int main() {
  // TODO: ����UI����
  // UI�������MailManager


  // ����
  MailManager mgr;

  mgr.SetCredential(CredentialInfo(MailAddress("example@a.com"), "password",
                                   ServerEndPoint("smtp.example.com", 123), ServerEndPoint("pop.example.com", 123)));
  mgr.SendMail(Mail("Test", "Test Content", MailAddress("example@a.com"), MailAddress("example@a.com")));
  return 0;
}