#include <unordered_map>
#include "MailManager.h"
#include "ThirdParty/mailio/include/mailio/message.hpp"
#include "ThirdParty/mailio/include/mailio/pop3.hpp"
#include "ThirdParty/mailio/include/mailio/smtp.hpp"

using namespace mailio;

MailManager::MailManager() {
    sqlite3_open("data.db", &db);
    string sqlCreateMails =
        "CREATE TABLE IF NOT EXISTS mails (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "subject TEXT, content TEXT, time INTEGER);";
    sqlite3_exec(db, sqlCreateMails.c_str(), nullptr, nullptr, nullptr);
}

MailManager::~MailManager() { sqlite3_close(db); }

template<typename T>
string quote(const T& t) {
    stringstream ss;
    ss << "\"" << t << "\"";
    return ss.str();
}

void MailManager::FetchMails() {
    if (m_Credential) {
        // TODO: 先拉取邮件头，如果不在数据库里，那么拉取整个

        auto &pop3 = m_Credential->GetPop3();
        pop3s conn(pop3.GetIPAddress(), pop3.GetPort());
        conn.authenticate(m_Credential->GetUsername(), m_Credential->GetPassword(),
            pop3s::auth_method_t::LOGIN);

        auto mails = conn.list(0); // (序号, id）

        time_t maxTime = -1;
        string sqlMaxTime = "SELECT MAX(time) FROM mails;";
        auto callback = [](void* p, int count, char** values, char** names) -> int {
            *static_cast<time_t*>(p) = values[0] ? atoi(values[0]) : 0;
            return 0;
        };
        sqlite3_exec(db, sqlMaxTime.c_str(), callback, &maxTime, nullptr);

        for (int i = mails.size(); i >= 1; i--) {
            message msgHeader;
            conn.fetch(i, msgHeader, true);
            time_t time = to_time_t(msgHeader.date_time().local_time());
            if (time > maxTime) {
                cout << "Writing into sqlite: " << msgHeader.subject() << endl;
                message msg;
                conn.fetch(i, msg, false);
                stringstream sqlInsert;
                sqlInsert << "INSERT INTO mails (subject, content, time) VALUES (" << quote(msg.subject()) << "," << quote(msg.content()) << "," << quote(time) << ");";
                char* errmsg;
                sqlite3_exec(db, sqlInsert.str().c_str(), nullptr, nullptr, &errmsg);
            }
            else {
                break;
            }
        }
        cout << "Fetch done." << endl;
    }
}

list<Mail> MailManager::ListMails() const {
    // TODO: 收件人、发件人
    list<Mail> lst;
    string sql = "SELECT subject, content, time FROM mails ORDER BY time DESC;";
    auto callback = [](void* p, int count, char** values, char** names) -> int {
        auto lst = static_cast<list<Mail>*>(p);
        lst->push_back(Mail(values[0], values[1], MailAddress("from@example.com"), MailAddress("to@example.com")));
        return 0;
    };
    sqlite3_exec(db, sql.c_str(), callback, &lst, nullptr);
    return lst;
}

void MailManager::SendMail(const Mail &mail) const {
    if (m_Credential) {
        message msg;
        msg.from(mail_address("SENDER", mail.GetSender().GetAddress()));
        msg.add_recipient(
            mail_address("RECEIVER", mail.GetReceiver().GetAddress()));
        msg.subject(mail.GetSubject());
        msg.content(mail.GetContent());
        auto &smtp = m_Credential->GetSmtp();
        smtps conn(smtp.GetIPAddress(), smtp.GetPort());
        conn.authenticate(m_Credential->GetUsername(), m_Credential->GetPassword(),
            smtps::auth_method_t::LOGIN);
        conn.submit(msg);
    }
    else {
        throw "Credential not set!";
    }
}
