import pandas as pd
import numpy as np
import argparse

def split_dataset(csv_path, train_ratio):
    file_name = csv_path.split("/")[-1].split(".")[0]
    df = pd.read_csv(csv_path)
    shuffled_df = df.sample(frac=1, random_state=42)
    train_size = int(len(shuffled_df) * train_ratio)
    shuffled_df['split_data'] = np.where(np.arange(len(shuffled_df)) < train_size, 1, 0)
    shuffled_df.to_csv(f"{file_name}_after_split.csv", index=False)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--csv_path', type=str, default='', help='input csv path')
    parser.add_argument('--train_ratio', type=float, default=0.8, help='input csv path')

    opt = parser.parse_args()
    split_dataset(opt.csv_path, opt.train_ratio)

