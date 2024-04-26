package calc

// import "fmt"

func Calc(expr string) (int, error) {
  stack := []rune{}

  for _, ch := range expr {
    switch ch {
      case '0':
        stack = append(stack, ch)
      case '1':
        stack = append(stack, ch)
      case ' ':
        continue
    }
  }

  return int(stack[0] - '0'), nil
}

