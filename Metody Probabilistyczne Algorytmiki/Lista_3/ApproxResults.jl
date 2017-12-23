using PyPlot

x = collect(1000:1000:10000)

Max1 = [11.66, 13.05, 14.82, 15.39, 16.09, 16.21, 16.97, 17.62, 18.13, 18.09]
SecondMax1 = [7.98, 9.69, 10.79, 11.61, 12.04, 12.44, 12.88, 13.92, 13.45, 13.83]

Max2 = [33.17, 48.98, 60.09, 74.05, 83.8, 85.16, 105.55, 105.69, 112.21, 128.31]
SecondMax2 =  [18.75, 28.33, 34.96, 43.9, 49.87, 55.18, 60.21, 61.39, 66.3, 75.14]

Max3 = [283.07, 483.96, 736.69, 898.05, 1112.9, 1255.11, 1442.21, 1559.26, 1747.3, 1824.55]
SecondMax3 = [35.53, 55.3, 66.56, 80.01, 86.41, 102.42, 114.42, 125.56, 140.04, 145.46]

Max4 = [32.04, 50.72, 68.56, 85.75, 94.95, 110.38, 128.45, 130.83, 136.05, 150.16]
SecondMax4 = [18.58, 29.82, 38.26, 47.4, 53.43, 58.63, 66.12, 73.25, 78.86, 82.89]

Max5 = [275.73, 462.89, 619.71, 745.46, 940.47, 1015.36, 1223.98, 1244.98, 1399.62, 1516.83]
SecondMax5 = [37.21, 57.2, 83.64, 95.67, 105.89, 130.59, 126.98, 164.08, 157.14, 166.26]

Max6 = [797.38, 1588.67, 2390.78, 3191.14, 3982.29, 4778.64, 5573.52, 6378.6, 7159.94, 7960.65]
SecondMax6 = [5.9, 7.3, 7.67, 8.35, 8.55, 8.95, 9.1, 9.75, 10.29, 10.36]

function ApproxResults(x, results, funct)
    fig, ax = PyPlot.subplots()
    y = []
    foreach(i -> push!(y, funct(i)), x)
    ax[:plot](x, results, color="red", "-")
    ax[:plot](x, y, color="black", "-")
end # ApproxResults

function PrintAllMaxes(x, arr1, arr2, arr3, arr4, arr5, arr6)
    fig, ax = PyPlot.subplots()
    ax[:plot](x, arr1, color="red", "-")
    ax[:plot](x, arr2, color="red", "-")
    ax[:plot](x, arr3, color="red", "-")
    ax[:plot](x, arr4, color="red", "-")
    ax[:plot](x, arr5, color="red", "-")
    ax[:plot](x, arr6, color="red", "-")
end

function PrintAllSecondMaxes(x, arr1, arr2, arr3, arr4, arr5, arr6)
    fig, ax = PyPlot.subplots()
    ax[:plot](x, arr1, color="blue", "-")
    ax[:plot](x, arr2, color="blue", "-")
    ax[:plot](x, arr3, color="blue", "-")
    ax[:plot](x, arr4, color="blue", "-")
    ax[:plot](x, arr5, color="blue", "-")
    ax[:plot](x, arr6, color="blue", "-")
end

function f1(x)
    return 1.37*log2(x)
end # f1

function f2(x)
    return 1.03*log2(x)
end # f2

function f3(x)
    return 0.5*x^(3/5)
end # f3

function f4(x)
    return 0.75*x^(3/6)
end # f4

function f5(x)
    return 0.9*x^(5/6)
end # f5

function f6(x)
    return 0.58*x^(3/5)
end # f6

function f7(x)
    return 0.65*x^(3/5)
end # f7

function f8(x)
    return 0.85*x^(3/6)
end # f8

function f9(x)
    return 1.52*x^(3/4)
end # f9

function f10(x)
    return 0.67*x^(3/5)
end # f10

function f11(x)
    return 0.8*x
end # f11

function f12(x)
    return 0.79*log2(x)
end # f12

# (1) 1 / 2n
# ApproxResults(x, Max1, f1)
# ApproxResults(x, SecondMax1, f2)

# (2) 1/n - (n^0.1)/(n^4/3)
# ApproxResults(x, Max2, f3)
# ApproxResults(x, SecondMax2, f4)

# (3) 1/n + (n^0.1)/(n^4/3)
# ApproxResults(x, Max3, f5)
# ApproxResults(x, SecondMax3, f6)

# (4) 1/n - 2/(n^4/3)
# ApproxResults(x, Max4, f7)
# ApproxResults(x, SecondMax4, f8)

# (5) 1/n + 2/(n^4/3)
# ApproxResults(x, Max5, f9)
# ApproxResults(x, SecondMax5, f10)

# (6) 2/n
# ApproxResults(x, Max6, f11)
# ApproxResults(x, SecondMax6, f12)

# PrintAllMaxes(x, Max1, Max2, Max3, Max4, Max5, Max6)
# PrintAllSecondMaxes(x, SecondMax1, SecondMax2, SecondMax3, SecondMax4, SecondMax5, SecondMax6)
