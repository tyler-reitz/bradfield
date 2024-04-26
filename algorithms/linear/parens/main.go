package main

import "testing"

func main() {
  Parens("")
}

func Parens(s string) bool {
  return true
}

func TestParens(t *testing.T) {
  got := Parens("")

  if got != true {
    t.Errorf("Parens(\"\") = %v; want true", got)
  }
}
