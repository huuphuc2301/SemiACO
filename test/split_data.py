import pandas as pd
import numpy as np
import argparse

def split_dataset(csv_path, random_ratio, test_ratio):
    file_name = csv_path.split("/")[-1].split(".")[0]
    df = pd.read_csv(csv_path)
    shuffled_df = df.sample(frac=1, random_state=22)

    test_size = int(test_ratio * len(shuffled_df))
    test_df = shuffled_df.iloc[:test_size, :]
    train_df = shuffled_df.iloc[test_size:, :]

    train_size = int(len(train_df) * (1 - random_ratio))
    train_df.loc[:, 'split_data'] = (np.arange(len(train_df)) < train_size).astype(int)
    train_df.to_csv(f"{file_name}_train.csv", index=False)
    test_df.to_csv(f"{file_name}_test.csv", index = False)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--csv_path', type=str, default='', help='input csv path')
    parser.add_argument('--test_ratio', type=float, default=0.4, help='test ratio')
    parser.add_argument('--random_ratio', type=float, default=0.2, help='random data ratio in train data')
    opt = parser.parse_args()
    split_dataset(opt.csv_path, opt.random_ratio, opt.test_ratio)

