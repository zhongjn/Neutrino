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
class MailParser {
public:
  typedef unsigned int uint;

  vector<double> Parse(const Mail &mail);
  MailParser();
  ~MailParser();
private:
  const string wordListPath = "spambase/wordlist.txt";
};