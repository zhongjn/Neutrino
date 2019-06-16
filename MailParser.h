#pragma once

#include "Mail.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <fstream>

using namespace std;

// analyze the attributes of email
// static method used to parse mails
class MailParser {
public:
  typedef unsigned int uint;

  static vector<double> Parse(const Mail &mail);
};