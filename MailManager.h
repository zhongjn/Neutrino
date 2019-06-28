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

// ��ʾ�ʼ���ȡԴ��ȫ����ĳ���ļ��С������䣩
struct ListSource {
    enum class Type {
        All,
        Unread,
        Read,
        Flagged,
        Spam,
        Folder
    } type = Type::All;
    // ���typeΪFolder����ôfolderId��Ҫ����
    int folderId;
};

// ��ʾ�ʼ�ɸѡ������
struct ListCondition {
    struct Field {
        // �ϸ�/ģ��
        bool strict = false;
        // Ҫƥ����ַ���
        string match;
        Field(bool strict, string match) : strict(strict), match(match) {};
    };
    // ���ΪNull�������ø�����
    //Nullable<Field> subject;
    //Nullable<Field> content;
    //Nullable<Field> author;
    Nullable<string> match_full;
};

class MailManager {
    //PROPERTY_WRITE(Nullable<CredentialInfo>, Credential)
        // TODO: �������ԣ�

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

    // ��ȡ�����ʼ���д������
    void FetchMails();

    // ��ȡ���������ʼ���sourceָ����ȡԴ��ȫ����ĳ���ļ��С������䣩��condָ��ɸѡ����
    vector<Mail> ListMails(const ListSource& source = ListSource(), const ListCondition& cond = ListCondition()) const; // �г����б��ص��ʼ�����UI��) TODO: ɸѡ������

    // ����һ���ʼ�
    void SendMail(const Mail& mail) const;

    // ɾ��һ���ʼ�������id
    void DeleteMail(int id);

    // ����һ���ʼ��������ļ��У�folderId�ֶ���ΪNull�����ȫ���ʼ�
    void SetMailFolder(int mailId, Nullable<int> folderId);

    // ��һ���ʼ���Ϊ�򲻱�Ϊ������������Ƴ������䣩
    void SetMailSpam(int mailId, bool spam);

    // ��һ���ʼ���Ϊ��Ϊ���Ǳ�
    void SetMailFlag(int mailId, bool flag);

    //  ��һ���ʼ���Ϊ�Ѷ�
    void SetMailRead(int mailId, bool read);

    // ����һ���Զ����ļ���
    void AddFolder(string name);

    // ɾ��һ���Զ����ļ���
    void DeleteFolder(int id);

    // �г������Զ����ļ���
    vector<Folder> ListFolders();

    // ������һ���Զ����ļ���
    void RenameFolder(int id, string name);
};