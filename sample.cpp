#ifndef SEMIACO_SAMPLE_H
#define SEMIACO_SAMPLE_H

#include <bits/stdc++.h>
using namespace std;

namespace Sample
{
    struct sample {
        vector<int> feature;
        int label;

        sample(vector<int> _feature, int _label) {
            feature = _feature;
            label = _label;
        }
    };

    int cosine(sample x, sample y) {
        return 0;
    }
}
