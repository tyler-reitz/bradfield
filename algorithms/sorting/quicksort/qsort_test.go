// Test Qsort
package qsort

import "testing"

func TestQuickSort(t *testing.T) {
  cases := []struct{
    _case, want []int
  }{
    {_case: []int{}, want: []int{}},
    {_case: []int{0}, want: []int{0}},
    {_case: []int{1, 0}, want: []int{0, 1}},
    {_case: []int{0, -1, -2, -3}, want: []int{-3, -2, -1, 0}},
    {_case: []int{4, 6, 3, 5, 2}, want: []int{2, 3, 4, 5, 6}},
    {
      _case: []int{5, 6, 3, 8, 2, 1, 9, 0, 7, 4},
      want: []int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    },
  }

  for _,_case := range cases {
    got   := Qsort(_case._case)

    for i, val := range _case.want {
      if got[i] != val {
        t.Fatalf("Qsort(%v) = %v, want %v", _case._case, got, _case.want)
      }
    }
  }
}
