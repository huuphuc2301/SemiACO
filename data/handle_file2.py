import pandas as pd

def convert_floats_to_integers(input_file, output_file):
    # Đọc file CSV vào DataFrame
    df = pd.read_csv(input_file)

    # Chuyển đổi tất cả các cột có kiểu số float sang số nguyên
    for col in df.select_dtypes(include=['float']):
        # Sử dụng hàm astype để chuyển đổi kiểu dữ liệu
        df[col] = df[col].astype('int64')

    # Ghi DataFrame kết quả vào file CSV mới
    df.to_csv(output_file, index=False)

# Ví dụ sử dụng
input_file = 'clean1_.csv'
output_file = 'clean1.csv'
convert_floats_to_integers(input_file, output_file)
