#include <unordered_map>
#include "DB.h"
#include "MailManager.h"
#include "ThirdParty/mailio/include/mailio/message.hpp"
#include "ThirdParty/mailio/include/mailio/pop3.hpp"
#include "ThirdParty/mailio/include/mailio/smtp.hpp"

using namespace mailio;

MailManager::MailManager() {
    sqlite3_open("data.db", &db);
    string sqlCreateMails =
        "CREATE TABLE IF NOT EXISTS mails ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "   subject TEXT,"
        "   content TEXT,"
        "   time INTEGER,"
        "   is_spam integer DEFAULT 0,"
        "   folder_id INTEGER DEFAULT - 1,"
        "   read integer DEFAULT 0,"
        "   flag integer DEFAULT 0"
        ");";

    string sqlCreateFolders =
        "CREATE TABLE IF NOT EXISTS folders ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "   name TEXT"
        ");";

    sqlite3_exec(db, sqlCreateMails.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, sqlCreateFolders.c_str(), nullptr, nullptr, nullptr);

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

        auto& pop3 = m_Credential->GetPop3();
        pop3s conn(pop3.GetIPAddress(), pop3.GetPort());
        conn.authenticate(m_Credential->GetUsername(), m_Credential->GetPassword(),
            pop3s::auth_method_t::LOGIN);

        auto mails = conn.list(0); // (序号, id）

        time_t maxTime = -1;
        string sqlMaxTime = "SELECT MAX(time) FROM mails;";
        DB_CALLBACK(callback) {
            DB_CALLBACK_PARAM(time_t);
            *param = values[0] ? atoi(values[0]) : 0;
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

                // TODO: 判断垃圾邮件

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

vector<Mail> MailManager::ListMails(const ListSource& source, const ListCondition& cond) const {
    // TODO: 收件人、发件人
    vector<Mail> lst;
    string sql = "SELECT id, subject, content, time FROM mails ORDER BY time DESC;";
    DB_CALLBACK(callback) {
        DB_CALLBACK_PARAM(vector<Mail>);
        param->push_back(Mail(atoi(values[0]), values[1], values[2], MailAddress("from@example.com"), MailAddress("to@example.com")));
        return 0;
    };
    sqlite3_exec(db, sql.c_str(), callback, &lst, nullptr);
    return lst;
}

void MailManager::SendMail(const Mail& mail) const {
    if (m_Credential) {
        message msg;
        msg.from(mail_address("SENDER", mail.GetSender().GetAddress()));
        msg.add_recipient(
            mail_address("RECEIVER", mail.GetReceiver().GetAddress()));
        msg.subject(mail.GetSubject());
        msg.content(mail.GetContent());
        auto& smtp = m_Credential->GetSmtp();
        smtps conn(smtp.GetIPAddress(), smtp.GetPort());
        conn.authenticate(m_Credential->GetUsername(), m_Credential->GetPassword(),
            smtps::auth_method_t::LOGIN);
        conn.submit(msg);
    }
    else {
        throw "Credential not set!";
    }
}

void MailManager::DeleteMail(int id) {
    stringstream ss;
    ss << "DELETE FROM mails WHERE id=" << id;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

void MailManager::SetMailFolder(int mailId, Nullable<int> folderId) {
    if (!folderId) folderId = -1;
    stringstream ss;
    ss << "UPDATE mails SET folder_id=" << folderId.Value() << " WHERE id=" << mailId;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

void MailManager::SetMailSpam(int mailId, bool spam) {
    stringstream ss;
    ss << "UPDATE mails SET is_spam=" << spam << " WHERE id=" << mailId;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

void MailManager::AddFolder(string name) {
    stringstream ss;
    ss << "INSERT INTO folders (name) VALUES (" << quote(name) << ")";
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

void MailManager::DeleteFolder(int id) {
    stringstream ss;
    ss << "DELETE FROM folders WHERE id=" << id;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

vector<Folder> MailManager::ListFolders() {
    vector<Folder> folders;
    string sql = "SELECT id,name FROM folders";
    DB_CALLBACK(callback) {
        DB_CALLBACK_PARAM(vector<Folder>);
        param->push_back(Folder(atoi(values[0]), values[1]));
        return 0;
    };
    sqlite3_exec(db, sql.c_str(), callback, &folders, nullptr);
    return folders;
}

void MailManager::RenameFolder(int id, string name) {
    stringstream ss;
    ss << "UPDATE folders SET name=" << quote(name) << " WHERE id=" << id;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}
