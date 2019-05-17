#include "IPEndPoint.h"

ServerEndPoint::ServerEndPoint(string ipAddress, int port)
{
  m_IPAddress = ipAddress;
  m_Port = port;
}