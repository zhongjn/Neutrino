#include <unordered_map>
#include "DB.h"
#include "MailManager.h"
#include "ThirdParty/mailio/include/mailio/message.hpp"
#include "ThirdParty/mailio/include/mailio/pop3.hpp"
#include "ThirdParty/mailio/include/mailio/smtp.hpp"

using namespace mailio;

bool MailManager::VerifyCredential(const CredentialInfo& cred) {
    try {
        auto& pop3 = cred.GetPop3();
        pop3s conn_pop3(pop3.GetIPAddress(), pop3.GetPort());
        conn_pop3.authenticate(cred.GetUsername(), cred.GetPassword(),
            pop3s::auth_method_t::LOGIN);

        auto& smtp = cred.GetSmtp();
        smtps conn_smtp(smtp.GetIPAddress(), smtp.GetPort());
        conn_smtp.authenticate(cred.GetUsername(), cred.GetPassword(),
            smtps::auth_method_t::LOGIN);
    }
    catch (exception & ex) {
        cout << ex.what() << endl;
        return false;
    }
    return true;
}

MailManager::MailManager() {
    sqlite3_open("data.db", &db);

    string sqlCreateCredential = R"(
        CREATE TABLE IF NOT EXISTS cred (
            username text,
            password TEXT,
            smtp_addr TEXT,
            smtp_port integer,
            pop3_addr TEXT,
            pop3_port integer
        );
    )";

    string sqlCreateMails = R"(
        CREATE TABLE IF NOT EXISTS mails (
            id INTEGER PRIMARY KEY AUTOINCREMENT, 
            subject TEXT,
            content TEXT,
            time INTEGER,
            is_spam integer DEFAULT 0,
            folder_id INTEGER DEFAULT -1,
            read integer DEFAULT 0,
            flag integer DEFAULT 0,
            sender TEXT,
            receiver TEXT,
            attachment_name TEXT
        );
    )";

    string sqlCreateFolders =
        "CREATE TABLE IF NOT EXISTS folders ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "   name TEXT"
        ");";

    sqlite3_exec(db, sqlCreateCredential.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, sqlCreateMails.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, sqlCreateFolders.c_str(), nullptr, nullptr, nullptr);

    Nullable<CredentialInfo> saved = LoadSavedCredential();
    if (saved) Login(saved.Value(), true);
}

Nullable<CredentialInfo> MailManager::LoadSavedCredential() {
    string sql = "SELECT username, password, smtp_addr, smtp_port, pop3_addr, pop3_port FROM cred";
    struct Param {
        Nullable<CredentialInfo> cred;
        CryptoProvider* crypt;
    } param{ Null(), this->crypt_cred };

    DB_CALLBACK(callback) {
        DB_CALLBACK_PARAM(Param);
        param->cred = CredentialInfo(MailAddress(values[0]), param->crypt->Decrypt(values[1]), ServerEndPoint(values[2], atoi(values[3])), ServerEndPoint(values[4], atoi(values[5])));
        return 0;
    };
    sqlite3_exec(db, sql.c_str(), callback, &param, 0);
    return param.cred;
}

MailManager::~MailManager() { sqlite3_close(db); }

bool MailManager::Login(const CredentialInfo& cred, bool rememberMe) {
    if (VerifyCredential(cred)) {
        this->cred = cred;
        string sqldel = "DELETE FROM cred;";
        sqlite3_exec(db, sqldel.c_str(), 0, 0, 0);
        if (rememberMe) {
            char* err = nullptr;
            string sql = string_format(
                "INSERT INTO cred (username, password, smtp_addr, smtp_port, pop3_addr, pop3_port) VALUES (%s, %s, %s, %d, %s, %d);",
                quote(cred.GetUsername()).c_str(),
                quote(crypt_cred->Encrypt(cred.GetPassword())).c_str(),
                quote(cred.GetSmtp().GetIPAddress()).c_str(),
                cred.GetSmtp().GetPort(),
                quote(cred.GetPop3().GetIPAddress()).c_str(),
                cred.GetPop3().GetPort());
            sqlite3_exec(db, sql.c_str(), 0, 0, &err);
        }
        return true;
    }
    return false;
}

