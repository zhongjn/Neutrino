#include "CredentialInfo.h"

CredentialInfo::CredentialInfo(MailAddress username, string password,
                               ServerEndPoint smtp, ServerEndPoint pop3) {
  m_Username = username;
  m_Password = password;
  m_Smtp = smtp;
  m_Pop3 = pop3;
}
