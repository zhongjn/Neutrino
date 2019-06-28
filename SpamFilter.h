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

    bool Predict(const string& text); // interface
    SpamFilter();

private:
    vector<double> attrs; // attributes

    double logpHam; // log priors
    double logpSpam;

    vector<double> loglHam; // log likelihood
    vector<double> loglSpam;

    fstream file;

    void Train();
    void Test();
    bool Predict();
    void StoreParams();
    void LoadParams();
};
