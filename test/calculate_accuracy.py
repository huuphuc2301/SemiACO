import pandas as pd
import numpy as np
import argparse
import json
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import f1_score, accuracy_score

def calculate_result(csv_path, selected_feature_json):
    data = pd.read_csv(csv_path)
    selected_features = json.load(open(selected_feature_json, "r"))
    selected_features = selected_features['important_feature']

    X_train = data[data["split_data"] == 1].iloc[:, selected_features]
    y_train = data[data["split_data"] == 1]["class"]

    knn = KNeighborsClassifier()
    knn.fit(X_train, y_train)

    X_test = data[data["split_data"] == 0].iloc[:, selected_features]
    y_test_true = data[data["split_data"] == 0]["class"]
    y_test_pred = knn.predict(X_test)

    f_score = f1_score(y_test_true, y_test_pred)
    accuracy = accuracy_score(y_test_true, y_test_pred)

    return f_score, accuracy

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--csv_path', type=str, default='', help='input csv path')
    parser.add_argument('--selected_features', type=str, default='', help='select feature json path')

    opt = parser.parse_args()
    f_score, accuracy = calculate_result(opt.csv_path, opt.selected_features)
    print("F-score: ", f_score, "\nAccuracy: ", accuracy)
