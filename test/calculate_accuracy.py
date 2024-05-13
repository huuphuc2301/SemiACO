import pandas as pd
import numpy as np
import argparse
import json
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import f1_score, accuracy_score

def calculate_result(train_csv_path, test_csv_path, selected_feature_json):
    data_train = pd.read_csv(train_csv_path)
    data_test = pd.read_csv(test_csv_path)
    selected_features = json.load(open(selected_feature_json, "r"))
    selected_features = selected_features['important_feature']

    X_train = data_train[data_train["split_data"] == 1].iloc[:, selected_features]
    y_train = data_train[data_train["split_data"] == 1]["class"]

    knn = KNeighborsClassifier(5)
    knn.fit(X_train, y_train)

    X_test = data_test.iloc[:, selected_features]
    y_test_true = data_test["class"]
    y_test_pred = knn.predict(X_test)

    f_score = f1_score(y_test_true, y_test_pred)
    accuracy = accuracy_score(y_test_true, y_test_pred)

    return f_score, accuracy

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--train_csv_path', type=str, default='', help='input csv path')
    parser.add_argument('--test_csv_path', type=str, default='', help='input csv path')
    parser.add_argument('--selected_features', type=str, default='', help='select feature json path')

    opt = parser.parse_args()
    f_score, accuracy = calculate_result(opt.train_csv_path, opt.test_csv_path, opt.selected_features)
    print("F-score: ", f_score, "\nAccuracy: ", accuracy)
