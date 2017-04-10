import sklearn
import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import datasets
iris = datasets.load_iris()

raw_data = {'Sepal length': [x[0] for x in iris.data], 'Sepal width': [x[1] for x in iris.data], 'Petal length': [x[2] for x in iris.data], 'Petal width': [x[3] for x in iris.data], 'Class':iris.target}
df = pd.DataFrame(raw_data, columns = ['Sepal length', 'Sepal width', 'Petal length', 'Petal width', 'Class'])
print(df.describe())
print(df.groupby(df['Class']).describe())

#-------Plots-------
feature_1 = [x[0] for x in iris.data]
feature_2 = [x[1] for x in iris.data]
feature_3 = [x[2] for x in iris.data]
feature_4 = [x[3] for x in iris.data]
plt.clf()

#First feature
plt.boxplot(feature_1) 
#plt.show()
plt.savefig('Feature_1 - Box.png')
plt.clf()
plt.hist(feature_1)
plt.title("Histogram of first feature")
plt.xlabel("Value")
plt.ylabel("Frequency")
#plt.show()
plt.savefig('Feature_1 - Histogram.png')
plt.clf()

#Second feature
plt.boxplot(feature_2)
#plt.show()
plt.savefig('Feature_2 - Box.png')
plt.clf()
plt.hist(feature_2)
plt.title("Histogram of second feature")
plt.xlabel("Value")
plt.ylabel("Frequency")
#plt.show()
plt.savefig('Feature_2 - Histogram.png')
plt.clf()

#Third feature
plt.boxplot(feature_3)
#plt.show()
plt.savefig('Feature_3 - Box.png')
plt.clf()
plt.hist(feature_3)
plt.title("Histogram of third feature")
plt.xlabel("Value")
plt.ylabel("Frequency")
#plt.show()
plt.savefig('Feature_3 - Histogram.png')
plt.clf()

#Fourth feature
plt.boxplot(feature_4)
#plt.show()
plt.savefig('Feature_4 - Box.png')
plt.clf()
plt.hist(feature_4)
plt.title("Histogram of fourth feature")
plt.xlabel("Value")
plt.ylabel("Frequency")
#plt.show()
plt.savefig('Feature_4 - Histogram.png')
plt.clf()

#Inna licznosc
np.unique(feature_1) #Wyswietlenie unikalnych elementow tablicy
len(np.unique(feature_1)) #Wyswietlenie liczby unikalnych elementow tablicy
len(np.unique(feature_2))
len(np.unique(feature_3))
len(np.unique(feature_4))
