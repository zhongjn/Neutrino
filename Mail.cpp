#include "Mail.h"

Mail::Mail(int id, string subject, string content, MailAddress sender, MailAddress receiver) {
    m_Id = id;
	m_Subject = subject;
	m_Content = content;
	m_Sender = sender;
	m_Receiver = receiver;
}
