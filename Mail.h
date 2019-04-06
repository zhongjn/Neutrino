#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

// 表示一个邮件
class Mail {
  PROPERTY_READ(string, Title)
  PROPERTY_READ(string, Content)
  PROPERTY_READ(MailAddress, Sender)
  PROPERTY_READ(MailAddress, Receiver)
  // TODO: 其他字段

public:
  Mail(string title, string content, MailAddress sender, MailAddress receiver) {
    m_Title = title;
    m_Content = content;
    m_Sender = sender;
    m_Receiver = receiver;
  }
};