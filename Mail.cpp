#include "Mail.h"

Mail::Mail(string subject, string content, MailAddress sender, MailAddress receiver) {
	m_Subject = subject;
	m_Content = content;
	m_Sender = sender;
	m_Receiver = receiver;
}
