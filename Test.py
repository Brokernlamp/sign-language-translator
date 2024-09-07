# Load the new data
new_df = pd.read_csv('test.csv')

# Preprocess the new data (if needed)
# Make sure the preprocessing steps match those used for the training data

# Make predictions on the new data
y_pred_new = clf.predict(new_df)  # 'clf' is the trained Random Forest classifier

# Print the predicted labels
print("Predicted Labels:")
print(y_pred_new)

