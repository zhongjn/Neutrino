#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

// ��ʾһ���ʼ�
class Mail {
  PROPERTY_READ(string, Title)
  PROPERTY_READ(string, Content)
  PROPERTY_READ(MailAddress, Sender)
  PROPERTY_READ(MailAddress, Receiver)
  // TODO: �����ֶ�

public:
  Mail(string title, string content, MailAddress sender, MailAddress receiver) {
    m_Title = title;
    m_Content = content;
    m_Sender = sender;
    m_Receiver = receiver;
  }
};