#pragma once

#include "Mail.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

const string trainPath = "spambase/train.data";
const string testPath = "spambase/test.data";
const string paramsPath = "spambase/params";
const int numAttrs = 57; // number of attributes
const double bias = 1.0 / 4000; // smoothing bias

// Naive Bayes Classifier
class SpamFilter {
public:
  typedef unsigned int uint;

  bool Predict(Mail mail);
  bool Predict();
  SpamFilter();
  ~SpamFilter();

private:
  vector<double> attrs; // attributes
  double logpHam; // log priors
  double logpSpam;

  vector<double> attrsHam; // attributes of training set
  vector<double> attrsSpam;
  vector<double> loglHam; // log likelihood of ham
  vector<double> loglSpam; // log likelihood of spam
  uint tp, fp, fn, tn;

  void Train();
  void Test();
  void GetAttrs(Mail mail);
  void StoreParams();
  void LoadParams();
  void Likelihood(); // calculate the likelihood
  double Posterior();
};
