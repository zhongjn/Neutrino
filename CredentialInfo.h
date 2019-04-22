#pragma once
#include "MailAddress.h"
#include "Property.h"
#include <string>

using namespace std;

class CredentialInfo {
  PROPERTY_READ(MailAddress, Username)
  PROPERTY_READ(string, Password)
  PROPERTY_READ(string, Server)
  PROPERTY_READ(int, Port)
  // TODO: 其他字段，对接网络邮箱用

public:
  CredentialInfo() = default;

  CredentialInfo(MailAddress username, string password, string server, int port);
};