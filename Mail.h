#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

// 表示一个邮件
class Mail {
    PROPERTY(int, Id)
    PROPERTY(bool, Flag)
    PROPERTY(bool, Spam)
    PROPERTY(bool, Read)
    PROPERTY_READ(string, Subject)
    PROPERTY_READ(string, Content)
    PROPERTY_READ(MailAddress, Sender)
    PROPERTY_READ(MailAddress, Receiver)
    // TODO: 其他字段

public:
    Mail() = default;
    Mail(string subject, string content, MailAddress sender,
        MailAddress receiver);
};