import pandas as pd
import numpy as np
from sklearn.metrics import roc_auc_score, accuracy_score, precision_score, recall_score, f1_score
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis, QuadraticDiscriminantAnalysis
from sklearn.cluster import KMeans
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import precision_recall_fscore_support
from sklearn.metrics import mean_squared_error as mse
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import cross_validate
from sklearn.model_selection import train_test_split
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.feature_selection import SelectFromModel
from sklearn.svm import LinearSVC
from sklearn.ensemble import VotingClassifier
from sklearn.feature_selection import RFECV
from math import floor

import warnings
warnings.filterwarnings('ignore')

# For plotting
import seaborn as sns
import matplotlib.pyplot as plt

# Read the data set
initial_data = pd.read_csv('data.csv')

# Remove the "ID" column
initial_data.drop(initial_data.columns[0], axis=1, inplace=True)

for column in initial_data.columns:
    if "Unnamed" in column:
        initial_data.drop(column, axis = 1, inplace=True)

# Binary diagnosis input
initial_data['diagnosis']=initial_data['diagnosis'].map({'M':1,'B':0})

# NOTE: W naszym przypadku chcemy, zeby każde wejście w naszym zbiorze treningowym miało
# tylko dwie własności. Dlatego zakomentowałem ten blok kodu, żeby przedefiniować go dalej w inny sposób.
# X = initial_data[['radius_mean', 'texture_mean', 'perimeter_mean', 'area_mean', 'smoothness_mean', 'compactness_mean', 'concavity_mean', 'concave points_mean', 'symmetry_mean','fractal_dimension_mean',
#                  'radius_se', 'texture_se', 'perimeter_se', 'area_se', 'smoothness_se', 'compactness_se', 'concavity_se', 'concave points_se', 'symmetry_se', 'fractal_dimension_se',
#                  'radius_worst', 'texture_worst', 'perimeter_worst', 'area_worst', 'smoothness_worst', 'compactness_worst', 'concavity_worst', 'concave points_worst', 'symmetry_worst','fractal_dimension_worst']]
# y = initial_data['diagnosis']

# Get the data, that we would like to use further
X = initial_data[['radius_mean', 'texture_mean']]
y = initial_data['diagnosis']

# Check, if there are some missing values in the set
col_labels = ['diagnosis','radius_mean', 'texture_mean', 'perimeter_mean', 'area_mean', 'smoothness_mean', 'compactness_mean', 'concavity_mean', 'concave_points_mean', 'symmetry_mean','fractal_dimension_mean',
              'radius_se', 'texture_se', 'perimeter_se', 'area_se', 'smoothness_se', 'compactness_se', 'concavity_se', 'concave points_se', 'symmetry_se', 'fractal_dimension_se',
              'radius_worst', 'texture_worst', 'perimeter_worst', 'area_worst', 'smoothness_worst', 'compactness_worst', 'concavity_worst', 'concave points_worst', 'symmetry_worst','fractal_dimension_worst'
             ]
initial_data.columns = col_labels

for c in col_labels:
    no_missing = initial_data[c].isnull().sum()
    if no_missing > 0:
        print(c)
        print(no_missing)

# Define sizes of trening, testing and prediction sets FOR ALL MODELS
prediction_set_size = 4 # This is the number of values from each class, so in fact the whole prediction set will consist of 10 elements
################################################################################
# TODO: Przy zmianie poniższej wartości "class_zero_train_set_size" z 11 na 12 wyrzuca błąd:
#
# cvxpy.error.DCPError: Problem does not follow DCP rules. Specifically:
# The objective is not DCP. Its following subexpressions are not:
#
# Oznacza to, że kod zaimplementowany przez autorów QSVM w którymś miejscu korzysta
# z tzw. programowania wypukłego. Możliwe, że dla naszych danych z pliku CSV problem
# przestaje być wypukły, przez co nie może zostać przetworzony przez pakiet CVXPY.
# (Ale do czego on może być tam używany, to nie mam pojęcia...)
# TODO C.D. Trzeba tak dobrać wejścia w zbiorze testowym, żeby QSVM mógł się
# uczyć na możliwie jak największym zbiorze testowym. Może trzeba usunąć jakieś wiersze
# w pliku CSV? Pokombinujcie.
class_zero_train_set_size = 11
class_one_train_set_size = 13
class_zero_test_set_size = 15
class_one_test_set_size = 15

