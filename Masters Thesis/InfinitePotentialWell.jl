using PyPlot

# Przypadek niesymetryczny (jeden skok potencjału w 0, a drugi w d)
function Ψ²(L, x, t)
    ħ = 1
    E₁ = 1
    E₂ = 4
    return (1/L)*(sin((π*x)/L)^2 + sin((2*π*x)/L)^2 + 2*cos(((E₂ - E₁)*t)/ħ)*sin((π*x)/L)*sin((2*π*x)/L))
end # Ψ²

function ϕ₁(L, x, t)
    return (1/L)*(sin((π*x)/L)^2)
end # Ψ²

function ϕ₂(L, x, t)
    return (1/L)*(sin((2*π*x)/L)^2)
end # Ψ²

function ϕ₁ϕ₂(L, x, t)
    ħ = 1
    E₁ = 1
    E₂ = 4
    return (1/L)*(2*cos(((E₂ - E₁)*t)/ħ)*sin((π*x)/L)*sin((2*π*x)/L))
end # Ψ²

function Plot(L, location, ax, color)
    x = []
    y = []
    for t ∈ 0:0.1:5
        push!(x, t)
        result = Ψ²(L, location, t)
        push!(y, result)
    end
    # println("y: ", y)
    ax[:plot](x, y, color="$color", "-")
end # Plot

function PlotSpace(L, t, ax, color)
    x = []
    y = []
    for location ∈ 0:0.01:L
        push!(x, location)
        result = Ψ²(L, location, t)
        push!(y, result)
    end
    println("Całka: ", sum(y)/length(y))
    ax[:plot](x, y, label="t: $t", color="$color", "-")
    ax[:legend]()
    grid("on")
end # PlotSpace


function PlotSpace2(L, t)
    fig, ax = PyPlot.subplots()
    x = []
    y = []
    ϕ₁ˈs = []
    ϕ₂ˈs = []
    ϕ₁ϕ₂ˈs = []
    for location ∈ 0:0.01:L
        push!(x, location)
        # result = Ψ²(L, location, t)
        push!(ϕ₁ˈs, ϕ₁(L, location, t))
        push!(ϕ₂ˈs, ϕ₂(L, location, t))
        push!(ϕ₁ϕ₂ˈs, ϕ₁ϕ₂(L, location, t))
        push!(y, Ψ²(L, location, t))
    end
    println("Całość: ", sum(y)/length(y))
    println("1: ", sum(ϕ₁ˈs)/length(ϕ₁ˈs))
    println("2: ", sum(ϕ₂ˈs)/length(ϕ₂ˈs))
    println("3: ", sum(ϕ₁ϕ₂ˈs)/length(ϕ₁ϕ₂ˈs))
    ax[:plot](x, y, color="black", "-")
    ax[:plot](x, ϕ₁ˈs, color="red", "-")
    ax[:plot](x, ϕ₂ˈs, color="blue", "-")
    ax[:plot](x, ϕ₁ϕ₂ˈs, color="green", "-")
    grid("on")
end # PlotSpace2

function PlotSpaceE₁(L, t, ax, color)
    x = []
    y = []
    for location ∈ 0:0.01:L
        push!(x, location)
        push!(y, ϕ₁(L, location, t))
    end
    println("Całka: ", sum(y)/length(y))
    ax[:plot](x, y, label="t: $t", color="$color", "-")
    ax[:legend]()
    grid("on")
end # PlotSpace

function PlotSpaceE₂(L, t, ax, color)
    x = []
    y = []
    for location ∈ 0:0.01:L
        push!(x, location)
        push!(y, ϕ₂(L, location, t))
    end
    println("Całka: ", sum(y)/length(y))
    ax[:plot](x, y, label="t: $t", color="$color", "-")
    ax[:legend]()
    grid("on")
end # PlotSpace

L = 1

# fig, ax = PyPlot.subplots()
# Plot(L, 0, ax, "blue")
# Plot(L, 0.25*L, ax, "green")
# Plot(L, 0.75*L, ax, "red")
# Plot(L, L, ax, "black")


# PlotSpace2(L, 0)
# PlotSpace2(L, 0.5)
# PlotSpace2(L, 1)
# PlotSpace2(L, 2)

# fig, ax = PyPlot.subplots()
# PlotSpace(L, 0, ax, "orange")
# PlotSpace(L, 0.5, ax, "green")
# PlotSpace(L, 1, ax, "blue")
# fig, ax = PyPlot.subplots()
# PlotSpace(L, 0, ax, "grey")
# PlotSpace(L, 2, ax, "red")
# PlotSpace(L, 4, ax, "purple")

fig, ax = PyPlot.subplots()
PlotSpaceE₁(L, 0, ax, "orange")
PlotSpaceE₁(L, 0.5, ax, "green")
PlotSpaceE₁(L, 1, ax, "blue")


fig, ax = PyPlot.subplots()
PlotSpaceE₂(L, 0, ax, "orange")
PlotSpaceE₂(L, 0.5, ax, "green")
PlotSpaceE₂(L, 1, ax, "blue")
