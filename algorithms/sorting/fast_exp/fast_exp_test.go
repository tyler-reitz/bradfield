// Test
package fast_expo

import "testing"

func TestFastExp(t *testing.T) {
  cases := []struct{
    base, exp, want int
  }{
    {base: 2, exp:0, want: 1},
    {base: 2, exp:1, want: 2},
    {base: 2, exp:2, want: 4},
    {base: 2, exp:3, want: 8},
    {base: 2, exp:4, want: 16},
    {base: 2, exp:5, want: 32},
    {base: 2, exp:6, want: 64},
    {base: 2, exp:7, want: 128},
    {base: 2, exp:8, want: 256},
    {base: 2, exp:16, want: 65536},

    {base: 3, exp:1, want: 3},
    {base: 3, exp:2, want: 9},
    {base: 3, exp:3, want: 27},
    {base: 3, exp:4, want: 81},
  }

  for _,c := range cases {
    got := FastExpo(c.base, c.exp)

    if got != c.want {
      t.Errorf("FastExp(%v,%v) = %v, want %v", c.base, c.exp, got, c.want)
    }
  }
}
