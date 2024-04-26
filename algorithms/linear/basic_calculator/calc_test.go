package calc

import "testing"

func TestCalc0(t *testing.T) {
  want := 0
  got, err := Calc("0")

  if got != want || err != nil {
    t.Fatalf(`Calc('0') = %v`, got)  
  }
}

func TestCalc1(t *testing.T) {
  want := 1
  got, err  := Calc("1")

  if got != want || err != nil {
    t.Fatalf(`Calc('1') = %v`, got)  
  }
}

func TestCalcEmptyExp(t *testing.T) {
  want := 1
  got, err  := Calc("1")

  if got != want || err != nil {
    t.Fatalf(`Calc('0') = %v`, got)  
  }
}
