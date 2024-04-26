// Tests Merge Sort package
package mergeSort

import (
  "testing"
)

func TestMerge(t *testing.T) {
  cases := []struct{
    a, b, want []int
  }{
    {a: []int{0}, b: []int{}, want: []int{0}},
    {a: []int{}, b: []int{0}, want: []int{0}},

    {a: []int{0}, b: []int{0}, want: []int{0,0}},
    {a: []int{0}, b: []int{1}, want: []int{0,1}},
    {a: []int{1}, b: []int{0}, want: []int{0,1}},

    {a: []int{0}, b: []int{1,2}, want: []int{0,1,2}},
    {a: []int{0,1}, b: []int{2}, want: []int{0,1,2}},
    {a: []int{0,2}, b: []int{1}, want: []int{0,1,2}},

    {a: []int{3}, b: []int{7}, want: []int{3,7}},
    {a: []int{6}, b: []int{2}, want: []int{2,6}},
    {a: []int{4}, b: []int{2,6}, want: []int{2,4,6}},

    {a: []int{3,7}, b: []int{2,4,6}, want: []int{2,3,4,6,7}},

    {a: []int{8}, b: []int{1}, want: []int{1,8}},
    {a: []int{0}, b: []int{5}, want: []int{0,5}},
    {a: []int{9}, b: []int{0,5}, want: []int{0,5,9}},

    {a: []int{1,8}, b: []int{0,5,9}, want: []int{0,1,5,8,9}},

    {a: []int{2,3,4,6,7}, b: []int{0,1,5,8,9}, want: []int{0,1,2,3,4,5,6,7,8,9}},
  }

  for _,c := range cases {
    got := merge(c.a, c.b)

    for i,v := range c.want {
      if got[i] != v {
        t.Errorf("Merge(%v, %v) = %v, want %v", c.a, c.b, got, c.want)
      }
    }
  }

}

func TestMergeSort(t *testing.T) {
  cases := []struct{
    vals, want []int
  }{
    {
      vals: []int{3, 7, 4, 6, 2, 8, 1, 9, 0, 5},
      want: []int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    },
    {
      vals: []int{0, -1, -2, -3, -4, -5, -6, -7, -8, -9},
      want: []int{-9, -8, -7, -6, -5, -4, -3, -2, -1, 0},
    },
  }

  for _,c := range cases {
    got := MergeSort(c.vals)
    for i, val := range c.want {
      if val != got[i] {
        t.Errorf("MergeSort(} = %v, wanted %v", got, c.want)
      }
    }
  }
}

