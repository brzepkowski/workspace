from __future__ import division #Force float division
import sklearn
import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.cross_validation import train_test_split
iris = datasets.load_iris()

# Podzial na zbior treningowy i testowy
data_train, data_test, target_train, target_test = train_test_split(iris.data, iris.target, test_size=0.2, random_state=42)

#Regresja logistyczna
from sklearn import linear_model
clf1 = linear_model.LogisticRegression()
clf1.fit(data_train, target_train)
clf1.predict(data_test)
target_test

#Drzewo decyzyjne
from sklearn import tree
clf2 = tree.DecisionTreeClassifier()
clf2.fit(data_train, target_train)
clf2.predict(data_test)
target_test

#Metoda k-najblizszych sasiadow
from sklearn import neighbors
clf3 = neighbors.KNeighborsClassifier()
clf3.fit(data_train, target_train)
clf3.predict(data_test)
target_test

#-----------Bledy realne-------------
from sklearn.cross_validation import cross_val_score
iris_length = len(iris.data)
generalization_error1 = 0
for x in xrange(iris_length):
	generalization_error1 += float(1/iris_length) * (1-clf1.score([iris.data[x]], [iris.target[x]]))

print(generalization_error1)

generalization_error2 = 0
for x in xrange(iris_length):
	generalization_error2 += float(1/iris_length) * (1-clf2.score([iris.data[x]], [iris.target[x]]))

print(generalization_error2)

generalization_error3 = 0
for x in xrange(iris_length):
	generalization_error3 += float(1/iris_length) * (1-clf3.score([iris.data[x]], [iris.target[x]]))

print(generalization_error3)
