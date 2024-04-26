import assert from "node:assert"

function ascend(n) {
  if (n === 0) return 1  
  if (n < 0) return 0

  return ascend(n-1) + ascend(n-2) + ascend(n-3)
}

assert.equal(ascend(0), 1)
assert.equal(ascend(1), 1)
assert.equal(ascend(2), 2)
assert.equal(ascend(3), 4)
assert.equal(ascend(4), 7)
assert.equal(ascend(5), 13)

