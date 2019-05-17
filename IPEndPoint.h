#pragma once
#include "Property.h"
#include <string>

using namespace std;

class ServerEndPoint {
  PROPERTY_READ(string, IPAddress)
  PROPERTY_READ(int, Port)
public:
  ServerEndPoint() = default;
  ServerEndPoint(string ipAddress, int port);
};