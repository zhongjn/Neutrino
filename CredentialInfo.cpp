#include "CredentialInfo.h"

CredentialInfo::CredentialInfo(MailAddress username, string password, string server, int port) {
	m_Username = username;
	m_Password = password;
  m_Server = server;
  m_Port = port;
}
