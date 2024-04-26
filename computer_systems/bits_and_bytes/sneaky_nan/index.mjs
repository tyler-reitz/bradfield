import assert from "node:assert"

export const conceal = (msg) => {
  const bits = [ 0x7F, 0xF0|8|msg.length, ...Buffer.alloc(6, msg) ]

  return Buffer.from(bits).readDoubleBE()
}

export const extract = (val) => {
  const buf = Buffer.alloc(8)

  buf.writeDoubleBE(val)

  return buf.slice(2, 2+(buf[1]&7)).toString()
}

assert.equal(extract(conceal('hello!')), 'hello!')
assert.equal(extract(conceal('foo')), 'foo')
