#include "MailParser.h"

MailParser::MailParser() {

}

MailParser::~MailParser() {

}

// hard-coded parser restricted to spambase
vector<double> MailParser::Parse(const Mail &mail) {
  stringstream ss(mail.GetSubject() + ' ' + mail.GetContent());
  string s;

  // count word frequencies
  vector<string> wordlist;
  unordered_map<string, uint> freq;
  ifstream infile(wordListPath, ios::in);
  if (!infile) {
    cerr << "Fail to open file" << endl;
    exit(1);
  }
  while (!infile.eof()) {
    infile >> s;
    wordlist.push_back(s);
    freq[s] = 0;
  }
  infile.close();

  uint totalWords = 0, totalChars = ss.gcount();
  uint CapRunLenAvg = 0, CapRunLenMax = 0, CapRunLenSum = 0;

  while (getline(ss, s, ' ')) {
    if (freq.find(s) != freq.end())
      ++freq[s];
    ++totalWords;
  }
}