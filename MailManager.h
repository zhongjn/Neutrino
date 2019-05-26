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
  // TODO: 其他属性？

private:
  sqlite3* db = nullptr;

public:
  MailManager();
  ~MailManager();
  // 拉取在线邮件，写到本地（SQLite？）
  void FetchMails();
  list<Mail> ListMails() const; // 列出所有本地的邮件（给UI用) TODO: 筛选、检索
  void SendMail(const Mail &mail) const;
  // TODO: 其他
};