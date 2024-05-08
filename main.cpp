#include <iostream>
#include "heuristic.h"
#include "knn.h"
using namespace std;

int main() {
    HeuristicData heuristicData;
    heuristicData.readDataFromFile("./test/musk2_after_split.csv");
    heuristicData.init();

    KNN knn;
    knn.init(heuristicData.labeledSamples, heuristicData.labels, 0.8);
    vector<int> features;
    for (int i = 1; i <= 30; i++) {
        features.push_back(2 * i);
    }
    cout << knn.calculateAccuracy(features);
    int x = 1;
}