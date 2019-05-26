#pragma once
#include "CredentialInfo.h"
#include "Mail.h"
#include "Nullable.h"
#include "Property.h"
#include "ThirdParty/sqlite3/sqlite3.h"
#include <list>
using namespace std;

class MailManager {
  PROPERTY_WRITE(Nullable<CredentialInfo>, Credential)
  // TODO: �������ԣ�

private:
  sqlite3* db = nullptr;

public:
  MailManager();
  ~MailManager();
  // ��ȡ�����ʼ���д�����أ�SQLite����
  void FetchMails();
  list<Mail> ListMails() const; // �г����б��ص��ʼ�����UI��) TODO: ɸѡ������
  void SendMail(const Mail &mail) const;
  // TODO: ����
};