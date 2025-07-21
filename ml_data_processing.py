import pandas as pd
import glob
import os

# ======== USER SETTINGS ========
DATA_FOLDER = "logs"  # folder where all CSVs are stored
OUTPUT_FILE = "cleaned_merged_data.csv"

# Manually set correct labels (filename: label)
# Example: 0 = stationary, 1 = walking, 2 = running
FILE_LABEL_MAP = {
    "stationary_0.csv": 0,
    "stationary_1.csv": 0,
    "walking_0.csv": 1,
    "walking_1.csv": 1,  
    "running_0.csv": 2,
    "running_1.csv": 2,
    "running_2.csv": 2
}
# ===============================

def load_and_clean_csv(file_path):
    try:
        df = pd.read_csv(file_path)
    except pd.errors.ParserError as e:
        print(f"⚠️ Bad line in {file_path}: {e}")
        df = pd.read_csv(file_path, on_bad_lines="skip")

    df.columns = [c.strip() for c in df.columns]

    df.drop_duplicates(inplace=True)
    df.dropna(inplace=True)

    df = df[(df["accelX_mps2"].between(-50, 50)) &
            (df["accelY_mps2"].between(-50, 50)) &
            (df["accelZ_mps2"].between(-50, 50)) &
            (df["gyroX_dps"].between(-500, 500)) &
            (df["gyroY_dps"].between(-500, 500)) &
            (df["gyroZ_dps"].between(-500, 500))]

    filename = os.path.basename(file_path)
    df["label"] = FILE_LABEL_MAP.get(filename, df.get("label", -1))
    return df

def merge_all_csvs(data_folder):
    csv_files = glob.glob(os.path.join(data_folder, "*.csv"))
    all_dfs = []

    for file in csv_files:
        print(f"Processing {file}...")
        df = load_and_clean_csv(file)
        all_dfs.append(df)

    return pd.concat(all_dfs, ignore_index=True)

if __name__ == "__main__":
    cleaned_data = merge_all_csvs(DATA_FOLDER)
    cleaned_data.reset_index(drop=True, inplace=True)

    print(f"Total rows after cleaning: {len(cleaned_data)}")
    print(cleaned_data.head())

    cleaned_data.to_csv(OUTPUT_FILE, index=False)
    print(f"Cleaned data saved to {OUTPUT_FILE}")
