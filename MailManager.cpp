#include "MailManager.h"
#include "ThirdParty/mailio/include/mailio/message.hpp"
#include "ThirdParty/mailio/include/mailio/pop3.hpp"
#include "ThirdParty/mailio/include/mailio/smtp.hpp"

using namespace mailio;

MailManager::MailManager() {
    sqlite3_open("data", &conn);
    char *createMails =
        "CREATE TABLE IF NOT EXISTS mails (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "subject TEXT, content TEXT, time DATETIME);";
    sqlite3_exec(conn, createMails, NULL, NULL, NULL);
}

MailManager::~MailManager() { sqlite3_close(conn); }

void MailManager::FetchMails() {
    if (m_Credential.HasValue()) {
        auto &cred = m_Credential.GetValue();
        // TODO: 先拉取邮件头，如果不在数据库里，那么拉取整个

        auto &pop3 = cred.GetPop3();
        pop3s conn(pop3.GetIPAddress(), pop3.GetPort());
        conn.authenticate(cred.GetUsername(), cred.GetPassword(),
            pop3s::auth_method_t::LOGIN);

        auto mails = conn.list(0);
        message msg;
        conn.fetch(2, msg, true);
        int a = 1;
    }
}

list<Mail> MailManager::ListMails() const {
    list<Mail> lst;
    lst.push_back(Mail("test subject 1", "test content 1", MailAddress("example1@a.com"), MailAddress("example1@b.com")));
    lst.push_back(Mail("test subject 2", "test content 2", MailAddress("example2@a.com"), MailAddress("example2@b.com")));
    lst.push_back(Mail("test subject 3", "test content 3", MailAddress("example3@a.com"), MailAddress("example3@b.com")));
    lst.push_back(Mail("test subject 4", "test content 4", MailAddress("example4@a.com"), MailAddress("example4@b.com")));
    return list<Mail>();
}

void MailManager::SendMail(const Mail &mail) const {
    if (m_Credential.HasValue()) {
        auto &cred = m_Credential.GetValue();
        message msg;
        msg.from(mail_address("SENDER", mail.GetSender().GetAddress()));
        msg.add_recipient(
            mail_address("RECEIVER", mail.GetReceiver().GetAddress()));
        msg.subject(mail.GetSubject());
        msg.content(mail.GetContent());

        auto &smtp = cred.GetSmtp();
        smtps conn(smtp.GetIPAddress(), smtp.GetPort());
        conn.authenticate(cred.GetUsername(), cred.GetPassword(),
            smtps::auth_method_t::LOGIN);
        conn.submit(msg);
    }
    else {
        throw "Credential not set!";
    }
}
