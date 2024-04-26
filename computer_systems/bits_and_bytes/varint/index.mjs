import fs from "node:fs"
import assert from "node:assert"

const path = process.argv[2]

const encode = (n) => {
  let acc = []

  while (n) {
    let part = n & ~-128n
    if (n >>= 7n) part |= 128n
    acc = [ ...acc, Number(part) ]
  }

  return Buffer.from(acc)
}

const decode = (buf) => {
  let n = 0n

  for (let i=0n; i<buf.length; i++) {
    n |= (BigInt(buf[i]) & ~-0x80n) << i * 7n
  }

  return n;
}

if (path) {
  const buf = fs.readFileSync(path)

  const varint = encode(buf.readBigUInt64BE())
  console.log( varint )

  const int = decode(varint)
  console.log( int )

  assert.equal(int, buf.readBigUInt64BE())
}

const buf0 = Buffer.from([ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, ])

const buf1 = Buffer.from([ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, ])

const buf2 = Buffer.from([ 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, ])

const buf3 = Buffer.from([ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ])

const buf4 = Buffer.from([ 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, ])

const buf5 = Buffer.from([ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, ])

const buf6 = Buffer.from([ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, ])

assert.equal(decode(encode(buf0.readBigUInt64BE())), buf0.readBigUInt64BE())
assert.equal(decode(encode(buf1.readBigUInt64BE())), buf1.readBigUInt64BE())
assert.equal(decode(encode(buf2.readBigUInt64BE())), buf2.readBigUInt64BE())
assert.equal(decode(encode(buf3.readBigUInt64BE())), buf3.readBigUInt64BE())
assert.equal(decode(encode(buf4.readBigUInt64BE())), buf4.readBigUInt64BE())
assert.equal(decode(encode(buf5.readBigUInt64BE())), buf5.readBigUInt64BE())
assert.equal(decode(encode(buf6.readBigUInt64BE())), buf6.readBigUInt64BE())

