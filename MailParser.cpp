#include "MailParser.h"

const string wordListPath = "spambase/wordlist.txt";

// hard-coded parser restricted to spambase
vector<double> MailParser::Parse(const string& str) {
    // string text = mail.GetSubject() + ' ' + mail.GetContent();
    // preprocessing

    stringstream sso;
    for (auto ch : str) {
        if (ch >= 0) sso << ch;
    }
    string text_origin = sso.str();
    string text = text_origin;

    for (auto it = text.begin(); it != text.end(); ++it) {
        if (*it >= 0 && !isalpha((unsigned char)*it) && !isdigit((unsigned char)*it) && *it != ' ') {
            it = text.insert(it, ' ') + 2;
            it = text.insert(it, ' ');
        }
    }

    stringstream ss(text);
    string s;

    // count word frequencies
    vector<string> wordlist;
    unordered_map<string, uint> freq;

    ifstream infile;
    infile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        infile.open(wordListPath, ios::in);
        while (!infile.eof()) {
            infile >> s;
            wordlist.push_back(s);
            freq[s] = 0;
        }
        infile.close();
    }
    catch (const ifstream::failure & e) {
        cerr << "Fail to open wordlist" << endl;
    }

    uint totalWords = 0, totalChars = text.length();

    while (getline(ss, s, ' ')) {
        if (freq.find(s) != freq.end())
            ++freq[s];
        ++totalWords;
    }

    vector<double> res;
    for (auto word : wordlist) {
        if (word.length() == 1) // signs
            res.push_back(double(freq[word]) / double(totalChars) * 100.0);
        else
            res.push_back(double(freq[word]) / double(totalWords) * 100.0);
    }

    // count captial run length
    text = text_origin;
    // preprocessing
    for (auto it = text.begin(); it != text.end();) {
        if (*it >= 0 && !isalpha((unsigned char)*it))
            it = text.erase(it);
        else
            ++it;
    }
    text += 'o'; // tricky, avoid CAPITAL END

    uint capRunLenMax = 1, capRunLenSum = 1, capRunLenCur = 1, capRunCnt = 0;
    bool isInCap = false;
    for (const auto& c : text) {
        if (isupper(c)) {
            if (!isInCap) {
                isInCap = true;
                capRunLenCur = 1;
            }
            else
                ++capRunLenCur;
        }
        else if (isInCap) {
            if (capRunLenCur > capRunLenMax)
                capRunLenMax = capRunLenCur;
            capRunLenSum += capRunLenCur;
            ++capRunCnt;
            isInCap = false;
        }
    }
    double capRunLenAvg = double(capRunLenSum) / double(capRunCnt);

    res.push_back(capRunLenAvg);
    res.push_back(capRunLenMax);
    res.push_back(capRunLenSum);

    return res;
}