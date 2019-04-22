#include "MailManager.h"
#include "ThirdParty/mailio/include/mailio/message.hpp"
#include "ThirdParty/mailio/include/mailio/smtp.hpp"

using namespace mailio;

void MailManager::FetchMails() {}

list<Mail> MailManager::ListMails() const { return list<Mail>(); }

void MailManager::SendMail(const Mail &mail) const {
  if (m_Credential.HasValue()) {
    auto &cred = m_Credential.GetValue();
    message msg;
    msg.from(mail_address("SENDER", mail.GetSender().GetAddress()));
    msg.add_recipient(mail_address("RECEIVER", mail.GetReceiver().GetAddress()));
    msg.subject(mail.GetSubject());
    msg.content(mail.GetContent());

    smtps conn(cred.GetServer(), cred.GetPort());
    conn.authenticate(cred.GetUsername(), cred.GetPassword(), smtps::auth_method_t::LOGIN);
    conn.submit(msg);
  }
  else {
    throw "Credential not set!";
  }
}
