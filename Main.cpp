#include "MailManager.h"
#include <iostream>

using namespace std;

int main() {
  // TODO: ����UI����
  // UI�������MailManager


  // ����
  MailManager mgr;
  mgr.SetCredential(CredentialInfo(MailAddress("example@a.com"), "password",
                                   "smtp.example.com", 123));
  mgr.SendMail(Mail("Test", "Test Content", MailAddress("example@a.com"), MailAddress("example@a.com")));
  return 0;
}