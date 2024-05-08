//
// Created by huuphuc on 05/05/2024.
//

#ifndef SEMIACO_KNN_H
#define SEMIACO_KNN_H


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std;

// Define a struct to hold the KNN model
struct KNN {
    vector<vector<int> > x_train;
    vector<int> y_train;
    vector<vector<int> > x_test;
    vector<int> y_test;
    vector<int> features;
    int k;

    double euclideanDistance(const vector<int> &p1, const vector<int> &p2) {
        double distance = 0.0;
        for (int i : features) {
            distance += pow(p1[i] - p2[i], 2);
        }
        return sqrt(distance);
    }


    void init(const vector<vector<int> > &sample, const vector<int> &label, double trainPercentage) {
        int size = sample.size();
        vector<int> random(size);
        iota(random.begin(), random.end(), 0);
        std::random_device rd;
        std::mt19937 gen(rd());

        std::shuffle(random.begin(), random.end(), gen);
        int trainNum = round(size * trainPercentage);
        for (int i = 0; i < trainNum; i++) {
            x_train.push_back(sample[i]);
            y_train.push_back(label[i]);
        }
        for (int i = trainNum; i < size; i++) {
            x_test.push_back(sample[i]);
            y_test.push_back(label[i]);
        }
    }


    int predictLabel(const vector<int> &x) {
        vector<pair<double, int> > distances;

        // Calculate distances between x and all training points
        for (int i = 0; i < x_train.size(); ++i) {
            double dist = euclideanDistance(x, x_train[i]);
            distances.push_back(make_pair(dist, y_train[i]));
        }

        // Sort distances
        sort(distances.begin(), distances.end());

        // Count votes for each class
        vector<int> classVotes(k, 0);
        for (int i = 0; i < k; ++i) {
            classVotes[distances[i].second]++;
        }

        // Return the class with the most votes
        int maxVotes = 0;
        int maxIndex = -1;
        for (int i = 0; i < k; ++i) {
            if (classVotes[i] > maxVotes) {
                maxVotes = classVotes[i];
                maxIndex = i;
            }
        }

        return y_train[maxIndex];
    }

    double calculateAccuracy(vector<int> inputFeatures) {
        this->features = inputFeatures;
        int trueCnt = 0;
        for (int i = 0; i < x_test.size(); ++i) {
            int predictedLabel = predictLabel(x_test[i]);
            trueCnt += predictedLabel == y_test[i];
        }

        return (double) trueCnt / (double) x_test.size();
    }
};






#endif //SEMIACO_KNN_H