# TODO: Chcemy pozbyć się takiej definicji, jaka jest w poniższej linii i stworzyć
# jeden zbiór treningowy dla wszystkich metod (klasycznych jak i dla QSVM). Na końcu
# kodu definiowane są "training_set", "test_set" i "prediction_set" dla modułu QSVM.
# Stwórzcie zbiory "X_train" i "y_train" w taki sposób, żeby klasyczne modele mogły się na nich
# nauczyć i żeby jednocześnie były takiego samego rozmiaru jak "training_set" itd. na końcu kodu.
X_train, X_test, y_train, y_test = train_test_split(X,y,test_size=0.34)
# X_train = X[:total_train_set_size]
# y_train = y[:total_train_set_size]
################################################################################

# Logistic Regression
LR = LogisticRegression()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(LR, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
LR_fit_time = scores['fit_time'].mean()
LR_score_time = scores['score_time'].mean()
LR_accuracy = scores['test_accuracy'].mean()
LR_precision = scores['test_precision_macro'].mean()
LR_recall = scores['test_recall_macro'].mean()
LR_f1 = scores['test_f1_weighted'].mean()
LR_roc = scores['test_roc_auc'].mean()


# Decision Tree
decision_tree = DecisionTreeClassifier()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(decision_tree, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
dtree_fit_time = scores['fit_time'].mean()
dtree_score_time = scores['score_time'].mean()
dtree_accuracy = scores['test_accuracy'].mean()
dtree_precision = scores['test_precision_macro'].mean()
dtree_recall = scores['test_recall_macro'].mean()
dtree_f1 = scores['test_f1_weighted'].mean()
dtree_roc = scores['test_roc_auc'].mean()


# Support Vector Machine
SVM = SVC(probability = True)

scoring = ['accuracy','precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(SVM, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
SVM_fit_time = scores['fit_time'].mean()
SVM_score_time = scores['score_time'].mean()
SVM_accuracy = scores['test_accuracy'].mean()
SVM_precision = scores['test_precision_macro'].mean()
SVM_recall = scores['test_recall_macro'].mean()
SVM_f1 = scores['test_f1_weighted'].mean()
SVM_roc = scores['test_roc_auc'].mean()


# Linear Discriminant Analysis
LDA = LinearDiscriminantAnalysis()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(LDA, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
LDA_fit_time = scores['fit_time'].mean()
LDA_score_time = scores['score_time'].mean()
LDA_accuracy = scores['test_accuracy'].mean()
LDA_precision = scores['test_precision_macro'].mean()
LDA_recall = scores['test_recall_macro'].mean()
LDA_f1 = scores['test_f1_weighted'].mean()
LDA_roc = scores['test_roc_auc'].mean()


# Quadratic Discriminant Analysis
QDA = QuadraticDiscriminantAnalysis()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(QDA, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
QDA_fit_time = scores['fit_time'].mean()
QDA_score_time = scores['score_time'].mean()
QDA_accuracy = scores['test_accuracy'].mean()
QDA_precision = scores['test_precision_macro'].mean()
QDA_recall = scores['test_recall_macro'].mean()
QDA_f1 = scores['test_f1_weighted'].mean()
QDA_roc = scores['test_roc_auc'].mean()


# Random Forest Classifier
random_forest = RandomForestClassifier()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(random_forest, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
forest_fit_time = scores['fit_time'].mean()
forest_score_time = scores['score_time'].mean()
forest_accuracy = scores['test_accuracy'].mean()
forest_precision = scores['test_precision_macro'].mean()
forest_recall = scores['test_recall_macro'].mean()
forest_f1 = scores['test_f1_weighted'].mean()
forest_roc = scores['test_roc_auc'].mean()


# K-Nearest Neighbors
KNN = KNeighborsClassifier()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(KNN, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
KNN_fit_time = scores['fit_time'].mean()
KNN_score_time = scores['score_time'].mean()
KNN_accuracy = scores['test_accuracy'].mean()
KNN_precision = scores['test_precision_macro'].mean()
KNN_recall = scores['test_recall_macro'].mean()
KNN_f1 = scores['test_f1_weighted'].mean()
KNN_roc = scores['test_roc_auc'].mean()


# Naive Bayes
bayes = GaussianNB()

scoring = ['accuracy', 'precision_macro', 'recall_macro' , 'f1_weighted', 'roc_auc']
scores = cross_validate(bayes, X_train, y_train, scoring=scoring, cv=20)

sorted(scores.keys())
bayes_fit_time = scores['fit_time'].mean()
bayes_score_time = scores['score_time'].mean()
bayes_accuracy = scores['test_accuracy'].mean()
bayes_precision = scores['test_precision_macro'].mean()
bayes_recall = scores['test_recall_macro'].mean()
bayes_f1 = scores['test_f1_weighted'].mean()
bayes_roc = scores['test_roc_auc'].mean()


# Comparison
models_initial = pd.DataFrame({
    'Model'       : ['Logistic Regression', 'Decision Tree', 'Support Vector Machine', 'Linear Discriminant Analysis', 'Quadratic Discriminant Analysis', 'Random Forest', 'K-Nearest Neighbors', 'Bayes'],
    'Fitting time': [LR_fit_time, dtree_fit_time, SVM_fit_time, LDA_fit_time, QDA_fit_time, forest_fit_time, KNN_fit_time, bayes_fit_time],
    'Scoring time': [LR_score_time, dtree_score_time, SVM_score_time, LDA_score_time, QDA_score_time, forest_score_time, KNN_score_time, bayes_score_time],
    'Accuracy'    : [LR_accuracy, dtree_accuracy, SVM_accuracy, LDA_accuracy, QDA_accuracy, forest_accuracy, KNN_accuracy, bayes_accuracy],
    'Precision'   : [LR_precision, dtree_precision, SVM_precision, LDA_precision, QDA_precision, forest_precision, KNN_precision, bayes_precision],
    'Recall'      : [LR_recall, dtree_recall, SVM_recall, LDA_recall, QDA_recall, forest_recall, KNN_recall, bayes_recall],
    'F1_score'    : [LR_f1, dtree_f1, SVM_f1, LDA_f1, QDA_f1, forest_f1, KNN_f1, bayes_f1],
    'AUC_ROC'     : [LR_roc, dtree_roc, SVM_roc, LDA_roc, QDA_roc, forest_roc, KNN_roc, bayes_roc],
    }, columns = ['Model', 'Fitting time', 'Scoring time', 'Accuracy', 'Precision', 'Recall', 'F1_score', 'AUC_ROC'])

print(models_initial.sort_values(by='Accuracy', ascending=False))

################################################################################
################################  QSVM  ########################################
################################################################################
from qiskit import BasicAer
from qiskit.ml.datasets import *
from qiskit.circuit.library import ZZFeatureMap
from qiskit.aqua.utils import split_dataset_to_data_and_labels, map_label_to_class_name
from qiskit.aqua import QuantumInstance
from qiskit.aqua.algorithms import QSVM

# setup aqua logging
import logging
from qiskit.aqua import set_qiskit_aqua_logging
# set_qiskit_aqua_logging(logging.DEBUG)  # choose INFO, DEBUG to see the log

# [Optional] Setup token to run the experiment on a real device
# If you would like to run the experiment on a real device, you need to setup your account first.
# Note: If you do not store your token yet, use IBMQ.save_account('MY_API_TOKEN') to store it first.
# from qiskit import IBMQ
# provider = IBMQ.load_account()

# Cast the data into format, that is understandable for qiskit.aqua
class_zero = []
class_one = []
for index, row in initial_data.iterrows():
    if int(row['diagnosis']) == 0:
        class_zero.append([row['radius_mean'], row['texture_mean']])
    else:
        class_one.append([row['radius_mean'], row['texture_mean']])

# NOTE: Poniższy blok kodu został napisany właśnie tutaj. Zakomentowałem go i przeniosłem
# na początek skrytpu, żeby móc zdefiniować jeden zbiór testowy dla wszystkich modeli.
# prediction_set_size = 4 # This is the number of values from each class, so in fact the whole prediction set will consist of 10 elements
# class_zero_train_set_size = 11
# class_one_train_set_size = 13
# class_zero_test_set_size = 15
# class_one_test_set_size = 15

train_set = {0:class_zero[:class_zero_train_set_size], 1:class_one[:class_one_train_set_size]}
test_set = {0:class_zero[class_zero_train_set_size:class_zero_train_set_size+class_zero_test_set_size],
            1:class_one[class_one_train_set_size:class_one_train_set_size+class_one_test_set_size]}
prediction_set = class_zero[-prediction_set_size:] + class_one[-prediction_set_size:]
prediction_set_classes = [0]*prediction_set_size + [1]*prediction_set_size

# Create model and make predictions
seed = 10598

feature_dim=2 # we support feature_dim 2 or 3

feature_map = ZZFeatureMap(feature_dimension=feature_dim, reps=2, entanglement='linear')
qsvm = QSVM(feature_map, train_set, test_set, prediction_set)

backend = BasicAer.get_backend('qasm_simulator')
quantum_instance = QuantumInstance(backend, shots=1024, seed_simulator=seed, seed_transpiler=seed)

result = qsvm.run(quantum_instance)

print("testing success ratio: {}".format(result['testing_accuracy']))
print("preduction of datapoints:")
print("ground truth: {}".format(prediction_set_classes))
print("prediction:   {}".format(result['predicted_classes']))

# print("kernel matrix during the training:")
# kernel_matrix = result['kernel_matrix_training']
# img = plt.imshow(np.asmatrix(kernel_matrix),interpolation='nearest',origin='upper',cmap='bone_r')
# plt.show()
