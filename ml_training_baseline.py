import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix

# ===== USER SETTINGS =====
INPUT_FILE = "ml_ready_features.csv"
TEST_SIZE = 0.2   # 20% for testing
RANDOM_STATE = 42
# =========================

# 1. Load Data
print(f"Loading {INPUT_FILE}...")
df = pd.read_csv(INPUT_FILE)

# Separate features and labels
X = df.drop(columns=["label"])
y = df["label"]

# 2. Train/Test Split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=TEST_SIZE,
                                                    random_state=RANDOM_STATE,
                                                    stratify=y)

# 3. Train Random Forest
print("Training Random Forest...")
model = RandomForestClassifier(n_estimators=100, random_state=RANDOM_STATE)
model.fit(X_train, y_train)

# 4. Evaluate
y_pred = model.predict(X_test)
acc = accuracy_score(y_test, y_pred)

print(f"\nAccuracy on Test Set: {acc * 100:.2f}%")
print("\nClassification Report:")
print(classification_report(y_test, y_pred))
print("\nConfusion Matrix:")
print(confusion_matrix(y_test, y_pred))

# 5. Save the Model (optional, for later export)
import joblib
joblib.dump(model, "motion_classifier_rf.pkl")
print("Model saved as motion_classifier_rf.pkl")
