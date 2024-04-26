// Merge sort
package mergeSort

func merge(a, b []int) []int {
  res := []int{}

  var i,j = 0,0
  for i<len(a)&&j<len(b) {
    if a[i] < b[j] {
      res = append(res, a[i])
      i++
    } else {
      res = append(res, b[j])
      j++
    }
  }

  for i < len(a) {
    res = append(res, a[i])
    i++
  }

  for j < len(b) {
    res = append(res, b[j])
    j++
  }

  return res
}

func MergeSort(n []int) []int {
  l := len(n)

  if l <= 1 {
    return n
  } 
  
  a, b := MergeSort(n[:l/2]), MergeSort(n[l/2:])

  return merge(a, b)
}

