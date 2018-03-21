#!/usr/bin/python

# M. P. Polak & P. Scharoch 2017
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

def function1D(x):
	return x*np.sin(x)

def function2D(x, y):
	return np.sin(x) + np.cos(y)

def tabulateFunction1D(function, numberOfSteps, xₗ, xᵣ, outputFileName):
	Δ = (xᵣ - xₗ) / (numberOfSteps - 1)
	file = open(outputFileName,'w')
	xarr = []
	yarr = []
	x = xₗ - Δ # We set beginning of tabulation at xₗ
	for i in range(0, numberOfSteps + 1):
		x = x + Δ
		xarr.append(x)
		yarr.append(function(x))
        # file.write(str(x)+" "+str(function(x))+"\n")
		string = str(x) + ": " + str(function(x)) + "\n"
		file.write(string)
	file.close()
	plt.plot(xarr, yarr, 'b-')
	plt.show()

def tabulateFunction2D(function, numberOfSteps1st, numberOfSteps2nd, xₗ, xᵣ, yₗ, yᵣ, outputFileName):
	fig = plt.figure()
	ax = plt.axes(projection='3d')
	Δ1 = (xᵣ - xₗ) / (numberOfSteps1st - 1)
	Δ2 = (yᵣ - yₗ) / (numberOfSteps2nd - 1)
	file = open(outputFileName,'w')
	xarr = []
	yarr = []
	zarr = []
	x = xₗ - Δ1 # We set beginning of tabulation at xₗ
	for i in range(0, numberOfSteps1st + 1):
		x = x + Δ1
		y = yₗ - Δ2
		for j in range(0, numberOfSteps2nd + 1):
			y = y + Δ2
			xarr.append(x)
			yarr.append(y)
			zarr.append(function(x, y))
			string = str(x) + ", " + str(y) + ": " + str(function(x, y)) + "\n"
			file.write(string)
	file.close()
	# ax.scatter3D(xarr, yarr, zarr, c=zarr)
	# -----------Dodatkowe----------
	xarr = np.linspace(xₗ, xᵣ, numberOfSteps1st)
	yarr = np.linspace(yₗ, yᵣ, numberOfSteps2nd)
	X, Y = np.meshgrid(xarr, yarr)
	Z = function2D(X, Y)
	# ax.contour3D(X, Y, Z, 50, cmap='binary')
	ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none')
	plt.show()

# tabulateFunction1D(function1D, 1000, -10.0, 10.0, "output.txt")
# tabulateFunction2D(function2D, 100, 100, -10.0, 10.0, -10.0, 10.0, "output.txt")
