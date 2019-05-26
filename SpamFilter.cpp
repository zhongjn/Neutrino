#include "SpamFilter.h"

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

bool SpamFilter::Predict() {
  bool res = false;
  // cout << attrs * loglSpam + logpSpam << ' ' << attrs * loglHam + logpHam << endl;
  if (attrs * loglSpam + logpSpam > attrs * loglHam + logpHam)
    res = true;
  return res;
}

bool SpamFilter::Predict(Mail mail) {

}

void SpamFilter::Train() {
  // read data from SPAM Base
  ifstream infile;
  infile.open(trainPath, ios::in);
  if (!infile) {
    cerr << "Fail to open file" << endl;
    exit(1);
  }

  uint numSpamTrain = 0, numHamTrain = 0;

  vector<double> mailAttrs(numAttrs);
  while (!infile.eof()) {
    bool isSpam = false;

    string mailData; // raw data
    getline(infile, mailData);
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

  logpSpam = log(double(numSpamTrain) / double(numHamTrain + numSpamTrain));
  logpHam = log(double(numHamTrain) / double(numHamTrain + numSpamTrain));

  infile.close();

  Likelihood(); // calculate likelihood
}

void SpamFilter::Test() {
  ifstream infile;
  infile.open(testPath, ios::in);
  if (!infile) {
    cerr << "Fail to open file" << endl;
    exit(1);
  }

  double attr;
  while (!infile.eof()) {
    bool isSpam = false;
    attrs.resize(0);

    string mailData;
    getline(infile, mailData);
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

  infile.close();

  cout << "precision: " << double(tp) / double(tp + fp) << endl;
  cout << "recall: "  << double(tp) / double(tp + fn) << endl;
}

void SpamFilter::Likelihood() {
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

SpamFilter::SpamFilter() :
  tp(0), fp(0), fn(0), tn(0) {
  for (auto i = 0; i < numAttrs; ++i) {
    attrsHam.push_back(0.0);
    attrsSpam.push_back(0.0);
  }

  ifstream infile;
  infile.open(paramsPath, ios::in);
  if (!infile) {
    cout << "training" << endl;
    Train();
    cout << "storing" << endl;
    StoreParams();
    cout << "testing" << endl;
    Test();
  }
  else
    LoadParams();
  
  infile.close();
}

SpamFilter::~SpamFilter() {
}

void SpamFilter::StoreParams() {
  ofstream outfile;
  outfile.open(paramsPath, ios::out);
  if (!outfile) {
    cerr << "Fail to open file" << endl;
    exit(1);
  }

  outfile << logpHam << ' ' << logpSpam << endl;
  for (auto val : loglHam)
    outfile << val << ' ';
  outfile << endl;
  for (auto val : loglSpam)
    outfile << val << ' ';
  outfile << endl;
}

void SpamFilter::LoadParams() {
  ifstream infile;
  infile.open(paramsPath, ios::in);
  if (!infile) {
    cerr << "Fail to open file" << endl;
    exit(1);
  }

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
}