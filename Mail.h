#pragma once
#include <string>
#include "MailAddress.h"
using namespace std;

// ��ʾһ���ʼ�
class Mail {
    string m_Title;
	string m_Content;
	MailAddress m_Sender;
	MailAddress m_Receiver;
	// TODO: �����ֶ�
public:
	Mail(string title, string content, MailAddress sender, MailAddress receiver);
	string& GetTitle();
	string& GetContent();
	MailAddress& GetSender();
	MailAddress& GetReceiver();
}