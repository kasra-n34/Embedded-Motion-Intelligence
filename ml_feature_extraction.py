import pandas as pd
import numpy as np

# ===== USER SETTINGS =====
INPUT_FILE = "cleaned_merged_data.csv"
OUTPUT_FILE = "ml_ready_features.csv"
WINDOW_SIZE = 50       # samples per window (~1 sec at 50 Hz, adjust if needed)
STEP_SIZE = 50         # same as window size for non-overlapping windows (use 25 for 50% overlap)
# =========================

def extract_features(window: pd.DataFrame):
    """Compute classical features for one window."""
    features = {}

    # Accel features
    for axis in ["accelX_mps2", "accelY_mps2", "accelZ_mps2"]:
        features[f"{axis}_mean"] = window[axis].mean()
        features[f"{axis}_std"] = window[axis].std()
        features[f"{axis}_min"] = window[axis].min()
        features[f"{axis}_max"] = window[axis].max()

    # Gyro features
    for axis in ["gyroX_dps", "gyroY_dps", "gyroZ_dps"]:
        features[f"{axis}_mean"] = window[axis].mean()
        features[f"{axis}_std"] = window[axis].std()
        features[f"{axis}_min"] = window[axis].min()
        features[f"{axis}_max"] = window[axis].max()

    # Signal Magnitude Area (SMA)
    window["accel_magnitude"] = np.sqrt(window["accelX_mps2"]**2 +
                                        window["accelY_mps2"]**2 +
                                        window["accelZ_mps2"]**2)
    window["gyro_magnitude"] = np.sqrt(window["gyroX_dps"]**2 +
                                       window["gyroY_dps"]**2 +
                                       window["gyroZ_dps"]**2)
    features["accel_SMA"] = window["accel_magnitude"].mean()
    features["gyro_SMA"] = window["gyro_magnitude"].mean()

    # Label (most frequent label in the window)
    features["label"] = window["label"].mode()[0]

    return features

def create_feature_dataset(df):
    feature_rows = []

    for start in range(0, len(df) - WINDOW_SIZE + 1, STEP_SIZE):
        window = df.iloc[start:start + WINDOW_SIZE]
        feature_rows.append(extract_features(window))

    return pd.DataFrame(feature_rows)

if __name__ == "__main__":
    print(f"Loading {INPUT_FILE}...")
    df = pd.read_csv(INPUT_FILE)

    print("Extracting features...")
    feature_df = create_feature_dataset(df)

    print(f"Total windows created: {len(feature_df)}")
    print(feature_df.head())

    feature_df.to_csv(OUTPUT_FILE, index=False)
    print(f"ML-ready feature dataset saved to {OUTPUT_FILE}")