void MailManager::FetchMails() {
    if (cred) {

        auto& pop3 = cred->GetPop3();
        pop3s conn(pop3.GetIPAddress(), pop3.GetPort());
        conn.authenticate(cred->GetUsername(), cred->GetPassword(),
            pop3s::auth_method_t::LOGIN);

        auto mails = conn.list(0); // (序号, id）

        time_t maxTime = -1;
        string sqlMaxTime = "SELECT MAX(time) FROM mails;";
        DB_CALLBACK(callback) {
            DB_CALLBACK_PARAM(time_t);
            *param = values[0] ? atoi(values[0]) : 0;
            return 0;
        };
        sqlite3_exec(db, sqlMaxTime.c_str(), callback, &maxTime, 0);

        for (int i = mails.size(); i >= 1; i--) {
            message msgHeader;
            conn.fetch(i, msgHeader, true);
            time_t time = to_time_t(msgHeader.date_time().local_time());
            if (time > maxTime) {
                cout << "Writing into sqlite: " << msgHeader.subject() << endl;
                message msg;
                conn.fetch(i, msg, false);


                string text = msg.subject() + " " + msg.content();
                int spam = filter.Predict(text);

                string attName;
                int atts = msg.attachments_size();
                if (atts > 0) {
                    {
                        ofstream ofs("attachments/TEMP", std::ios::binary);
                        msg.attachment(1, ofs, attName);
                    }
                    rename("attachments/TEMP", ("attachments/" + attName).c_str());
                }

                string sqlInsert = string_format(
                    "INSERT INTO mails (subject, content, time, is_spam, sender, receiver, attachment_name) VALUES (%s, %s, %d, %d, %s, %s, %s)",
                    quote(msg.subject()).c_str(),
                    quote(crypt_mail->Encrypt(msg.content())).c_str(),
                    time,
                    spam,
                    quote(msg.sender().address).c_str(),
                    quote(msg.recipients_to_string()).c_str(),
                    quote(attName).c_str()
                );
                char* errmsg;
                sqlite3_exec(db, sqlInsert.c_str(), 0, 0, &errmsg);
            }
            else {
                break;
            }
        }
        cout << "Fetch done." << endl;
    }
}

vector<Mail> MailManager::ListMails(const ListSource & source, const ListCondition & cond) const {
    // TODO: 收件人、发件人
    vector<Mail> lst;
    stringstream ss;
    ss << R"(
        SELECT 
            id, subject, content, time, read, 
            flag, is_spam, sender, receiver, attachment_name 
        FROM mails)";
    switch (source.type)
    {
        case ListSource::Type::All:
            ss << " WHERE is_spam=0";
            break;
        case ListSource::Type::Unread:
            ss << " WHERE is_spam=0 AND read=0";
            break;
        case ListSource::Type::Read:
            ss << " WHERE is_spam=0 AND read=1";
            break;
        case ListSource::Type::Flagged:
            ss << " WHERE is_spam=0 AND flag=1";
            break;
        case ListSource::Type::Spam:
            ss << " WHERE is_spam=1";
            break;
        case ListSource::Type::Folder:
            ss << " WHERE is_spam=0 AND folder_id=" << source.folderId;
            break;
        default:
            throw logic_error("Invalid ListSource type. This should not happen.");
    }

    if (cond.match_full) {
        ss << " AND instr(ifnull(subject, '') || ifnull(content, ''), " << quote(cond.match_full.Value()) << ") > 0";
    }
    ss << " ORDER BY time DESC;";

    struct Param {
        vector<Mail> mails;
        CryptoProvider* crypt;
    } param{ vector<Mail>(), this->crypt_mail };

    DB_CALLBACK(callback) {
        DB_CALLBACK_PARAM(Param);
        Mail m(values[1], param->crypt->Decrypt(values[2]), MailAddress(values[7]), MailAddress(values[8]));
        m.SetId(atoi(values[0]));
        m.SetRead(atoi(values[4]));
        m.SetFlag(atoi(values[5]));
        m.SetSpam(atoi(values[6]));
        string attName = values[9];
        if (attName.length() > 0) {
            m.SetAttachmentName(attName);
        }
        param->mails.push_back(move(m));
        return 0;
    };
    sqlite3_exec(db, ss.str().c_str(), callback, &param, 0);
    return param.mails;
}

void MailManager::SendMail(const Mail & mail, Nullable<string> attachment_path) const {
    if (cred) {
        message msg;
        msg.from(mail_address("SENDER", mail.GetSender().GetAddress()));
        msg.add_recipient(
            mail_address("RECEIVER", mail.GetReceiver().GetAddress()));
        msg.subject(mail.GetSubject());
        msg.content(mail.GetContent());

        if (attachment_path) {
            string att = attachment_path.Value();
            try {
                ifstream ifs(att);
                const size_t last_slash_idx = att.find_last_of("\\/");
                string name = att;
                if (std::string::npos != last_slash_idx)
                {
                    name.erase(0, last_slash_idx + 1);
                }
                msg.attach(ifs, name, message::media_type_t::NONE, "att");
            }
            catch (exception & ex) {
                cout << ex.what() << endl;
            }
        }

        auto& smtp = cred->GetSmtp();
        smtps conn(smtp.GetIPAddress(), smtp.GetPort());
        conn.authenticate(cred->GetUsername(), cred->GetPassword(),
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

void MailManager::SetMailFlag(int mailId, bool flag) {
    stringstream ss;
    ss << "UPDATE mails SET flag=" << flag << " WHERE id=" << mailId;
    sqlite3_exec(db, ss.str().c_str(), nullptr, nullptr, nullptr);
}

void MailManager::SetMailRead(int mailId, bool read) {
    stringstream ss;
    ss << "UPDATE mails SET read=" << read << " WHERE id=" << mailId;
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
