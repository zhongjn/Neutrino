#pragma once

#include "Mail.h"
#include "MailParser.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

// Naive Bayes Classifier
class SpamFilter {
public:
  typedef unsigned int uint;

  bool Predict(const Mail &mail); // interface
  SpamFilter();
  ~SpamFilter();

private:
  const string trainPath = "spambase/train.data";
  const string testPath = "spambase/test.data";
  const string paramsPath = "spambase/params";
  const int numAttrs = 57; // number of attributes
  const double bias = 1.0 / 4000; // smoothing

  vector<double> attrs; // attributes

  double logpHam; // log priors
  double logpSpam;

  vector<double> loglHam; // log likelihood
  vector<double> loglSpam;

  uint tp, fp, fn, tn; // evaluations

  void Train();
  void Test();
  bool Predict();
  void StoreParams();
  void LoadParams();
};
