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
  // TODO: �����ֶΣ��Խ�����������

public:
  CredentialInfo() = default;

  CredentialInfo(MailAddress username, string password, string server, int port);
};