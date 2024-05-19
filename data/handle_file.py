import pandas as pd

def remove_first_column(input_file, output_file):
    # Đọc file CSV vào DataFrame
    df = pd.read_csv(input_file)

    # Xóa cột đầu tiên (cột đầu tiên có chỉ số là 0)
    df = df.iloc[:, 1:]

    # Ghi DataFrame kết quả vào file CSV mới
    df.to_csv(output_file, index=False)

# Ví dụ sử dụng
input_file = 'arcene.csv'
output_file = 'arcene.csv'
remove_first_column(input_file, output_file)
