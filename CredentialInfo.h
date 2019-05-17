#pragma once
#include "IPEndPoint.h"
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

class CredentialInfo {
  PROPERTY_READ(MailAddress, Username)
  PROPERTY_READ(string, Password)
  PROPERTY_READ(ServerEndPoint, Smtp)
  PROPERTY_READ(ServerEndPoint, Pop3)
  // TODO: 其他字段，对接网络邮箱用

public:
  CredentialInfo() = default;
  CredentialInfo(MailAddress username, string password, ServerEndPoint stmp,
                 ServerEndPoint pop3);
};