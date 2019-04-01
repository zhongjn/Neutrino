#pragma once
#include <list>
#include "Mail.h"
#include "Credential.h"
using namespace std;

class MailManager {
	// TODO: 私有字段
public:
	void SetCredential(const Credential& cred);
	void FetchMails(); // 拉取在线邮件，写到本地（SQLite？）
	list<Mail> GetMails(); // 列出所有本地的邮件（给UI用) TODO: 筛选、检索
	void SendMail(const Mail& mail);
	// TODO: 其他
};