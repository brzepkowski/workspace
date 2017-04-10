#Obliczenia naukowe - Lista 3
#Bartosz Rzepkowski - 22.11.2015

module Metody
export mbisekcji
export mstycznych
export msiecznych

function mbisekcji(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
  err = 0
  u = f(a)
  w = f(b)
  e = b-a
  if sign(u) == sign(w)
    err = 1
    return a, b, 0, err
  end
  for it = 1: 100000
    e = e / 2
    r = a + e
    v = f(r)
    if abs(e) < delta || abs(v) < epsilon
      return r, v, it, err
    end
    if sign(v) != sign(u)
      b = r
      w = v
    else
      a = r
      u = v
    end
  end
end

function mstycznych(f, pf, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
  err = 0
  v = f(x0)
  if pf == 0
    err = 1
    return x0, v, 0, err
  end
  if abs(v) < epsilon
    return x0, v, 0, err
  end
  for it = 1: maxit
    r = x0 - v / pf(x0)
    v = f(r)
    if abs(r - x0) < delta || abs(v) < epsilon
      return r, v, it, err
    end
    x0 = r
  end
end

function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64)
  a = x0
  b = x1
  err = 0
  fa = f(a)
  fb = f(b)
  if sign(fa) == sign(fb)
    err = 1
    return x0, x1, 0, err
  end
  for it = 1: 100000
    if abs(fa) > abs(fb)
      buffer = a
      a = b
      b = buffer
      buffer = fa
      fa = fb
      fb = buffer
    end
    s = (b - a) / (fb - fa)
    b = a
    fb = fa
    a = a - fa*s
    fa = f(a)
    if abs(b - a) < delta || abs(fa) < epsilon
      return a, fa, it, err
    end
  end
end

end

