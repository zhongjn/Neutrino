#pragma once
#include <list>
#include "Mail.h"
#include "Credential.h"
using namespace std;

class MailManager {
	// TODO: ˽���ֶ�
public:
	void SetCredential(const Credential& cred);
	void FetchMails(); // ��ȡ�����ʼ���д�����أ�SQLite����
	list<Mail> GetMails(); // �г����б��ص��ʼ�����UI��) TODO: ɸѡ������
	void SendMail(const Mail& mail);
	// TODO: ����
};