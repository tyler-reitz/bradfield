import assert from "node:assert"


function parens(tokens) {
  const stack = []

  for (const token of tokens) {
    switch (token) {
      case "}": {
        if (stack.pop() !== "{") return false
        break
      }
      case "]": {
        if (stack.pop() !== "[") return false
        break
      }
      case ")": {
        if (stack.pop() !== "(") return false
        break
      }
      default:
        stack.push(token)
    }
  }

  return stack.length ? false : true
}

assert.equal(parens(""), true)

assert.equal(parens("("), false)
assert.equal(parens(")"), false)
assert.equal(parens("()"), true)
assert.equal(parens("(()"), false)
assert.equal(parens("(())"), true)
assert.equal(parens("())"), false)

assert.equal(parens("["), false)
assert.equal(parens("]"), false)
assert.equal(parens("[]"), true)
assert.equal(parens("[[]"), false)
assert.equal(parens("[[]]"), true)
assert.equal(parens("[]]"), false)

assert.equal(parens("(["), false)
assert.equal(parens("])"), false)
assert.equal(parens("([])"), true)
assert.equal(parens("([)])"), false)

assert.equal(parens("{"), false)
assert.equal(parens("}"), false)
assert.equal(parens("{}"), true)

assert.equal(parens("([{}])"), true)
assert.equal(parens("[[]]"), true)

