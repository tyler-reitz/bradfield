// Quicksort
package qsort

func Qsort(n []int) []int {
  if len(n) <= 1 {
    return n
  }

  i,j := 1,1
  for j<len(n) {
    if n[j] < n[0] {
      n[i], n[j] = n[j], n[i]
      i++
      j++
    } else {
      j++
    }
  }

  n[0], n[i-1] = n[i-1], n[0]
 
  Qsort(n[:i-1])
  Qsort(n[i:])
  
  return n
}
