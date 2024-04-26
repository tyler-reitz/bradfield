// Fast exponentiaion
package fast_expo

func FastExpo(base, exp int) (res int) {
  if exp == 0 {
    return 1
  }

  if exp == 1 {
    return base
  }

  res = base * base

  var i int
  for i=2; i*2<=exp; i*=2 {
    res *= res
  }

  return res * FastExpo(base, exp % i)
}
