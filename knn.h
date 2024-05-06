//
// Created by huuphuc on 05/05/2024.
//

#ifndef SEMIACO_KNN_H
#define SEMIACO_KNN_H


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Define a struct to hold the KNN model
struct KNN {
    vector<vector<double>> X_train;
    vector<int> y_train;
    vector<int> features;
    int k;

    double euclideanDistance(const vector<double> &p1, const vector<double> &p2) {
        double distance = 0.0;
        for (int i : features) {
            distance += pow(p1[i] - p2[i], 2);
        }
        return sqrt(distance);
    }


    void fit(const vector<vector<double>> &X, const vector<int> &y) {
        X_train = X;
        y_train = y;
    }


    int predict(const vector<double> &x) {
        vector<pair<double, int>> distances;

        // Calculate distances between x and all training points
        for (int i = 0; i < X_train.size(); ++i) {
            double dist = euclideanDistance(x, X_train[i]);
            distances.push_back({dist, y_train[i]});
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

        return maxIndex;
    }

    double calculateFscore(vector<int> features) {
        return 0;
    }
};

// Helper function to calculate Euclidean distance between two points


int main() {
    // Example usage
    KNN classifier;
    classifier.k = 2;

    vector<vector<double>> X_train = {{1, 2},
                                      {2, 3},
                                      {3, 4}};
    vector<int> y_train = {0, 1, 0};
    classifier.fit(X_train, y_train);

    vector<double> new_data = {2, 2};
    int predicted_class = classifier.predict(new_data);

    cout << "Predicted class: " << predicted_class << endl;

    return 0;
}


#endif //SEMIACO_KNN_H
