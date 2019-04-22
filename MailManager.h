#pragma once
#include "CredentialInfo.h"
#include "Mail.h"
#include "Nullable.h"
#include "Property.h"
#include <list>
using namespace std;

class MailManager {
  PROPERTY_WRITE(Nullable<CredentialInfo>, Credential)
  // TODO: �������ԣ�

public:
  MailManager() = default;

  // ��ȡ�����ʼ���д�����أ�SQLite����
  void FetchMails();

  list<Mail> ListMails() const; // �г����б��ص��ʼ�����UI��) TODO: ɸѡ������

  void SendMail(const Mail &mail) const;

  // TODO: ����
};