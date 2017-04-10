using PyPlot
using Nettle

function ϵ(x, A)
  return in(x, A)
end

function max_val(len)
  sum = BigInt(0)
  multiplier = BigInt(1)
  for x in 1:len
    sum = sum + multiplier
    multiplier = multiplier * 2
  end
  return sum
end

function h_(x, mode::String, len)
  hash = parse(BigInt, hexdigest(mode, string(x)), 16)
  #println(length(bin(hash)))
  max = max_val(len)
  return BigFloat(hash / max)
end

function h(x)
  return h_(x, "md5", 128)
end

function counting(k, Multiset)
  M = fill(BigFloat(1.0), k)
  for x in Multiset
    h_x = h(x)
    if h_x < M[k] && ϵ(h_x, M) == false
      M[k] = h_x
      M = sort(M)
    end
  end
  if M[k] == 1
    return length([x for x in M if x != 1.0])
  else
    return Float64((k - 1) / M[k])
  end
end

function zad_1(k, n)
    x = collect(1:n)
    y = Float64[]
    for i in 1:n
      if (i % 25 == 0)
        println(i)
      end
      push!(y, counting(k, collect(1:i)) / i)
    end
    close()
    plot(x, y, "bo")
end

function zad_1_all(n)
  k_ = [2, 3, 10, 100, 400]
  for k in k_
    println("Counting for k = ", k, "...")
    x = collect(1:n)
    y = Float64[]
    for i in 1:n
      push!(y, counting(k, collect(1:i)) / i)
    end
    close()
    plot(x, y, "bo")
    title = string("k = ", k, ", n = ", n)
    savefig(title)
  end
  println("Done")
end

#zad_1_all(10^4)

#--------- Influence of repetitions -------------
arr = collect(1:10^4)
#println(Float64(counting(196, arr) / 10^4)) # >> 0.9948098248501568
for i in 5000:10000
  arr[i] = 5000
end
#println(Float64(counting(196, collect(1:5000)) / 5000)) # >> 1.083018513534656
#println(Float64(counting(196, arr) / 5000)) # >> 1.083018513534656

arr = collect(1:10^4)
#println(Float64(counting(400, arr) / 10^4)) # >> 1.0575202099619931
for i in 7500:10000
  arr[i] = 7500
end
#println(Float64(counting(400, collect(1:7500)) / 7500)) # >> 1.058534893641112
#println(Float64(counting(400, arr) / 7500)) # >> 1.058534893641112

#------------------- Zad 2 --------------------

function zad_2_jump(k, n)
  successes = 0
  experiments = 0
  for i in 100:100:n
    println("s = ", successes)
    experiments = experiments + 1
    if i % 100 == 0
      println(i)
    end
    if abs(BigFloat(counting(k, collect(1:i)) / i ) - 1) < 0.1
      successes = successes + 1
    end
  end
  println("suc = ", successes)
  return Float16(successes / (n / 100))
end

function zad_2(k, n)
  successes = 0
  for i in 1:n
    if i % 100 == 0
      println(i)
    end
    if abs(BigFloat(counting(k, collect(1:i)) / i) - 1) < 0.1
      successes = successes + 1
    end
  end
  return Float16(successes / n)
end

#zad_2(302, 5000) # >> 0.912
#println(zad_2(250, 1000)) # >> 0.935
#println(zad_2(252, 1000)) # >> 0.945
#println(zad_2(253, 1000)) # >> 0.953 !!!
#println(zad_2(255, 1000)) # >> 0.959
#println(zad_2(260, 1000)) # >> 0.976
#println(zad_2(290, 1000)) # >> 1.0
#println(zad_2(320, 1000)) # >> 1.0

#---------------- Zad_3 -------------------

function counting_(k, Multiset, mode::String, len)
  M = fill(BigFloat(1.0), k)
  for x in Multiset
    h_x = h_(x, mode, len)
    #println("1 - ", Float64(h_x), ", ", Float64(M[k]), " -> ", Float64(h_x) < Float64(M[k]))
    if h_x < BigFloat(M[k]) && ϵ(h_x, M) == false
      M[k] = h_x
      M = sort(M)
    end
  end
  if M[k] == 1
    return length([x for x in M if x != 1.0])
  else
    return Float64((k - 1) / M[k])
  end
end

function h_cut(x, mode::String, original_len, len)
  hash = parse(BigInt, hexdigest(mode, string(x)), 16)
  #println("h1 = ", hash)
  hash = hash >> (length(bin(hash)) - len + (original_len - length(bin(hash))))
  #println("h2 = ", hash)
  max = max_val(len)
  return BigFloat(hash / max)
end

function counting_cut(k, Multiset, mode::String, len)
  M = fill(BigFloat(1.0), k)
  for x in Multiset
    h_x = h_cut(x, mode, 128, len)
    if h_x < M[k] && ϵ(h_x, M) == false
      M[k] = h_x
      M = sort(M)
    end
  end
  if M[k] == 1
    return length([x for x in M if x != 1.0])
  else
    return Float64((k - 1) / M[k])
  end
end

#---------Different functions--------
#println(counting_(400, collect(1:10^4), "sha1", 160)) # >> 9671.053816925645
#println(counting_(400, collect(1:10^4), "sha224", 224)) # >> 9875.194406779327
#println(counting_(400, collect(1:10^4), "sha256", 256)) # >> 10184.792120178916
#println(counting_(400, collect(1:10^4), "sha384", 384)) # >> 10112.601941438461
#println(counting_(400, collect(1:10^4), "sha512", 512)) # >> 10611.217982336495
#println(counting_(400, collect(1:10^4), "md2", 128)) # >> 10247.405815811562
#println(counting_(400, collect(1:10^4), "md5", 128)) # >> 10575.202099619932
#println(counting_(400, collect(1:10^4), "ripemd160", 160)) # >> 10404.898084361615

#-----------Length of hash-------------

function length_of_hash(n)
    x = collect(5:20)
    y = Float64[]
    for i in x
      push!(y, counting_cut(400, collect(1:n), "md5", i))
    end
    close()
    plot(x, y, "bo")
end

length_of_hash(10000)

#println(counting(400, collect(1:10^5))) # >> 96692.76824112699
#println(counting_cut(400, collect(1:10^5), "md5", 21)) # >> 96201.79914922972
#println(counting_cut(400, collect(1:10^5), "md5", 20)) # >> 93347.03815261045
#println(counting_cut(400, collect(1:10^5), "md5", 19)) # >> 88154.45132743364
#---------
#println(counting(400, collect(1:10^4))) # >> 10575.202099619932
#println(counting_cut(400, collect(1:10^4), "md5", 17)) # >> 10234.310958904109
#println(counting_cut(400, collect(1:10^4), "md5", 16)) # >> 9782.441077441077
#println(counting_cut(400, collect(1:10^4), "md5", 15)) # >> 9259.230169971672
#println(counting_cut(400, collect(1:10^4), "md5", 14)) # >> 7952.332116788321

#-------- Additional -----------

function plot_of_hash(n)
    x = collect(1:n)
    y = Float64[]
    for i in 1:n
      push!(y, h_(i, "md5", 128))
    end
    close()
    plot(x, y, "bo")
end

#plot_of_hash(10^4)
