#include <iostream>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <vector>
#define MAX_NUM_FEATURE 300

using namespace std;

struct HeuristicData {
    int numFeature = 0;
    double flCos[MAX_NUM_FEATURE][MAX_NUM_FEATURE];
    double flCorr[MAX_NUM_FEATURE];
    double MI[MAX_NUM_FEATURE];
    double MIC[MAX_NUM_FEATURE];
    double V[MAX_NUM_FEATURE];

    vector<vector<int> > allFeatures;
    vector<vector<int> > labeledFeatures;
    vector<int> labels;

    //todo
    double get_distance(uint32_t from, uint32_t to) const {

    }

    uint32_t find_node_with_max_value(uint32_t from, const vector<uint32_t> &nodes) const {
        auto result = from;
        auto min_dist = numeric_limits<double>::max();
        for (auto node : nodes) {
            auto dist = get_distance(from, node);
            if (dist < min_dist) {
                min_dist = dist;
                result = node;
            }
        }
        return result;
    }

    //todo
    void readDataFromFile(const char *fileName) {
        freopen(fileName, "r", stdin);
        string line;
        getline(cin, line);
        getline(cin, line);
        vector<string> row = split(line, ',');
        numFeature = row.size() - 2;
        for (int i = 1; i <= numFeature; i++) {
            vector<int> empty1;
            allFeatures.push_back(empty1);
            vector<int> empty2;
            labeledFeatures.push_back(empty2);
        }
        addDataFromRow(row);
        while (getline(cin, line)) {
            row = split(line, ',');
            addDataFromRow(row);
        }
        cout<<"finish";
    }

    void init() {
        calculateFlCos();
        calculateMI();
        calculateMIC();
        for (int i = 0; i < numFeature; i++) {
            V[i] = flCorr[i] =  MIC[i];
        }
    }

    void calculateFlCos() {
        for (int i = 0; i < numFeature; i++) {
            for (int j = 0; j < numFeature; j++) {
                flCos[i][j] = cosineSimilarity(allFeatures[i], allFeatures[j]);
            }
        }
    }

    void calculateMI() {
        for (int i = 0; i < numFeature; i++) {
            MI[i] = mutualInformation(labeledFeatures[i], labels);
        }
    }

    void calculateMIC() {
        for (int i = 0; i < numFeature; i++) {
            MIC[i] = MI[i];
        }
    }

    void addDataFromRow(vector<string> row) {
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
                mi += p_xy * log2(p_xy / (p_x * p_y));
            }
        }
        return mi;
    }



    static double dotProduct(const vector<int>& v1, const vector<int>& v2) {
        double result = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            result += v1[i] * v2[i];
        }
        return result;
    }

    static double magnitude(const vector<int>& v) {
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



