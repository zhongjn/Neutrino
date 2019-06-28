#pragma once
#include "CredentialInfo.h"
#include "Mail.h"
#include "Nullable.h"
#include "Property.h"
#include "Folder.h"
#include "Utility.h"
#include "CryptoProvider.h"
#include "ThirdParty/sqlite3/sqlite3.h"
#include <vector>

using namespace std;

// 表示邮件读取源（全部、某个文件夹、垃圾箱）
struct ListSource {
    enum class Type {
        All,
        Unread,
        Read,
        Flagged,
        Spam,
        Folder
    } type = Type::All;
    // 如果type为Folder，那么folderId需要设置
    int folderId;
};

// 表示邮件筛选的条件
struct ListCondition {
    struct Field {
        // 严格/模糊
        bool strict = false;
        // 要匹配的字符串
        string match;
        Field(bool strict, string match) : strict(strict), match(match) {};
    };
    // 如果为Null，则不启用该条件
    //Nullable<Field> subject;
    //Nullable<Field> content;
    //Nullable<Field> author;
    Nullable<string> match_full;
};

class MailManager {
    //PROPERTY_WRITE(Nullable<CredentialInfo>, Credential)
        // TODO: 其他属性？

private:
    sqlite3* db = nullptr;
    Nullable<CredentialInfo> cred = Null();
    CryptoProvider* crypt_cred = new Win32CryptoProvider();
    CryptoProvider* crypt_mail = new AES256CryptoProvider("asd12@!#*asd");

    Nullable<CredentialInfo> LoadSavedCredential();
    bool VerifyCredential(const CredentialInfo& cred);

public:
    MailManager();
    MailManager(const MailManager&) = delete;
    MailManager(MailManager&&) = default;
    ~MailManager();

    bool IsLoginNeeded() { return !cred; }
    bool Login(const CredentialInfo& cred, bool rememberMe);

    // 拉取在线邮件，写到本地
    void FetchMails();

    // 读取本地所有邮件，source指定读取源（全部、某个文件夹、垃圾箱），cond指定筛选条件
    vector<Mail> ListMails(const ListSource& source = ListSource(), const ListCondition& cond = ListCondition()) const; // 列出所有本地的邮件（给UI用) TODO: 筛选、检索

    // 发送一封邮件
    void SendMail(const Mail& mail) const;

    // 删除一封邮件，根据id
    void DeleteMail(int id);

    // 设置一封邮件归属的文件夹，folderId字段若为Null则归入全部邮件
    void SetMailFolder(int mailId, Nullable<int> folderId);

    // 把一封邮件标为或不标为垃圾（移入或移除垃圾箱）
    void SetMailSpam(int mailId, bool spam);

    // 把一封邮件标为或不为标星标
    void SetMailFlag(int mailId, bool flag);

    //  把一封邮件标为已读
    void SetMailRead(int mailId, bool read);

    // 创建一个自定义文件夹
    void AddFolder(string name);

    // 删除一个自定义文件夹
    void DeleteFolder(int id);

    // 列出所有自定义文件夹
    vector<Folder> ListFolders();

    // 重命名一个自定义文件夹
    void RenameFolder(int id, string name);
};