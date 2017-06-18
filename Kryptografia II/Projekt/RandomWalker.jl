using LightGraphs


function randomWalker(N, d, l)
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  indexes = Dict{Int, Int}()

  # Zrobione do umożliwienia iterowania od 0
  for i in 0:N
    indexes[i] = i+1
  end

  s = collect(0:N-1)
  for i in 1:d
    push!(S, s[randperm(length(s))])
  end

  vₖ = 0
  for k in 0:l-1
    # println(S[indexes[k % d]])
    # println("k = ", k ,"(", indexes[k], ") , k mod d = ", k % d,"(", indexes[k%d], ") , vₖ = ", vₖ, "(", indexes[vₖ], ") , vₖ+1 = ", S[indexes[k % d]][vₖ + 1], "(", indexes[S[indexes[k % d]][vₖ + 1]] , ")")
    vₖ = S[indexes[k % d]][vₖ + 1]
  end
  println(vₖ)
end

for i in 1:100
  randomWalker(10, 2, 6)
end
