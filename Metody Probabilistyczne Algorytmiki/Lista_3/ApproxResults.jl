using PyPlot

x = collect(1000:1000:10000)
max1 = [11.66, 13.05, 14.82, 15.39, 16.09, 16.21, 16.97, 17.62, 18.13, 18.09]
secondMax1 = [7.98, 9.69, 10.79, 11.61, 12.04, 12.44, 12.88, 13.92, 13.45, 13.83]

function ApproxResults(x, results, funct)
    fig, ax = PyPlot.subplots()
    y = []
    foreach(i -> push!(y, funct(i)), x)
    ax[:plot](x, results, color="red", "-")
    ax[:plot](x, y, color="black", "-")
end # ApproxResults

function f1(x)
    return 1.35*log2(x)
end # f1

function f2(x)
    return 1.03*log2(x)
end # f2

# ApproxResults(x, max1, f1)
ApproxResults(x, secondMax1, f2)
