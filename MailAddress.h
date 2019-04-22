#pragma once
#include "Property.h"
#include <string>
using namespace std;

class MailAddress {
  PROPERTY_READ(string, Address)

public:
  static bool Validate(const string &addr);

  MailAddress() = default;

  MailAddress(string addr);

  // ����������ṩ��const string&����ʽת��
  operator const string&() const;
};