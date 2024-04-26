import assert from "node:assert"

function calc(expr) {
  const stack = []

  for (const ch of expr) {
    switch (ch) {
      case ')': {
        const [ a, op, b ] = stack.splice(stack.length-3)

        switch (op) {
          case '+':
            stack.push(parseInt(a) + parseInt(b))
            break
          case '-':
            stack.push(parseInt(a) - parseInt(b))
            break
          default:
            throw('')
        }
      }  
      case '(':
      case ' ':
        continue
      default:
        stack.push(ch)
    }
  } 

  return stack.pop()
}

assert.equal(calc('0'), 0)
assert.equal(calc('1'), 1)

assert.equal(calc('(1+1)'), 2)
assert.equal(calc('(1-1)'), 0)
assert.equal(calc('(3-2)'), 1)

assert.equal(calc('((1+1)+1)'), 3)
assert.equal(calc('(1+(1+1))'), 3)

assert.equal(calc('(1+2)'), 3)
assert.equal(calc('(1 - (1 - 1))'), 1)
assert.equal(calc('((1 - 1) - 1)'), -1)
assert.equal(calc('((1 + 2) - (3 + 4))'), -4)

console.log("ok")

