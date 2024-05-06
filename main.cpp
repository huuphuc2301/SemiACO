#include <iostream>
#include "heuristic.h"
using namespace std;

int main() {
    HeuristicData heuristicData;
    heuristicData.readDataFromFile("./test/musk2_after_split.csv");
    heuristicData.init();
    int x = 1;
}