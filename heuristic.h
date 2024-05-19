#include <iostream>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <vector>
#define MAX_NUM_FEATURE 300

using namespace std;

struct HeuristicData {
    int numFeature = 0;
    vector<double> V;
    vector<double> MI;
    vector<vector<double> > cos;

    vector<vector<int> > allFeatures;
    vector<vector<int> > labeledFeatures;
    vector<int> labels;
    uint32_t dimension_;

    vector<vector<int> > labeledSamples;

    vector<pair<double, double> > singleStatistic;

    HeuristicData(const char* fileName) {
        readDataFromFile(fileName);
        init();
        int x = 1;
    }

    double getMutualInformation(int node) const {
        return MI[node];
    }

    double getSingleAccuracy(int node) const {
        return singleStatistic[node].first;
    }

    void readDataFromFile(const char *fileName) {
        freopen(fileName, "r", stdin);
        string line;
        getline(cin, line);
        getline(cin, line);
        vector<string> row = split(line, ',');
        numFeature = row.size() - 2;
        for (int i = 1; i <= numFeature; i++) {
            vector<int> empty;
            allFeatures.push_back(empty);
            labeledFeatures.push_back(empty);
        }
        addDataFromRow(row);
        while (getline(cin, line)) {
            row = split(line, ',');
            addDataFromRow(row);
        }
    }

    void init() {
        dimension_ = numFeature;
        for (int i = 0; i < numFeature; i++) {
            V.push_back(1);
            MI.push_back(calculateMutualInformation(labeledFeatures[i], labels));

        }

        for (int i = 0; i < numFeature; i++) {
            vector<double> cs;
            for (int j = 0; j < numFeature; j++) {
                cs.push_back(cosineSimilarity(labeledFeatures[i], labeledFeatures[j]));
            }
            cos.push_back(cs);
        }

        int x = 1;
    }


    void addDataFromRow(vector<string> row) {
        if (row[numFeature + 1] == "1") {
            vector<int> sample;
            for (string s : row) {
                sample.push_back(stoi(s));
            }
            labeledSamples.push_back(sample);
        }

        for (int i = 0; i < numFeature; i++) {
            allFeatures[i].push_back(stoi(row[i]));
        }
        if (row[numFeature + 1] == "1") {
            for (int i = 0; i < numFeature; i++) {
                labeledFeatures[i].push_back(stoi(row[i]));
            }
            labels.push_back(stoi(row[numFeature]));
        }

    }

    double calculateMutualInformation(const vector<int>& X, const vector<int>& Y) {
        // Calculate joint probability distribution
        map<pair<int, int>, double> jointProb;
        map<int, double> marginalX, marginalY;
        int n = X.size();

        for (int i = 0; i < n; ++i) {
            ++jointProb[make_pair(X[i], Y[i])];
            ++marginalX[X[i]];
            ++marginalY[Y[i]];
        }

        // Normalize probabilities
        for (auto& elem : jointProb)
            elem.second /= n;

        for (auto& elem : marginalX)
            elem.second /= n;

        for (auto& elem : marginalY)
            elem.second /= n;

        // Calculate mutual information
        double mutualInfo = 0.0;
        for (const auto& elem : jointProb) {
            double pXY = elem.second;
            double pX = marginalX[elem.first.first];
            double pY = marginalY[elem.first.second];

            mutualInfo += pXY * log2(pXY / (pX * pY));
        }

        return mutualInfo;
    }

    static double mutualInformation(const vector<int>& labeledFeature, const vector<int>& label) {
        int totalSamples = labeledFeature.size();
        int countBoth[10000][20];
        int countX[10000];
        int countY[20];
        for (int i = 0; i < totalSamples; ++i) {
            countBoth[labeledFeature[i] + 1000][label[i]]++;
            countX[labeledFeature[i] + 1000]++;
            countY[label[i]]++;
        }

        // Calculate mutual information
        double mi = 0.0;
        for (int x = 0; x <= 2000; x++) {
            for (int y = 0; y < 20; y++) {
                double p_xy = double (countBoth[x][y]) / totalSamples;
                double p_x = double (countX[x]) / totalSamples;
                double p_y = double (countY[y]) / totalSamples;
                if (p_xy > 0 && p_x * p_y > 0) {
                    mi += p_xy * log2(p_xy / (p_x * p_y));
                    int s= 1;
                }
            }
        }
        return -mi;
    }



    static double dotProduct(const vector<int>& v1, const vector<int>& v2) {
        double result = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            result += v1[i] * v2[i];
        }
        return result;
    }

    static double magnitude(const vector<int> &v) {
        double result = 0.0;
        for (double val : v) {
            result += val * val;
        }
        return sqrt(result);
    }

    static double cosineSimilarity(const vector<int>& v1, const vector<int>& v2) {
        double dot = dotProduct(v1, v2);
        double mag1 = magnitude(v1);
        double mag2 = magnitude(v2);
        if (mag1 == 0 || mag2 == 0) {
            return 0; // To avoid division by zero
        }
        return abs(dot / (mag1 * mag2));
    }

    static vector<string> split(string s, char ch) {
        vector<string> res;
        istringstream is(s);
        string item;
        while (getline(is, item, ch)) {
            res.push_back(item);
        }
        return res;
    }


};



