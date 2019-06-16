#include "SpamFilter.h"

const string trainPath = "spambase/train.data";
const string testPath = "spambase/test.data";
const string paramsPath = "spambase/params";
const int numAttrs = 57; // number of attributes
const double bias = 1.0 / 4000; // smoothing

// vector addition
template<class T>
vector<T> operator + (const vector<T> a, const vector<T> b) {
  if (a.size() != b.size())
    throw "Incompatable sizes";

  vector<T> res(a.size(), 0);
  for (auto i = 0; i < a.size(); ++i)
    res[i] = a[i] + b[i];

  return res;
}

// dot product
double operator * (const vector<double> &v, const vector<double> &u) {
  double res = 0.0;
  if (v.size() != u.size())
    return res;
    
  for (auto i = 0; i < v.size(); ++i)
    res += v[i] * u[i];

  return res;
}

SpamFilter::SpamFilter() {
  file.exceptions(fstream::failbit | fstream::badbit); // set file exceptions
  try {
    file.open(paramsPath, ios::in);
    LoadParams(); // load existing params
    file.close();
  }
  catch (const fstream::failure &e) {
    cout << "training" << endl; // initialization
    Train();
    cout << "storing" << endl;
    StoreParams();
    cout << "testing" << endl;
    Test();
  }
}

bool SpamFilter::Predict() {
  bool res = false;
  if (attrs * loglSpam + logpSpam > attrs * loglHam + logpHam) // posteriors
    res = true;

  return res;
}

bool SpamFilter::Predict(const Mail &mail) {
  attrs = MailParser::Parse(mail);

  return Predict();
}

void SpamFilter::Train() {
  uint numSpamTrain = 0, numHamTrain = 0;
  vector<double> mailAttrs(numAttrs), attrsHam(numAttrs), attrsSpam(numAttrs);

  // read data from SPAM Base
  try {
    file.open(trainPath, ios::in);

    while (!file.eof()) {
      bool isSpam = false;

      string mailData; // raw data
      getline(file, mailData);
      stringstream ss(mailData);
      for (int i = 0; i <= numAttrs; ++i) {
        string s;
        getline(ss, s, ',');
        stringstream toNumber(s);
        if (i == numAttrs)
          toNumber >> isSpam;
        else {
          toNumber >> mailAttrs[i];
          mailAttrs[i] += bias;
        }
      }
      if (isSpam) {
        ++numSpamTrain;
        attrsSpam = attrsSpam + mailAttrs;
      }
      else {
        ++numHamTrain;
        attrsHam = attrsHam + mailAttrs;
      }
    }
    file.close();
  }
  catch (const fstream::failure &e) {
    cerr << "Fail to open train file" << endl;
  }
  
  // calculate the priors
  logpSpam = log(double(numSpamTrain) / double(numHamTrain + numSpamTrain));
  logpHam = log(double(numHamTrain) / double(numHamTrain + numSpamTrain));

  // calculate the likelihood
  double sumAttrsHam = 0, sumAttrsSpam = 0;
  for (auto i = 0; i < attrsHam.size(); ++i) {
    sumAttrsHam += attrsHam[i];
    sumAttrsSpam += attrsSpam[i];
  }
  for (auto i = 0; i < attrsHam.size(); ++i) {
    loglHam.push_back(log(attrsHam[i] / sumAttrsHam));
    loglSpam.push_back(log(attrsSpam[i] / sumAttrsSpam));
  }
}

void SpamFilter::Test() {
  uint tp = 0, fp = 0, fn = 0, tn = 0; // evaluations

  try {
    file.open(testPath, ios::in);
    double attr;
    while (!file.eof()) {
      bool isSpam = false;
      attrs.resize(0);

      string mailData;
      getline(file, mailData);
      // read the data
      stringstream ss(mailData);
      for (int i = 0; i <= numAttrs; ++i) {
        string s;
        getline(ss, s, ',');
        stringstream toNumber(s);
        if (i == numAttrs)
          toNumber >> isSpam;
        else {
          toNumber >> attr;
          attrs.push_back(attr);
          attrs[i] += bias;
        }
      }
      // evaluate
      if (Predict()) {
        if (isSpam)
          ++tp;
        else
          ++fp;
      }
      else {
        if (isSpam)
          ++fn;
        else
          ++tn;
      }
    }

    file.close();
  }
  catch (const fstream::failure &e) {
    cerr << "Fail to open test file" << endl;
  }

  cout << "precision: " << double(tp) / double(tp + fp) << endl;
  cout << "recall: "  << double(tp) / double(tp + fn) << endl;
}

void SpamFilter::StoreParams() {
  try {
    file.open(paramsPath, ios::out);

    file << logpHam << ' ' << logpSpam << endl;
    for (auto val : loglHam)
      file << val << ' ';
    file << endl;
    for (auto val : loglSpam)
      file << val << ' ';
    file << endl;

    file.close();
  }
  catch (const fstream::failure &e) {
    cerr << "Fail to store params" << endl;
  }
}

void SpamFilter::LoadParams() {
  ifstream infile;
  infile.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    infile.open(paramsPath, ios::in);

    infile >> logpHam >> logpSpam;
    double attr;
    for (auto i = 0; i < numAttrs; ++i) {
      infile >> attr;
      loglHam.push_back(attr);
    }
    for (auto i = 0; i < numAttrs; ++i) {
      infile >> attr;
      loglSpam.push_back(attr);
    }

    infile.close();
  }
  catch (const ifstream::failure &e) {
    cerr << "Fail to load params" << endl;
  }
}