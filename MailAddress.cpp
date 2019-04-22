#include "MailAddress.h"

bool MailAddress::Validate(const string &addr) { return true; }

MailAddress::MailAddress(string addr) {
  if (!Validate(addr)) {
    throw "Unexpected error. Validate the address first.\n";
  }
  m_Address = addr;
}

MailAddress::operator const string& () const { return m_Address; }


