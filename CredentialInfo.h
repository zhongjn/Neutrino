#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

class CredentialInfo {
  PROPERTY_READ(MailAddress, Address)
  PROPERTY_READ(string, Username)
  PROPERTY_READ(string, Password)
  // TODO: �����ֶΣ��Խ�����������

public:
  CredentialInfo() {}

  CredentialInfo(MailAddress addr, string username, string password) {
    m_Address = addr;
    m_Username = username;
    m_Password = password;
  }
};