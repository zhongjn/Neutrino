#pragma once
#include "Property.h"
#include <string>
using namespace std;

class MailAddress {
  PROPERTY_READ(string, Address)

public:
  static bool Validate(const string &addr); // TODO: ��֤����Ϸ���

  MailAddress() {}

  MailAddress(string addr) {
    if (!Validate(addr)) {
      throw "Unexpected error. Validate the address first.\n";
    }
    m_Address = addr;
  }
};