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

  // 方便起见，提供到const string&的隐式转换
  operator const string&() const;
};