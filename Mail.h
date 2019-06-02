#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

// 表示一个邮件
class Mail {
    PROPERTY_READ(int, Id)
    PROPERTY_READ(string, Subject)
    PROPERTY_READ(string, Content)
    PROPERTY_READ(MailAddress, Sender)
    PROPERTY_READ(MailAddress, Receiver)
    // TODO: 其他字段

public:
    Mail() = default;
    Mail(int id, string subject, string content, MailAddress sender,
        MailAddress receiver);
};