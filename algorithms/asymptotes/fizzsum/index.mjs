import assert from "node:assert"

const _ = (x) => Math.floor(x)

const f = (x) => (n) => _(n/x) * x * (_(n/x)+1) / 2

const fizz = f(3)

assert.equal(fizz(0), 0)
assert.equal(fizz(2), 0)
assert.equal(fizz(3), 3)
assert.equal(fizz(5), 3)
assert.equal(fizz(6), 9)
assert.equal(fizz(8), 9)
assert.equal(fizz(9), 18)

const buzz = f(5)

assert.equal(buzz(0), 0)
assert.equal(buzz(4), 0)
assert.equal(buzz(5), 5)
assert.equal(buzz(9), 5)
assert.equal(buzz(10), 15)
assert.equal(buzz(14), 15)

const fizzbuzz = f(15)

assert.equal(fizzbuzz(0), 0)
assert.equal(fizzbuzz(15), 15)
assert.equal(fizzbuzz(30), 45)

function fizzsum(n) {
  return fizz(n) + buzz(n) - fizzbuzz(n)
}

assert.equal(fizzsum(0), 0)
assert.equal(fizzsum(3), 3)
assert.equal(fizzsum(5), 8)
assert.equal(fizzsum(6), 14)
assert.equal(fizzsum(8), 14)
assert.equal(fizzsum(9), 23)
assert.equal(fizzsum(10), 33)
assert.equal(fizzsum(12), 45)
assert.equal(fizzsum(15), 60)
assert.equal(fizzsum(18), 78)
assert.equal(fizzsum(20), 98)
assert.equal(fizzsum(21), 119)
assert.equal(fizzsum(24), 143)
assert.equal(fizzsum(25), 168)

